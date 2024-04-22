/*
 * Copyright 2019-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Board includes */
#include "board.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#if ENABLE_LOGGING_TASK
#include "iot_logging_task.h"
#endif /* ENABLE_LOGGING_TASK */

/* Driver includes */
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_iomuxc.h"

/* RGB LED driver header */
#include "sln_rgb_led_driver.h"
#include "sln_pwm_driver_flexio.h"

#if ENABLE_SHELL
/* Shell includes */
#include "sln_shell.h"
#endif /* ENABLE_SHELL */

/* Application headers */
#include "sln_local_voice_common.h"
// #include "switch.h"
#include "local_sounds_task.h"
#include "IndexCommands.h"
#include "app_layer.h"

/* Flash includes */
#include "sln_flash.h"
#include "sln_flash_fs.h"
#include "sln_flash_fs_ops.h"
#include "sln_flash_files.h"

/* Audio processing includes */
#include "audio_processing_task.h"
#include "pdm_to_pcm_task.h"
#include "sln_amplifier.h"
#if ENABLE_USB_AUDIO_DUMP
#include "audio_dump.h"
#endif /* ENABLE_USB_AUDIO_DUMP */

#include "sln_mic_config.h"
#if (MICS_TYPE == MICS_PDM)
#elif (MICS_TYPE == MICS_I2S)
#include "sln_i2s_mic.h"
#endif /* MICS_TYPE */

#if ENABLE_WIFI
#include "wifi_connection.h"
#endif /* ENABLE_WIFI */

#include "sln_main.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define audio_processing_task_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
TaskHandle_t appTaskHandle             = NULL;
TaskHandle_t audioProcessingTaskHandle = NULL;
TaskHandle_t micTaskHandle             = NULL;
TaskHandle_t localVoiceTaskHandle      = NULL;
#if ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER
TaskHandle_t aecAlignSoundTaskHandle   = NULL;
#endif /* ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER */
bool g_SW1Pressed                   = false;
oob_demo_control_t oob_demo_control = {0};

extern app_asr_shell_commands_t appAsrShellCommands;

/*******************************************************************************
 * Callbacks
 ******************************************************************************/
static void pre_sector_erase_callback(void)
{
    if (appAsrShellCommands.micsState != ASR_MICS_OFF)
    {
        SLN_MIC_OFF();
    }
}

