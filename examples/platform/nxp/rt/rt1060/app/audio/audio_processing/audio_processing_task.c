/*
 * Copyright 2018-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdbool.h>
#include <stdint.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* NXP includes. */
#include "board.h"
#include "sln_mic_config.h"
#include "sln_afe.h"
#include "sln_amplifier.h"
#include "sln_rgb_led_driver.h"
#include "local_sounds_task.h"
#if ENABLE_USB_AUDIO_DUMP
#include "audio_dump.h"
#endif /* ENABLE_USB_AUDIO_DUMP */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Sending AFE processed chunks to ASR once at 3 * 10ms */
#define AFE_BLOCKS_TO_ACCUMULATE (3)

#if SLN_MIC_COUNT == 2
#define AFE_MEM_SIZE AFE_MEM_SIZE_2MICS
#elif SLN_MIC_COUNT == 3
#define AFE_MEM_SIZE AFE_MEM_SIZE_3MICS
#else
#error "UNSUPPORTED NUMBER OF MICROPHONES"
#endif /* SLN_MIC_COUNT */

#if ENABLE_VAD
/* After Voice Activity detected, assume Voice Activity for next VAD_FORCED_TRUE_CALLS */
/* The value below is for VAD_LOW_POWER_AFTER_SEC * 100, because we have 100 calls per second (10ms frames)  */
#define VAD_FORCED_TRUE_CALLS VAD_LOW_POWER_AFTER_SEC * 100

/* Number of consecutive frames with activity detected before getting out of detection 'sleep' */
#define VAD_ACTIVITY_FRAMES    5
#endif /* ENABLE_VAD */

/* Max number of ASR slots to be buffered.
 * One slot is 30ms large. */
#if VAD_BUFFER_DATA
#define ASR_QUEUE_SLOTS 15
#else
#define ASR_QUEUE_SLOTS 5
#endif /* VAD_BUFFER_DATA */

/*******************************************************************************
 * Variables
 ******************************************************************************/

SDK_ALIGN(static uint8_t __attribute__((section(".bss.$SRAM_DTC"))) s_afeExternalMemory[AFE_MEM_SIZE], 8);
SDK_ALIGN(static int16_t __attribute__((section(".bss.$SRAM_ITC")))
          s_outStream[PCM_SINGLE_CH_SMPL_COUNT * AFE_BLOCKS_TO_ACCUMULATE],
          8);
#if VAD_BUFFER_DATA
SDK_ALIGN(static int16_t __attribute__((section(".bss.$SRAM_ITC")))
          s_dummyQueueSlot[PCM_SINGLE_CH_SMPL_COUNT * AFE_BLOCKS_TO_ACCUMULATE],
          8);

static uint8_t s_forceVadEvent = 0;
#endif /* VAD_BUFFER_DATA */

static uint8_t s_outBlocksCnt = 0;
QueueHandle_t g_xSampleQueue  = NULL;

static pcmPingPong_t *s_micInputStream = NULL;
static int16_t *s_ampInputStream       = NULL;

volatile uint32_t g_wakeWordLength  = 0;
volatile long unsigned int g_processedFrames = 0;

#if ENABLE_VAD
static TaskHandle_t s_localVoiceTaskHandle   = NULL;
#endif /* ENABLE_VAD */

#if ENABLE_AEC
static bool s_bypassAec = false;
#endif /* ENABLE_AEC */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static sln_afe_status_t _sln_afe_init(void);
static sln_afe_status_t _sln_afe_process_audio(int16_t *micStream, int16_t *ampStream, void **cleanStream);
static sln_afe_status_t _sln_afe_trigger_found(void);
#if ENABLE_VAD
static sln_afe_status_t _sln_afe_vad(int16_t *micStream, bool *voiceActivity);
#endif /* ENABLE_VAD */

/*******************************************************************************
 * Code
 ******************************************************************************/

void audio_processing_set_mic_input_buffer(int16_t *buf)
{
    s_micInputStream = (pcmPingPong_t *)buf;
}

void audio_processing_set_amp_input_buffer(int16_t *buf)
{
    s_ampInputStream = buf;
}

#if ENABLE_VAD
void audio_processing_set_local_voice_task_handle(TaskHandle_t handle)
{
    s_localVoiceTaskHandle = handle;
}

void audio_processing_force_vad_event(void)
{
	s_forceVadEvent = 1;
}
#endif /* ENABLE_VAD */

#if ENABLE_AEC
void audio_processing_set_bypass_aec(bool value)
{
    s_bypassAec = value;
}

bool audio_processing_get_bypass_aec(void)
{
    return s_bypassAec;
}
#endif /* ENABLE_AEC */

