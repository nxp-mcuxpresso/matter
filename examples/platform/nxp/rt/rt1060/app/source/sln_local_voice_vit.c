/*
 * Copyright 2022-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_VIT_ASR

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Include file system */
#include "sln_flash.h"
#include "sln_flash_files.h"
#include "sln_flash_fs_ops.h"

/* Drivers, settings */
#include "sln_rgb_led_driver.h"
#include "sln_amplifier.h"
#include "sln_mic_config.h"
#include "sln_i2s_mic.h"

/* Structures, API */
#include "sln_local_voice_common.h"
#include "sln_local_voice_vit.h"
#include "IndexCommands.h"
#include "audio_processing_task.h"

/* VIT includes */
#include "PL_platformTypes_CortexM.h"
#include "VIT.h"

/* Needed for command filtering */
#include "app_layer.h"
#include "demo_actions.h"

#define NUMBER_OF_CHANNELS     1
#define ASR_INPUT_FRAMES       3
#define NUM_SAMPLES_AFE_OUTPUT (PCM_SINGLE_CH_SMPL_COUNT * ASR_INPUT_FRAMES)
#define DEVICE_ID VIT_IMXRT1060
#define MEMORY_ALIGNMENT 8 // in bytes

#define VIT_OPERATING_MODE_WW  (VIT_WAKEWORD_ENABLE)
#define VIT_OPERATING_MODE_CMD (VIT_VOICECMD_ENABLE)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* VIT memory pools sizes */
#define FAST_MEMORY_SIZE_BYTES  (192000)
#define SLOW_MEMORY_SIZE_BYTES  (151000)
#define MODEL_MEMORY_SIZE_BYTES (450000)

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern QueueHandle_t g_xSampleQueue;
extern volatile uint32_t g_wakeWordLength;
extern TaskHandle_t appTaskHandle;
extern oob_demo_control_t oob_demo_control;
extern bool g_SW1Pressed;
extern QueueHandle_t xMatterActionsQueue;

uint32_t g_sampleCount = 0;
app_asr_shell_commands_t appAsrShellCommands                      = {};

/* Configure the detection period in second for each command
   VIT will return UNKNOWN if no command is recognized during this time span.
   When only activating one mode, WWD or CMD, even timeout event trigger,
   no model switching happens automatically. Set it to 60s aligned with UI session.
 */
#define VIT_COMMAND_TIME_SPAN 8

typedef enum _asr_session
{
    ASR_SESSION_STOPPED,
    ASR_SESSION_WAKE_WORD,
    ASR_SESSION_VOICE_COMMAND,
} asr_session_t;
static asr_session_t s_asrSession = ASR_SESSION_STOPPED;

static VIT_Handle_t VITHandle = PL_NULL;      // VIT handle pointer
static VIT_InstanceParams_st VITInstParams;   // VIT instance parameters structure
static VIT_ControlParams_st VITControlParams; // VIT control parameters structure
static PL_MemoryTable_st VITMemoryTable;      // VIT memory table descriptor
static PL_BOOL InitPhase_Error = PL_FALSE;
static PL_INT8 *pMemory[PL_NR_MEMORY_REGIONS];

#if SELF_WAKE_UP_PROTECTION
static VIT_Handle_t VITHandleSelfWake = PL_NULL; // VIT handle pointer for self wake up engine
static PL_MemoryTable_st VITMemoryTableSelfWake;
static PL_INT8 *pMemorySelfWake[PL_NR_MEMORY_REGIONS];
#endif /* SELF_WAKE_UP_PROTECTION */

VIT_StatusParams_st VIT_StatusParams_Buffer;

static uint32_t s_vitFastMemoryUsed = 0;
//AT_NONCACHEABLE_SECTION_ALIGN_DTC(static int8_t s_vitFastMemory[FAST_MEMORY_SIZE_BYTES], 8);
SDK_ALIGN(uint8_t __attribute__((section(".bss.$SRAM_DTC"))) s_vitFastMemory[FAST_MEMORY_SIZE_BYTES], 8);
static uint32_t s_vitSlowMemoryUsed = 0;
//AT_CACHEABLE_SECTION_ALIGN_OCRAM(static int8_t s_vitSlowMemory[SLOW_MEMORY_SIZE_BYTES], 8);
SDK_ALIGN(uint8_t __attribute__((section(".bss.$SRAM_OC_NON_CACHEABLE"))) s_vitSlowMemory[SLOW_MEMORY_SIZE_BYTES], 8);
//AT_CACHEABLE_SECTION_ALIGN_OCRAM(static int8_t s_vitModelMemory[MODEL_MEMORY_SIZE_BYTES], 64);