static void post_sector_erase_callback(void)
{
    if (appAsrShellCommands.micsState != ASR_MICS_OFF)
    {
        SLN_MIC_ON();
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/

#if ENABLE_STREAMER
static status_t audio_play_clip(char *fileName, uint32_t volume)
{
    status_t status = kStatus_Success;

    /* Make sure that speaker is not currently playing another audio. */
    while (LOCAL_SOUNDS_isPlaying())
    {
        vTaskDelay(10);
    }

    status = LOCAL_SOUNDS_PlayAudioFile(fileName, volume);

    return status;
}

#if ENABLE_S2I_ASR
static void announce_demo_s2i(uint16_t demo, uint32_t volume)
{
    char *prompt = NULL;
    switch (demo)
    {
        case ASR_CMD_CHANGE_DEMO:
            prompt = AUDIO_PLEASE_SAY_A_DEMO_NAME;
            break;
        case ASR_S2I_HVAC:
            prompt = AUDIO_HVAC_DEMO_HVAC;
            break;
        case ASR_S2I_OVEN:
            prompt = AUDIO_OVEN_DEMO_OVEN;
            break;
        case ASR_S2I_HOME:
            prompt = AUDIO_SMART_HOME_DEMO_HOME;
            break;
    }

    audio_play_clip(prompt, volume);
}
#else
static status_t announce_demo(asr_inference_t demo, asr_language_t lang, uint32_t volume)
{
    status_t ret = kStatus_Success;

    char *prompt = get_demo_prompt(demo, lang);
    if (prompt)
    {
        ret = audio_play_clip(prompt, volume);
    }

    return ret;
}
#endif /* ENABLE_S2I_ASR */
#endif /* ENABLE_STREAMER */

QueueHandle_t xBlindsQueue;

void vBlindsTask(void *pvParameters)
{
	uint8_t percentage;

    while(1)
    {
    	if (xBlindsQueue != NULL)
    	{
    		if (xQueueReceive(xBlindsQueue, &percentage, portMAX_DELAY) == pdPASS)
    		{
    			PWM_DriveBlinds(percentage);
    		}
    	}
    }
}

void appTask(void *arg)
{
    uint32_t statusFlash     = 0;
    mic_task_config_t config = {0};
    int32_t fileSystemSize   = 0;

    fileSystemSize = sln_flash_fs_ops_getFileSystemSize();
    configPRINTF(("LittleFS usage: %dKB out of %dKB\r\n", fileSystemSize/1024, FICA_FILE_SYS_SIZE/1024));

#if ENABLE_AMPLIFIER
#if ENABLE_STREAMER
    /* InitStreamer will initialize the amplifier as well */
    if (kStatus_Success != LOCAL_SOUNDS_InitStreamer())
    {
        configPRINTF(("LOCAL_SOUNDS_InitStreamer failed!\r\n"));
    }
#else
    /* No external buffers count when streamer is not used */
    SLN_AMP_Init(NULL);
#endif /* ENABLE_STREAMER */
#endif /* ENABLE_AMPLIFIER */

    int16_t *micBuf = SLN_MIC_GET_PCM_BUFFER_POINTER();
    audio_processing_set_mic_input_buffer(micBuf);

    int16_t *ampBuf = SLN_MIC_GET_AMP_BUFFER_POINTER();
    audio_processing_set_amp_input_buffer(ampBuf);


    /* Create audio processing task */
    if (xTaskCreate(audio_processing_task, "Audio_Proc_Task", 768, NULL, audio_processing_task_PRIORITY,
                    &audioProcessingTaskHandle) != pdPASS)
    {
        configPRINTF(("Audio processing task creation failed!\r\n"));
        RGB_LED_SetColor(LED_COLOR_RED);
        vTaskDelete(NULL);
    }

    /* Configure Microphones */
    config.thisTask       = &micTaskHandle;
    config.processingTask = &audioProcessingTaskHandle;
#if ENABLE_AEC
    config.feedbackEnable  = SLN_AMP_LoopbackEnable;
    config.feedbackDisable = SLN_AMP_LoopbackDisable;
#if USE_MQS
    config.loopbackRingBuffer = SLN_AMP_GetRingBuffer();
    config.loopbackMutex      = SLN_AMP_GetLoopBackMutex();
    config.updateTimestamp    = SLN_AMP_UpdateTimestamp;
    config.getTimestamp       = SLN_AMP_GetTimestamp;
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

    SLN_MIC_SET_TASK_CONFIG(&config);

    /* Create microphones data acquisition task */
    if (xTaskCreate(SLN_MIC_TASK_FUNCTION, SLN_MIC_TASK_NAME, SLN_MIC_TASK_STACK_SIZE, NULL, SLN_MIC_TASK_PRIORITY, &micTaskHandle) !=
        pdPASS)
    {
        configPRINTF(("PDM to PCM processing task creation failed!\r\n"));
        RGB_LED_SetColor(LED_COLOR_RED);
        vTaskDelete(NULL);
    }

#if ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER
    if (xTaskCreate(AUDIO_DUMP_AecAlignSoundTask, aec_align_sound_task_NAME, aec_align_sound_task_STACK, NULL, aec_align_sound_task_PRIORITY,
                        &aecAlignSoundTaskHandle) != pdPASS)
    {
        configPRINTF(("xTaskCreate AUDIO_DUMP_AecAlignSoundTask failed!\r\n"));
    }
#endif /* ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER */

    while (!SLN_MIC_GET_STATE() || (appAsrShellCommands.status != WRITE_SUCCESS))
    {
        vTaskDelay(1);
    }

    RGB_LED_SetColor(LED_COLOR_OFF);

#if ENABLE_AMPLIFIER
    SLN_AMP_SetVolume(appAsrShellCommands.volume);
#if ENABLE_STREAMER
#if ENABLE_S2I_ASR
    announce_demo_s2i(appAsrShellCommands.demo, appAsrShellCommands.volume);
#else
    announce_demo(appAsrShellCommands.demo, appAsrShellCommands.activeLanguage, appAsrShellCommands.volume);
#endif /* ENABLE_S2I_ASR */
#endif /* ENABLE_STREAMER */
#endif /* ENABLE_AMPLIFIER */

    APP_LAYER_HandleFirstBoardBoot();

    if (appAsrShellCommands.micsState == ASR_MICS_OFF)
    {
        /* close mics and amp feedback loop */
        SLN_MIC_OFF();
        /* show the user that device does not respond */
        RGB_LED_SetColor(LED_COLOR_ORANGE);
    }
    else if (appAsrShellCommands.asrMode == ASR_MODE_PTT)
    {
        configPRINTF(("ASR Push-To-Talk mode is enabled.\r\n"
                      "Press SW3 to input a command.\r\n"));
        /* show the user that device will wake up only on SW3 press */
        RGB_LED_SetColor(LED_COLOR_CYAN);
    }
    else if (appAsrShellCommands.asrMode == ASR_MODE_CMD_ONLY)
    {
        /* show the user that no WW is needed to wake up the board */
        RGB_LED_SetColor(LED_COLOR_BLUE);
    }

#if ENABLE_WIFI
    if (kStatus_Success != wifi_connect())
    {
        configPRINTF(("WiFi connection failed!\r\n"));
    }
#endif /* ENABLE_WIFI */

    uint32_t taskNotification = 0;
    while (1)
    {
        xTaskNotifyWait(0xffffffffU, 0xffffffffU, &taskNotification, portMAX_DELAY);

        if (taskNotification & kMicUpdate)
        {
            if (appAsrShellCommands.micsState == ASR_MICS_OFF)
            {
                /* close mics and amp feedback loop */
                SLN_MIC_OFF();
                /* show the user that device does not respond */
                RGB_LED_SetColor(LED_COLOR_ORANGE);
            }
            else
            {
                /* reopen mics and amp feedback loop */
                SLN_MIC_ON();
                /* turn off the orange led */
                RGB_LED_SetColor(LED_COLOR_OFF);
            }
        }

        if (taskNotification & kVolumeUpdate)
        {
#if ENABLE_AMPLIFIER
            SLN_AMP_SetVolume(appAsrShellCommands.volume);
#endif /* ENABLE_AMPLIFIER */
        }

        if (taskNotification & kWakeWordDetected)
        {
            APP_LAYER_ProcessWakeWord(&oob_demo_control);
        }

        if (taskNotification & kVoiceCommandDetected)
        {
#if ENABLE_S2I_ASR
            APP_LAYER_ProcessIntent();
#else
            APP_LAYER_ProcessVoiceCommand(&oob_demo_control);
#endif /* ENABLE_S2I_ASR */
        }

       if (taskNotification & kTimeOut)
        {
            APP_LAYER_ProcessTimeout(&oob_demo_control);
        }

        if (taskNotification & kAsrModelChanged)
        {
#if ENABLE_STREAMER
#if ENABLE_S2I_ASR
            announce_demo_s2i(appAsrShellCommands.demo, appAsrShellCommands.volume);
#else
            announce_demo(appAsrShellCommands.demo, oob_demo_control.language, appAsrShellCommands.volume);
#endif /* ENABLE_S2I_ASR */
#endif /* ENABLE_STREAMER */

            if ((appAsrShellCommands.asrMode != ASR_MODE_PTT) &&
                (appAsrShellCommands.micsState != ASR_MICS_OFF))
            {
                RGB_LED_SetColor(LED_COLOR_OFF);
            }

            if (appAsrShellCommands.asrMode == ASR_MODE_CMD_ONLY)
            {
                RGB_LED_SetColor(LED_COLOR_BLUE);
            }
        }

        if (taskNotification & kAsrModeChanged)
        {
            if (appAsrShellCommands.micsState != ASR_MICS_OFF)
            {
                if (appAsrShellCommands.asrMode == ASR_MODE_CMD_ONLY)
                {
                    RGB_LED_SetColor(LED_COLOR_BLUE);
                }
                else
                {
                    RGB_LED_SetColor(LED_COLOR_OFF);
                }
            }
        }

        if (appAsrShellCommands.asrMode == ASR_MODE_PTT)
        {
            /* show the user that device will wake up only on SW3 press */
            RGB_LED_SetColor(LED_COLOR_CYAN);
        }

        /* check the status of shell commands and flash file system */
        if (appAsrShellCommands.status == WRITE_READY)
        {
            appAsrShellCommands.status = WRITE_SUCCESS;

            /* Check if appAsrShellCommands structure is the same as in flash */
            app_asr_shell_commands_t appAsrShellCommandsMem = {};
            uint32_t len          = 0;
            statusFlash = sln_flash_fs_ops_read(ASR_SHELL_COMMANDS_FILE_NAME, NULL, 0, &len);
            if (statusFlash == SLN_FLASH_FS_OK)
            {
                statusFlash = sln_flash_fs_ops_read(ASR_SHELL_COMMANDS_FILE_NAME, (uint8_t *)&appAsrShellCommandsMem, 0, &len);
            }

            if (statusFlash != SLN_FLASH_FS_OK)
            {
                configPRINTF(("Failed reading local demo configuration from flash memory.\r\n"));
            }

            if (memcmp(&appAsrShellCommands, &appAsrShellCommandsMem, sizeof(app_asr_shell_commands_t)) != 0)
            {
                statusFlash = sln_flash_fs_ops_save(ASR_SHELL_COMMANDS_FILE_NAME, (uint8_t *)&appAsrShellCommands,
                                                  sizeof(app_asr_shell_commands_t));
                if (statusFlash != SLN_FLASH_FS_OK)
                {
                    configPRINTF(("Failed to write local demo configuration in flash memory.\r\n"));
                }
                else
                {
                    configPRINTF(("Updated local demo configuration in flash memory.\r\n"));
                }
            }
        }

        taskNotification = 0;
    }
}

/*!
 * @brief Main function
 */
void sln_main(void)
{
    SLN_Flash_Init();

    /*
     * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
     *                              = 24 * (32 + 77/100)
     *                              = 786.48 MHz
     */
    const clock_audio_pll_config_t audioPllConfig = {
        .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
        .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
        .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
        .denominator = 100, /* 30 bit denominator of fractional loop divider */
    };

    CLOCK_InitAudioPll(&audioPllConfig);

    CLOCK_SetMux(kCLOCK_Sai1Mux, BOARD_PDM_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, BOARD_PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, BOARD_PDM_SAI_CLOCK_SOURCE_DIVIDER);
    CLOCK_EnableClock(kCLOCK_Sai1);

    CLOCK_SetMux(kCLOCK_Sai2Mux, BOARD_PDM_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai2PreDiv, BOARD_PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai2Div, BOARD_PDM_SAI_CLOCK_SOURCE_DIVIDER);
    CLOCK_EnableClock(kCLOCK_Sai2);

    RGB_LED_Init();
    RGB_LED_SetColor(LED_COLOR_GREEN);

    FlexPWM_Init();

#if ENABLE_SHELL
    sln_shell_init();
#elif ENABLE_UART_CONSOLE
    BOARD_InitDebugConsole();
#endif /* ENABLE_SHELL */

    /* Init littlefs and set pre and post sector erase callbacks */
    sln_flash_fs_status_t statusFlash = sln_flash_fs_ops_init(false);
    if (SLN_FLASH_FS_OK != statusFlash)
    {
        configPRINTF(("littlefs init failed!\r\n"));
    }

    sln_flash_fs_cbs_t flash_mgmt_cbs = {NULL, NULL, pre_sector_erase_callback, post_sector_erase_callback};

    // statusFlash = sln_flash_fs_ops_setcbs(&flash_mgmt_cbs);
    if (SLN_FLASH_FS_OK != statusFlash)
    {
        configPRINTF(("littlefs callbacks setting failed!\r\n"));
    }

    xBlindsQueue = xQueueCreate(10, sizeof(uint8_t));
    xTaskCreate(vBlindsTask, "Blinds Task", 256, NULL, 2, NULL);

    xTaskCreate(appTask, "APP_Task", 512, NULL, configMAX_PRIORITIES - 4, &appTaskHandle);
    xTaskCreate(local_voice_task, "ASR_Task", 1280, NULL, configMAX_PRIORITIES - 4, &localVoiceTaskHandle);
#if ENABLE_SHELL
    xTaskCreate(sln_shell_task, "Shell_Task", 512, NULL, tskIDLE_PRIORITY, NULL);
#endif /* ENABLE_SHELL */

#if ENABLE_LOGGING_TASK
    xLoggingTaskInitialize(384, configMAX_PRIORITIES - 5, 32);
#endif /* ENABLE_LOGGING_TASK */
}