void audio_processing_task(void *pvParameters)
{
    uint8_t pingPongIdx           = 0;
    uint8_t pingPongAmpIdx        = 0;
    uint32_t taskNotification     = 0;
    uint32_t currentEvent         = 0;

    int16_t *micStream            = NULL;
    int16_t *ampStream            = NULL;
    void *cleanStream             = NULL;

    sln_afe_status_t afeStatus    = kAfeSuccess;
    // bool voiceActivity            = false;
    bool sendPackageToAsr         = true;

#if ENABLE_VAD
    bool prevVoiceActivity        = false;

    uint32_t vadStartTicks        = 0;
    uint32_t vadEndTicks          = 0;
    float totalVadSessionsSec  = 0;
    float vadSessionSec        = 0;
#endif /* ENABLE_VAD */

    /* SLN_AFE Initialization. */
    afeStatus = _sln_afe_init();
    if (afeStatus != kAfeSuccess)
    {
        configPRINTF(("ERROR [%d]: AFE engine initialization has failed!\r\n", afeStatus));
        RGB_LED_SetColor(LED_COLOR_RED);
        vTaskDelete(NULL);
    }

    g_xSampleQueue = xQueueCreate(ASR_QUEUE_SLOTS, PCM_SINGLE_CH_SMPL_COUNT * AFE_BLOCKS_TO_ACCUMULATE * sizeof(short));
    if (g_xSampleQueue == NULL)
    {
        configPRINTF(("Could not create queue for AFE to ASR communication. Audio processing task failed!\r\n"));
        RGB_LED_SetColor(LED_COLOR_RED);
        vTaskDelete(NULL);
    }

    while (1)
    {
        /* Suspend waiting to be activated when receiving PDM mic data after Decimation. */
        xTaskNotifyWait(0U, 0xffffffffU, &taskNotification, portMAX_DELAY);

        /* Figure out if it's a PING or PONG buffer received */
        if (taskNotification & PCM_PING_EVENT)
        {
            pingPongIdx    = 0U;
            pingPongAmpIdx = 0U;
            currentEvent   = PCM_PING_EVENT;
        }

        if (taskNotification & PCM_PONG_EVENT)
        {
            pingPongIdx    = 1U;
            pingPongAmpIdx = 1U;
            currentEvent   = PCM_PONG_EVENT;
        }

        taskNotification &= ~currentEvent;

        /* Check if a wake word was detected and if it was, notify SLN_AFE about it */
        afeStatus = _sln_afe_trigger_found();
        if (afeStatus != kAfeSuccess)
        {
            configPRINTF(("ERROR [%d]: AFE trigger found failed!\r\n", afeStatus));
            RGB_LED_SetColor(LED_COLOR_RED);
        }

#if ENABLE_STREAMER && !ENABLE_AEC
        /* If AEC is disabled, temporarily bypass audio processing while streaming audio. */
        if (LOCAL_SOUNDS_isPlaying())
        {
            continue;
        }
#endif /* ENABLE_STREAMER && !ENABLE_AEC */

        micStream = (*s_micInputStream)[pingPongIdx];
        if (s_ampInputStream != NULL)
        {
            ampStream = &s_ampInputStream[pingPongAmpIdx * PCM_SINGLE_CH_SMPL_COUNT];
        }
        else
        {
            ampStream = NULL;
        }

        /* Use SLN_AFE on microphones and speaker data to obtain a clean stream. */
        afeStatus = _sln_afe_process_audio(micStream, ampStream, &cleanStream);
        if (afeStatus != kAfeSuccess)
        {
            configPRINTF(("ERROR [%d]: AFE audio process failed!\r\n", afeStatus));
            RGB_LED_SetColor(LED_COLOR_RED);
        }
        else
        {
            g_processedFrames++;
        }

#if ENABLE_USB_AUDIO_DUMP
        AUDIO_DUMP_ForwardDataOverUsb(micStream, ampStream, cleanStream);
#endif /* ENABLE_USB_AUDIO_DUMP */

#if ENABLE_VAD
        /* Use SLN_AFE on mic stream to detect Voice Activity and Gate ASR if needed. */
        afeStatus = _sln_afe_vad(cleanStream, &voiceActivity);
        if (afeStatus != kAfeSuccess)
        {
            configPRINTF(("ERROR [%d]: AFE audio VAD failed!\r\n", afeStatus));
            RGB_LED_SetColor(LED_COLOR_RED);
            voiceActivity = true;
        }

        if (prevVoiceActivity != voiceActivity)
        {
            if (voiceActivity == true)
            {
                vadEndTicks         = xTaskGetTickCount();
                vadSessionSec       = (float)(vadEndTicks - vadStartTicks) / configTICK_RATE_HZ;
                totalVadSessionsSec += vadSessionSec;
                configPRINTF(("VAD: detection enabled after %d sec, total bypassing since power on: %d sec\r\n",
                              (int)vadSessionSec, (int)totalVadSessionsSec));

                /* Revert MCU frequency back to its default value when voice activity is detected
                 * so we can resume ASR processing */
                BOARD_RevertClock();

                /* Wake up the ASR task */
                if (s_localVoiceTaskHandle)
                {
                    vTaskResume(s_localVoiceTaskHandle);
                }
            }
            else
            {
                vadStartTicks = xTaskGetTickCount();
                configPRINTF(("VAD: no activity in last %d sec, detection disabled\r\n",
                              VAD_FORCED_TRUE_CALLS / 100));

                /* Suspend the ASR task */
                if (s_localVoiceTaskHandle)
                {
                    vTaskSuspend(s_localVoiceTaskHandle);
                }

                /* Run a lower MCU frequency when no voice activity is detected, because we can bypass
                 * ASR and save power by putting the MCU at lower MHz */
                BOARD_ReduceClock();
            }

            prevVoiceActivity = voiceActivity;
        }
#else
        /* If VAD is disabled set voiceActivity flag to true */
        // voiceActivity = true;
#endif /* ENABLE_VAD */

#if ENABLE_VAD
#if VAD_BUFFER_DATA
        sendPackageToAsr = true;
#else
        sendPackageToAsr = true;
#endif /* VAD_BUFFER_DATA */
#endif /* ENABLE_VAD */

        if (sendPackageToAsr)
        {
            /* Prepare and send clean data to ASR module */
            memcpy(&s_outStream[s_outBlocksCnt * PCM_SINGLE_CH_SMPL_COUNT], cleanStream, PCM_SINGLE_CH_SMPL_COUNT * 2);
            s_outBlocksCnt++;
            if (s_outBlocksCnt == AFE_BLOCKS_TO_ACCUMULATE)
            {
                if (xQueueSendToBack(g_xSampleQueue, s_outStream, 0) == errQUEUE_FULL)
                {
#if VAD_BUFFER_DATA
                    /* If ASR queue is full, remove the head of it, then try again to add at the back */
                    if (xQueueReceive(g_xSampleQueue, s_dummyQueueSlot, 0) != pdPASS)
                    {
                        configPRINTF(("Could not receive from the queue\r\n"));
                    }

                    if (xQueueSendToBack(g_xSampleQueue, s_outStream, 0) != pdPASS)
                    {
                        configPRINTF(("Could not send to the queue\r\n"));
                    }
#else
                    configPRINTF(("Failed to send AFE processed data to the ASR queue\r\n"));
#endif /* VAD_BUFFER_DATA */
                }
                s_outBlocksCnt = 0;
            }
        }
    }
}