#if SELF_WAKE_UP_PROTECTION
static uint32_t s_vitFastMemoryUsedSelfWake = 0;
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static int8_t s_vitFastMemorySelfWake[FAST_MEMORY_SIZE_BYTES], 8);
static uint32_t s_vitSlowMemoryUsedSelfWake = 0;
AT_CACHEABLE_SECTION_ALIGN_OCRAM(static int8_t s_vitSlowMemorySelfWake[SLOW_MEMORY_SIZE_BYTES], 8);
#endif /* SELF_WAKE_UP_PROTECTION */

typedef enum _cmd_state
{
    kWwConfirmed,
    kWwRejected,
    kWwNotSure,
} cmd_state_t;

/*******************************************************************************
 * Code
 ******************************************************************************/

void print_asr_version(void)
{
    VIT_LibInfo_st info;
    VIT_ReturnStatus_en status;
    status = VIT_GetLibInfo(&info);

    if (status != VIT_SUCCESS)
    {
        configPRINT_STRING("VIT_GetLibInfo error : %d\r\n", status);
    }
    else
    {
        configPRINT_STRING("ASR engine: VIT version 0x%04x\r\n", info.VIT_LIB_Release);
    }
}

static void asr_set_state(asr_session_t state)
{
    VIT_ReturnStatus_en VIT_Status = VIT_ERROR_UNDEFINED;
    s_asrSession                   = state;

    switch (state)
    {
        case ASR_SESSION_STOPPED:
            configPRINTF(("[ASR] Session stopped\r\n"));
            VITControlParams.OperatingMode = VIT_LPVAD_ENABLE;
            break;

        case ASR_SESSION_WAKE_WORD:
//            configPRINTF(("[ASR] Session waiting for Wake Word\r\n"));
            VITControlParams.OperatingMode = VIT_OPERATING_MODE_WW;
            break;

        case ASR_SESSION_VOICE_COMMAND:
//            configPRINTF(("[ASR] Session waiting for Voice Command\r\n"));
            VITControlParams.OperatingMode = VIT_OPERATING_MODE_CMD;
            break;

        default:
            configPRINTF(("Unknown state %d\r\n", state));
            break;
    }

    if (s_asrSession != ASR_SESSION_STOPPED)
    {
        VITControlParams.Feature_LowRes    = PL_FALSE;
        VITControlParams.Command_Time_Span = VIT_COMMAND_TIME_SPAN;

        VIT_Status = VIT_SetControlParameters(VITHandle, &VITControlParams);
#if SELF_WAKE_UP_PROTECTION
        VIT_Status = VIT_SetControlParameters(VITHandleSelfWake, &VITControlParams);
#endif /* SELF_WAKE_UP_PROTECTION */

        if (VIT_Status != VIT_SUCCESS)
        {
            configPRINTF(("[ASR] %d state failed %d\r\n", state, VIT_Status));
        }
    }
}

static VIT_ReturnStatus_en VIT_Deinit()
{
    VIT_ReturnStatus_en VIT_Status = VIT_SUCCESS;

    VIT_Status = VIT_GetMemoryTable(VITHandle, &VITMemoryTable, &VITInstParams);

    if (VIT_Status != VIT_SUCCESS)
    {
        configPRINTF(("VIT_GetMemoryTable error: %d\r\n", VIT_Status));
    }

    if (VIT_Status == VIT_SUCCESS)
    {
        // Free the MEM tables
        for (int i = 0; i < PL_NR_MEMORY_REGIONS; i++)
        {
            if (VITMemoryTable.Region[i].Size != 0)
            {
                memset(pMemory[i], 0, VITMemoryTable.Region[i].Size);
                pMemory[i] = NULL;
            }

    #if SELF_WAKE_UP_PROTECTION
            if (VITMemoryTableSelfWake.Region[i].Size != 0)
            {
                memset(pMemorySelfWake[i], 0, VITMemoryTableSelfWake.Region[i].Size);
                pMemorySelfWake[i] = NULL;
            }
    #endif /* SELF_WAKE_UP_PROTECTION */
        }
    }

    return VIT_Status;
}

static VIT_ReturnStatus_en VIT_Init(void)
{
    VIT_ReturnStatus_en VIT_Status = VIT_SUCCESS;

    uint8_t *asrModelAddr = NULL;

    asrModelAddr = get_demo_model(appAsrShellCommands.activeLanguage, appAsrShellCommands.demo);

    /* In case no demo found, boot in the default one */
    if (asrModelAddr == NULL)
    {
        asrModelAddr = get_demo_model(appAsrShellCommands.activeLanguage, DEFAULT_ASR_CMD_DEMO);
    }

    if (asrModelAddr == NULL)
    {
        configPRINTF(("VIT get model for %d failed. Pointer %p size %d for demo %d.\r\n", appAsrShellCommands.demo, asrModelAddr));
        VIT_Status = VIT_DUMMY_ERROR;
    }

    if (VIT_SUCCESS == VIT_Status)
    {
        VIT_Status = VIT_SetModel((const PL_UINT8 *)asrModelAddr, VIT_MODEL_IN_SLOW_MEM);
        if (VIT_Status != VIT_SUCCESS)
        {
            configPRINTF(("VIT_SetModel error: %d\r\n", VIT_Status));
        }
    }

    if (VIT_SUCCESS == VIT_Status)
    {
        /* Configure VIT Instance Parameters */
        VITInstParams.SampleRate_Hz   = VIT_SAMPLE_RATE;
        VITInstParams.SamplesPerFrame = VIT_SAMPLES_PER_30MS_FRAME;
        VITInstParams.NumberOfChannel = NUMBER_OF_CHANNELS;
        VITInstParams.DeviceId        = DEVICE_ID;
        VITInstParams.APIVersion      = VIT_API_VERSION;

        /* VIT get memory table: Get size info per memory type */
        VIT_Status = VIT_GetMemoryTable(PL_NULL, // VITHandle param should be NULL
                                        &VITMemoryTable, &VITInstParams);
        if (VIT_Status != VIT_SUCCESS)
        {
            configPRINTF(("VIT_GetMemoryTable error: %d\r\n", VIT_Status));
        }
    }

    if (VIT_SUCCESS == VIT_Status)
    {
        /* Reserve memory space: Malloc for each memory type */
        s_vitSlowMemoryUsed = 0;
        s_vitFastMemoryUsed = 0;
        for (int i = 0; i < PL_NR_MEMORY_REGIONS; i++)
        {
            /* Log the memory size */
            if (VITMemoryTable.Region[i].Size != 0)
            {
                /* reserve memory space
                   NB: VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_FAST_DATA] should be allocated
                   in the fastest memory of the platform (when possible) - this is not the case in this example.
                 */
                if (VITMemoryTable.Region[i].Type == PL_PERSISTENT_SLOW_DATA)
                {
                    pMemory[i] = (signed char *)&s_vitSlowMemory[s_vitSlowMemoryUsed];
                    s_vitSlowMemoryUsed += VITMemoryTable.Region[i].Size;
                    s_vitSlowMemoryUsed += MEMORY_ALIGNMENT - (s_vitSlowMemoryUsed % MEMORY_ALIGNMENT);
                }
                else
                {
                    pMemory[i] = (signed char *)&s_vitFastMemory[s_vitFastMemoryUsed];
                    s_vitFastMemoryUsed += VITMemoryTable.Region[i].Size;
                    s_vitFastMemoryUsed += MEMORY_ALIGNMENT - (s_vitFastMemoryUsed % MEMORY_ALIGNMENT);
                }
                VITMemoryTable.Region[i].pBaseAddress = (void *)pMemory[i];
            }
        }

        if (s_vitSlowMemoryUsed > SLOW_MEMORY_SIZE_BYTES)
        {
            configPRINTF(("VIT slow memory buffer is too small %d < %d.\r\n", SLOW_MEMORY_SIZE_BYTES, s_vitSlowMemoryUsed));
            vTaskDelay(100);
            while (1)
                ;
        }
        if (s_vitFastMemoryUsed > FAST_MEMORY_SIZE_BYTES)
        {
            configPRINTF(("VIT fast memory buffer is too small %d < %d.\r\n", FAST_MEMORY_SIZE_BYTES, s_vitFastMemoryUsed));
            vTaskDelay(100);
            while (1)
                ;
        }
    }

    if (VIT_SUCCESS == VIT_Status)
    {
        /* Create VIT Instance */
        VITHandle  = PL_NULL; // force to null address for correct memory initialization
        VIT_Status = VIT_GetInstanceHandle(&VITHandle, &VITMemoryTable, &VITInstParams);
        if (VIT_Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
            configPRINTF(("VIT_GetInstanceHandle error: %d\r\n", VIT_Status));
        }
    }

    /* Test the reset (OPTIONAL) */
    if (VIT_SUCCESS == VIT_Status)
    {
        VIT_Status = VIT_ResetInstance(VITHandle);
        if (VIT_Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
            configPRINTF(("VIT_ResetInstance error: %d\r\n", VIT_Status));
        }
    }
#if SELF_WAKE_UP_PROTECTION
    if (VIT_SUCCESS == VIT_Status)
    {
        VIT_Status = initialize_asr_self_wake_up();
    }
#endif /* SELF_WAKE_UP_PROTECTION */

    if (VIT_SUCCESS == VIT_Status)
    {
        /* Set and Apply VIT control parameters */
        asr_set_state(ASR_SESSION_WAKE_WORD);
    }

    return VIT_Status;
}