/*******************************************************************************
 * Static Functions
 ******************************************************************************/

static sln_afe_status_t _sln_afe_init(void)
{
    sln_afe_status_t afeStatus = kAfeSuccess;
    sln_afe_config_t afeConfig = {0};

    afeConfig.numberOfMics       = SLN_MIC_COUNT;
    afeConfig.afeMemBlock        = s_afeExternalMemory;
    afeConfig.mallocFunc         = pvPortMalloc;
    afeConfig.freeFunc           = vPortFree;
    afeConfig.afeMemBlockSize    = sizeof(s_afeExternalMemory);
#if ENABLE_DSMT_ASR
    afeConfig.postProcessedGain  = 4;
#else
    afeConfig.postProcessedGain  = 3;
#endif /* ENABLE_DSMT_ASR */
    afeConfig.wakeWordMaxLength  = WAKE_WORD_MAX_LENGTH_MS;
    afeConfig.micsPosition[0][0] = SLN_MIC1_POS_X;
    afeConfig.micsPosition[0][1] = SLN_MIC1_POS_Y;
    afeConfig.micsPosition[0][2] = 0;
    afeConfig.micsPosition[1][0] = SLN_MIC2_POS_X;
    afeConfig.micsPosition[1][1] = SLN_MIC2_POS_Y;
    afeConfig.micsPosition[1][2] = 0;
#if SLN_MIC_COUNT == 3
    afeConfig.micsPosition[2][0] = SLN_MIC3_POS_X;
    afeConfig.micsPosition[2][1] = SLN_MIC3_POS_Y;
    afeConfig.micsPosition[2][2] = 0;
#endif /* SLN_MIC_COUNT == 3 */

    afeConfig.dataInType  = kAfeTypeInt16;
    afeConfig.dataOutType = kAfeTypeInt16;

#if ENABLE_AEC
    afeConfig.aecEnabled      = 1;
    afeConfig.aecFilterLength = AEC_FILTER_LENGTH;
#else
    afeConfig.aecEnabled      = 0;
    afeConfig.aecFilterLength = 0;
#endif /* ENABLE_AEC */

    afeConfig.mcuType = kAfeMcuMIMXRT1060;

    afeStatus = SLN_AFE_Init(&afeConfig);

    return afeStatus;
}