/*!
 * @brief ASR main task
 */
void local_voice_task(void *arg)
{
    int16_t pi16Sample[NUM_SAMPLES_AFE_OUTPUT];
    uint32_t len            = 0;
    uint32_t statusFlash    = 0;
    VIT_ReturnStatus_en VIT_Status;
    static VIT_WakeWord_st s_WakeWord;
    static VIT_VoiceCommand_st s_VoiceCommand;
    VIT_DetectionStatus_en VIT_DetectionResults         = VIT_NO_DETECTION;

    /* wait until mics are active */
    while (!SLN_MIC_GET_STATE())
    {
        vTaskDelay(1);
    }

    // Read Shell Commands Parameters from flash memory. If not available, initialize and write into flash memory.
    statusFlash = sln_flash_fs_ops_read(ASR_SHELL_COMMANDS_FILE_NAME, NULL, 0, &len);
    if ((statusFlash != SLN_FLASH_FS_OK) || len != sizeof(app_asr_shell_commands_t))
    {
        configPRINTF(("Failed reading local demo configuration from flash memory.\r\n"));
    }
    else
    {
        statusFlash = sln_flash_fs_ops_read(ASR_SHELL_COMMANDS_FILE_NAME, (uint8_t *)&appAsrShellCommands, 0, &len);

        if (statusFlash != SLN_FLASH_FS_OK)
        {
            configPRINTF(("Failed reading local demo configuration from flash memory.\r\n"));
        }
    }

    if ((appAsrShellCommands.status != WRITE_SUCCESS) ||
        (false == validate_all_active_languages(appAsrShellCommands.activeLanguage, appAsrShellCommands.demo)) ||
        (appAsrShellCommands.version != APP_ASR_SHELL_VERSION))
    {
        appAsrShellCommands.demo           = BOOT_ASR_CMD_DEMO;
        appAsrShellCommands.asrMode        = ASR_MODE_WW_AND_CMD;
        appAsrShellCommands.activeLanguage = DEFAULT_ASR_LANGUAGE;
        appAsrShellCommands.micsState      = ASR_MICS_ON;
        appAsrShellCommands.timeout        = VIT_COMMAND_TIME_SPAN * 1000;
        appAsrShellCommands.volume         = DEFAULT_SPEAKER_VOLUME;
        appAsrShellCommands.status         = WRITE_SUCCESS;
        appAsrShellCommands.asrCfg         = ASR_CFG_DEMO_NO_CHANGE;
        appAsrShellCommands.vitActive      = 1;
        appAsrShellCommands.version        = APP_ASR_SHELL_VERSION;

        sln_flash_fs_ops_erase(ASR_SHELL_COMMANDS_FILE_NAME);
        statusFlash = sln_flash_fs_ops_save(ASR_SHELL_COMMANDS_FILE_NAME, (uint8_t *)&appAsrShellCommands,
                                          sizeof(app_asr_shell_commands_t));
        if (statusFlash != SLN_FLASH_FS_OK)
        {
            configPRINTF(("Failed writing local demo configuration in flash memory.\r\n"));
        }
    }

    VIT_Init();

    // We need to reset asrCfg state so we won't remember an unprocessed demo change that was saved in flash
    appAsrShellCommands.asrCfg = ASR_CFG_DEMO_NO_CHANGE;

    while (!g_xSampleQueue)
        vTaskDelay(10);

    while (1)
    {
        if (xQueueReceive(g_xSampleQueue, pi16Sample, portMAX_DELAY) != pdPASS)
        {
            configPRINTF(("Could not receive from the queue\r\n"));
        }

        /* Push to talk */
        if ((g_SW1Pressed == true) && (s_asrSession == ASR_SESSION_WAKE_WORD) && (appAsrShellCommands.asrMode == ASR_MODE_PTT))
        {
            g_SW1Pressed             = false;
            g_sampleCount = 0;
            asr_set_state(ASR_SESSION_VOICE_COMMAND);

            xTaskNotify(appTaskHandle, kWakeWordDetected, eSetBits);
        }

        /* Skip Wake Word phase and go directly to Voice Command phase.
         * As language will be selected last detected language.
         * As demo will be selected currently enabled demo. */
        if (oob_demo_control.skipWW == 1)
        {
            g_sampleCount = 0;
            asr_set_state(ASR_SESSION_VOICE_COMMAND);
            oob_demo_control.skipWW = 0;
        }

        VIT_Status = VIT_Process(VITHandle, pi16Sample, &VIT_DetectionResults);

        if (VIT_Status != VIT_SUCCESS)
        {
            configPRINTF(("VIT_Process error: %d\r\n", VIT_Status));
        }
        else if ((VIT_DetectionResults == VIT_WW_DETECTED) || (VIT_DetectionResults == VIT_VC_DETECTED))
        {
            if ((VIT_DetectionResults == VIT_WW_DETECTED) && (appAsrShellCommands.asrMode != ASR_MODE_PTT))
            {
                VIT_Status = VIT_GetWakeWordFound(VITHandle, &s_WakeWord);

                if (VIT_Status != VIT_SUCCESS)
                {
                    configPRINTF(("VIT_GetWakeWordFound error: %d\r\n", VIT_Status));
                }
                else if (s_WakeWord.Id > 0)
                {
                    g_wakeWordLength = s_WakeWord.StartOffset;

                    configPRINTF(("[ASR] Wake Word: %s(%d)\r\n", (s_WakeWord.pName == PL_NULL) ? "UNDEF" : s_WakeWord.pName, s_WakeWord.Id));

                    /* VIT supports only one language at a time so it does not offer the detected
                     * language because it is the one used during initialization. */
                    oob_demo_control.language   = appAsrShellCommands.activeLanguage;
                    oob_demo_control.commandSet = appAsrShellCommands.demo;

                    if ((appAsrShellCommands.asrMode == ASR_MODE_WW_AND_CMD) || (appAsrShellCommands.asrMode == ASR_MODE_WW_AND_MUL_CMD))
                    {
                        asr_set_state(ASR_SESSION_VOICE_COMMAND);
                    }

                    // Notify App Task Wake Word Detected
                    xTaskNotify(appTaskHandle, kWakeWordDetected, eSetBits);
                }
            }
            else if (VIT_DetectionResults == VIT_VC_DETECTED)
            {
                /* Retrieve id of the Voice Command detected
                   String of the Command can also be retrieved (when WW and CMDs strings are integrated in Model) */
                VIT_Status = VIT_GetVoiceCommandFound(VITHandle, &s_VoiceCommand);
                if (VIT_Status != VIT_SUCCESS)
                {
                    configPRINTF(("VIT_GetVoiceCommandFound error: %d\r\n", VIT_Status));
                }
                else if (s_VoiceCommand.Id > 0)
                {
                    uint16_t action = get_action_from_keyword(appAsrShellCommands.activeLanguage, appAsrShellCommands.demo, s_VoiceCommand.Id - 1);

                    matterActionStruct valueToSend = {0};
                    valueToSend.location = "all";
                    if (xMatterActionsQueue != 0)
                    {
                        if ((s_VoiceCommand.Id) == 1)
                        {
                            valueToSend.action = kMatterActionOnOff;
                            valueToSend.command = "on";
                        }
                        else if ((s_VoiceCommand.Id) == 2)
                        {
                            valueToSend.action = kMatterActionOnOff;
                            valueToSend.command = "off";
                        }
                        else if ((s_VoiceCommand.Id) == 3)
                        {
                            valueToSend.action = kMatterActionBlindsControl;
                            valueToSend.command = "lift";
                            valueToSend.value = (void *)100;
                            valueToSend.location = "central";
                        }
                        else if ((s_VoiceCommand.Id) == 4)
                        {
                            valueToSend.action = kMatterActionBlindsControl;
                            valueToSend.command = "close";
                            valueToSend.value = (void *)0;
                            valueToSend.location = "central";
                        }
                        else if ((s_VoiceCommand.Id) == 5)
                        {
                            valueToSend.action = kMatterActionBrightnessChange;
                            valueToSend.command = "increase";
                            valueToSend.value = (void *)20;
                        }
                        else if ((s_VoiceCommand.Id) == 6)
                        {
                            valueToSend.action = kMatterActionBrightnessChange;
                            valueToSend.command = "decrease";
                            valueToSend.value = (void *)20;
                        }

                        xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
                    }

                    /* Filter command here ? */
                    if (APP_LAYER_FilterVitDetection(action, appAsrShellCommands.demo) == false)
                    {
                        configPRINTF(("[ASR] Command: %s(%d)\r\n", (s_VoiceCommand.pName == PL_NULL) ? "UNDEF" : s_VoiceCommand.pName, s_VoiceCommand.Id));

                        /* VIT supports only one language at a time so it does not offer the detected
                         * language because it is the one used during initialization. */
                        oob_demo_control.language   = appAsrShellCommands.activeLanguage;
                        oob_demo_control.commandSet = appAsrShellCommands.demo;
                        oob_demo_control.commandId  = (uint8_t)s_VoiceCommand.Id - 1;

                        g_sampleCount = 0;

                        if (appAsrShellCommands.asrMode == ASR_MODE_WW_AND_MUL_CMD)
                        {
                            asr_set_state(ASR_SESSION_VOICE_COMMAND);
                        }
                        else if (appAsrShellCommands.asrMode == ASR_MODE_WW_AND_CMD)
                        {
                            asr_set_state(ASR_SESSION_WAKE_WORD);
                        }

                        xTaskNotify(appTaskHandle, kVoiceCommandDetected, eSetBits);
                    }
                    else
                    {
                        configPRINTF(("[ASR] Filter VIT Command: %s(%d)\r\n", (s_VoiceCommand.pName == PL_NULL) ? "UNDEF" : s_VoiceCommand.pName, s_VoiceCommand.Id));
                    }
                }
            }
        }

        // No timeout when ASR is in cmd only mode
        if ((s_asrSession == ASR_SESSION_VOICE_COMMAND) && (appAsrShellCommands.asrMode != ASR_MODE_CMD_ONLY))
        {
            g_sampleCount += NUM_SAMPLES_AFE_OUTPUT;

            if (g_sampleCount > 16000 / 1000 * appAsrShellCommands.timeout)
            {
                g_sampleCount = 0;

                asr_set_state(ASR_SESSION_WAKE_WORD);

                // Notify App Task Timeout
                xTaskNotify(appTaskHandle, kTimeOut, eSetBits);
            }
        }

        // reinitialize the ASR engine if language set was changed
        if (appAsrShellCommands.asrCfg & (ASR_CFG_DEMO_LANGUAGE_CHANGED | ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED))
        {
            VIT_Deinit();
            VIT_Init();
            appAsrShellCommands.asrCfg &= ~(ASR_CFG_DEMO_LANGUAGE_CHANGED | ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED);
            oob_demo_control.language   = appAsrShellCommands.activeLanguage;

            xTaskNotify(appTaskHandle, kAsrModelChanged, eSetBits);
        }

        if (appAsrShellCommands.asrCfg & ASR_CFG_MODE_CHANGED)
        {
            appAsrShellCommands.asrCfg &= ~ASR_CFG_MODE_CHANGED;

            if(appAsrShellCommands.asrMode == ASR_MODE_CMD_ONLY)
            {
                asr_set_state(ASR_SESSION_VOICE_COMMAND);
            }
            else
            {
                asr_set_state(ASR_SESSION_WAKE_WORD);
            }
        }
    } // end of while
}

#endif /* ENABLE_VIT_ASR */