static sln_afe_status_t _sln_afe_process_audio(int16_t *micStream, int16_t *ampStream, void **cleanStream)
{
    sln_afe_status_t afeStatus = kAfeSuccess;
    int16_t *refSignal         = NULL;

#if ENABLE_AMPLIFIER && ENABLE_AEC
    if ((SLN_AMP_GetState() == kSlnAmpIdle) || s_bypassAec)
    {
        /* Bypass AEC if there is no streaming (by sending NULL as refSignal).
         * Bypassing AEC greatly reduces CPU usage of SLN_AFE_Process_Audio function. */
        refSignal = NULL;
    }
    else
    {
        refSignal = ampStream;
    }
#endif /* ENABLE_AMPLIFIER && ENABLE_AEC */

    afeStatus = SLN_AFE_Process_Audio(micStream, refSignal, cleanStream);

    return afeStatus;
}

static sln_afe_status_t _sln_afe_trigger_found()
{
    sln_afe_status_t afeStatus          = kAfeSuccess;
    uint32_t wakeWordStartOffsetSamples = 0;
    uint32_t wakeWordStartOffsetMs      = 0;

    if (g_wakeWordLength != 0)
    {
        UBaseType_t asrMessagesWaiting = uxQueueMessagesWaiting(g_xSampleQueue);

        /* If ASR is behind AFE with more than 1 30ms frame, skip reporting
         * the trigger, as beamformer might be impacted */
        if (asrMessagesWaiting <= 1)
        {
            wakeWordStartOffsetSamples = g_wakeWordLength + (s_outBlocksCnt * PCM_SINGLE_CH_SMPL_COUNT)
                                         + asrMessagesWaiting * AFE_BLOCKS_TO_ACCUMULATE * PCM_SINGLE_CH_SMPL_COUNT;

            wakeWordStartOffsetMs      = wakeWordStartOffsetSamples / (PCM_SAMPLE_RATE_HZ / 1000);

            if (wakeWordStartOffsetMs <= WAKE_WORD_MAX_LENGTH_MS)
            {
                afeStatus        = SLN_AFE_Trigger_Found(wakeWordStartOffsetSamples);

                configPRINTF(("[AFE] Wake word trigger estimation: starting, length: %ld, %d\r\n",
                       g_processedFrames * PCM_SINGLE_CH_SMPL_COUNT - wakeWordStartOffsetSamples,
                       wakeWordStartOffsetSamples));
            }
            else
            {
                configPRINTF(("Warning: Reported wake word length (%d ms) bigger than supported max wake word length (%d ms)!\r\n",
                              wakeWordStartOffsetMs, WAKE_WORD_MAX_LENGTH_MS));

                afeStatus        = SLN_AFE_Trigger_Found(WAKE_WORD_MAX_LENGTH_MS * (PCM_SAMPLE_RATE_HZ / 1000));
            }
        }

        g_wakeWordLength = 0;
    }

    return afeStatus;
}

#if ENABLE_VAD
static sln_afe_status_t _sln_afe_vad(int16_t *micStream, bool *voiceActivity)
{
    sln_afe_status_t afeStatus = kAfeSuccess;

    static uint32_t timeout = VAD_FORCED_TRUE_CALLS;
    bool vadResult          = false;
    static bool prevVadResult      = false;
    static int  prevVadCount       = 0;

    afeStatus = SLN_AFE_Voice_Detected(micStream, &vadResult);

    if (vadResult == prevVadResult)
    {
        prevVadCount++;
    }
    else
    {
        prevVadCount = 0;
    }

    prevVadResult = vadResult;

    /* Reset ForcedTrue period if needed */
    if (((vadResult == true) && (prevVadCount >= VAD_ACTIVITY_FRAMES)) || s_forceVadEvent
#if ENABLE_STREAMER
            || LOCAL_SOUNDS_isPlaying()
#endif
            )
    {
        timeout = VAD_FORCED_TRUE_CALLS;

        if (s_forceVadEvent)
        {
             s_forceVadEvent = 0;
        }
    }

    /* After a Voice Activity was detected, enter in a "ForcedTrue" period for VAD_FORCED_TRUE_CALLS
     * and return True directly. We assume that during this period user will speak so there is no need to
     * actually check for Voice Activity. */
    if (timeout == 0)
    {
        *voiceActivity = false;
    }
    else
    {
        *voiceActivity = true;
    }

    if (timeout > 0)
    {
        timeout--;
    }

    return afeStatus;
}
#endif /* ENABLE_VAD */
