/*
 * Copyright 2019-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_AMPLIFIER

#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "fsl_common.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "queue.h"
#include "sln_mic_config.h"
#include "sln_amplifier.h"
#include "sln_amplifier_processing.h"

#if ENABLE_AEC
#if USE_MQS
#include "fsl_gpt.h"
#include "semphr.h"
#include "ringbuffer.h"
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief PLAY AUDIO ASYNC Task settings */
#define PLAY_AUDIO_ASYNC_TASK_NAME       "Play_Audio_Async_Task"
#define PLAY_AUDIO_ASYNC_TASK_STACK_SIZE 256
#define PLAY_AUDIO_ASYNC_TASK_PRIORITY   configTIMER_TASK_PRIORITY - 1

#define WAIT_SAI_FEF_FLAG_CLEAR 3

/* Maximum DMA chunk size: 512KB (both for TFA and MQS) */
#define PCM_AMP_DMA_CHUNK_MAX_SIZE 0x80000

#if ENABLE_AEC
#if USE_MQS
/* For 24Mhz and PS=200, 1 tick ~= 8.4us.
 * In this configuration, AMP_LOOPBACK_GPT will overflow after around 10 hours */
#define AMP_LOOPBACK_GPT                    GPT2
#define AMP_LOOPBACK_GPT_FREQ_MHZ           24
#define AMP_LOOPBACK_GPT_PS                 200

/* Convert Loopback GPT ticks to microseconds */
#define AMP_LOOPBACK_GPT_TICKS_TO_US(ticks) ((ticks * AMP_LOOPBACK_GPT_PS) / AMP_LOOPBACK_GPT_FREQ_MHZ)

#define AMP_LOOPBACK_NEW_SYNC_MAX_WAIT_MS 100

/* During loopback enable, delay the barge-in start in order to give time
 * to the pdm_to_pcm_task to restart its activities. */
#define AMP_LOOPBACK_START_DELAY_MS       15

typedef enum _loopback_state
{
    kLoopbackDisabled,
    kLoopbackNeedSync,
    kLoopbackEnabled,
} loopback_state_t;
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/* Structure provided to the async Play_Audio_Async_Task. */
typedef struct _sln_amp_write_packet
{
    uint8_t *data;
    uint32_t length;
    uint32_t slotSize;
    uint8_t slotCnt;
    bool inFlash;
} sln_amp_write_packet_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

static volatile sln_amp_state_t s_SlnAmpState = kSlnAmpIdle;

/* Used for synchronization with the Streamer */
static volatile uint8_t *s_StreamerFreeBuffs = NULL;
static volatile uint8_t s_StreamerMaxBuffs   = 0;

/* Used by SLN_AMP_WriteAudioBlocking and SLN_AMP_WriteAudioNoWait to know
 * how many buffers are free. */
static volatile uint8_t s_AmplifierFreeBuffs = 0;
static volatile uint8_t s_AmplifierMaxBuffs  = 0;

/* Used by SLN_AMP_WriteAudioNoWait to communicate with the async task.
 * Async task is created only once during the first call of SLN_AMP_WriteAudioNoWait. */
static TaskHandle_t s_AudioNoWaitTaskHandle = NULL;
static QueueHandle_t s_AudioNoWaitTaskQueue = NULL;

/* Used to manage the audio played by SLN_AMP_WriteAudioBlocking and SLN_AMP_WriteAudioNoWait. */
static volatile bool s_AbortPlaying = false;

static codec_handle_t s_CodecHandle = {0};

/* The DMA Handle for audio amplifier SAI3 */
static edma_handle_t s_AmpDmaTxHandle  = {0};
static sai_edma_handle_t s_AmpTxHandle = {0};

#if ENABLE_AEC
#if USE_MQS
static volatile loopback_state_t s_LoopbackState = kLoopbackEnabled;
static ringbuf_t *s_AmpRxDataRingBuffer          = NULL;
static volatile uint32_t s_PdmPcmTimestamp       = -1;
static SemaphoreHandle_t s_LoopBackMutex         = NULL;
static SemaphoreHandle_t s_LoopBackStateMutex    = NULL;
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static status_t SLN_AMP_TransferChunk(uint8_t *data, uint32_t length);
static status_t SLN_AMP_WriteAudioPipeline(
    uint8_t *data, uint32_t length, uint32_t slotSize, uint8_t slotCnt, bool inFlash);
static void SLN_AMP_PlayAudioAsyncTask(void *pvParameters);
static void SLN_AMP_TxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
#if USE_MQS
static void SLN_AMP_VolAndDiffInputControl(void *data, uint32_t length);
#endif /* USE_MQS */

#if ENABLE_AEC
#if USE_MQS
static void SLN_AMP_StartLoopbackTimer(void);
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/*******************************************************************************
 * Code
 ******************************************************************************/

void SAI_UserTxIRQHandler(void)
{
    uint8_t i;

    SAI_TxClearStatusFlags(BOARD_AMP_SAI, kSAI_FIFOErrorFlag);
    /* Give time FEF flag to be cleared */
    for (i = 0; i < WAIT_SAI_FEF_FLAG_CLEAR; i++)
    {
        if ((BOARD_AMP_SAI->TCSR & kSAI_FIFOErrorFlag) == 0)
        {
            break;
        }
    }
}

void SAI_UserRxIRQHandler(void)
{
    uint8_t i;

    /* Loopback mechanism is disabled. Just clear the FEF flag */
    SAI_RxClearStatusFlags(BOARD_AMP_SAI, kSAI_FIFOErrorFlag);

    /* Give time FEF flag to be cleared */
    for (i = 0; i < WAIT_SAI_FEF_FLAG_CLEAR; i++)
    {
        if ((BOARD_AMP_SAI->RCSR & kSAI_FIFOErrorFlag) == 0)
        {
            break;
        }
    }
}

status_t SLN_AMP_Init(volatile uint8_t *extStreamerBuffsCnt)
{
    status_t ret = kStatus_Success;

    sai_init_handle_t saiInitHandle = {
        .amp_dma_tx_handle = &s_AmpDmaTxHandle,
        .amp_sai_tx_handle = &s_AmpTxHandle,
        .sai_tx_callback   = SLN_AMP_TxCallback,
    };

    if (ret == kStatus_Success)
    {
        s_StreamerFreeBuffs = extStreamerBuffsCnt;
        if (NULL != s_StreamerFreeBuffs)
        {
            s_StreamerMaxBuffs  = *extStreamerBuffsCnt;
        }

        /* BOARD_SAI_Init triggers a Transfer, wait for its end. */
        s_AmplifierFreeBuffs = 0;
        s_SlnAmpState        = kSlnAmpPlayBlocking;

        BOARD_SAI_Init(saiInitHandle);

        while (s_AmplifierFreeBuffs == 0)
        {
            vTaskDelay(1);
        }
        s_AmplifierFreeBuffs = 0;
        s_SlnAmpState        = kSlnAmpIdle;

        ret = CODEC_Init(&s_CodecHandle, (codec_config_t *)BOARD_GetBoardCodecConfig());
    }
#if ENABLE_AEC
#if USE_MQS
    if (ret == kStatus_Success)
    {
        s_AmpRxDataRingBuffer =
            (ringbuf_t *)(((mqs_config_t *)(s_CodecHandle.codecConfig->codecDevConfig))->s_AmpRxDataRingBuffer);
        if (s_AmpRxDataRingBuffer == NULL)
        {
            configPRINTF(("Failed to create s_AmpRxDataRingBuffer\r\n"));
            ret = kStatus_Fail;
        }
    }

    if (ret == kStatus_Success)
    {
        s_LoopBackMutex = xSemaphoreCreateMutex();
        if (s_LoopBackMutex == NULL)
        {
            configPRINTF(("Failed to create s_LoopBackMutex\r\n"));
            ret = kStatus_Fail;
        }
    }

    if (ret == kStatus_Success)
    {
        s_LoopBackStateMutex = xSemaphoreCreateMutex();
        if (s_LoopBackMutex == NULL)
        {
            configPRINTF(("Failed to create s_LoopBackStateMutex\r\n"));
            ret = kStatus_Fail;
        }
    }

    if (ret == kStatus_Success)
    {
        SLN_AMP_StartLoopbackTimer();
    }
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

    return ret;
}

status_t SLN_AMP_WriteAudioBlocking(uint8_t *data, uint32_t length, uint32_t slotSize, uint8_t slotCnt, bool inFlash)
{
    status_t ret = kStatus_Success;

    if ((data == NULL) || (length == 0) || (slotSize == 0) || (slotCnt == 0))
    {
        ret = kStatus_Fail;
    }

#if USE_MQS
#if ENABLE_AEC
    /* In order loopback to work on MQS, it is required that
     * loopback ringbuffer has enough space to store scheduled to play audio data. */
    if (ret == kStatus_Success)
    {
        if ((slotSize * slotCnt) > (AMP_WRITE_SLOTS * PCM_AMP_DATA_SIZE_20_MS))
        {
            ret = kStatus_Fail;
        }
    }
#endif /* ENABLE_AEC */
#endif /* USE_MQS */

    if (ret == kStatus_Success)
    {
        /* If another audio is playing by SLN_AMP_WriteAudioNoWait or SLN_AMP_WriteStreamerNoWait,
         * do not play the current chunk. */
        if (s_SlnAmpState == kSlnAmpIdle)
        {
            s_SlnAmpState = kSlnAmpPlayBlocking;
        }
        else
        {
            ret = kStatus_Fail;
        }
    }

    if (ret == kStatus_Success)
    {
        ret = SLN_AMP_WriteAudioPipeline(data, length, slotSize, slotCnt, inFlash);

        s_SlnAmpState = kSlnAmpIdle;
    }

    return ret;
}

status_t SLN_AMP_WriteAudioNoWait(uint8_t *data, uint32_t length, uint32_t slotSize, uint8_t slotCnt, bool inFlash)
{
    status_t ret                   = kStatus_Success;
    sln_amp_write_packet_t *packet = NULL;

    if ((data == NULL) || (length == 0) || (slotSize == 0) || (slotCnt == 0))
    {
        ret = kStatus_Fail;
    }

#if USE_MQS
#if ENABLE_AEC
    /* In order loopback to work on MQS, it is required that
     * loopback ringbuffer has enough space to store scheduled to play audio data. */
    if (ret == kStatus_Success)
    {
        if ((slotSize * slotCnt) > (AMP_WRITE_SLOTS * PCM_AMP_DATA_SIZE_20_MS))
        {
            ret = kStatus_Fail;
        }
    }
#endif /* ENABLE_AEC */
#endif /* USE_MQS */

    if (ret == kStatus_Success)
    {
        /* Async task queue is created only once during first call of SLN_AMP_WriteAudioNoWait. */
        if (s_AudioNoWaitTaskQueue == NULL)
        {
            s_AudioNoWaitTaskQueue = xQueueCreate(1, sizeof(packet));
            if (s_AudioNoWaitTaskQueue == NULL)
            {
                configPRINTF(("Failed to create s_AudioNoWaitTaskQueue\r\n"));

                ret = kStatus_Fail;
            }
        }
    }

    if (ret == kStatus_Success)
    {
        /* Async task is created only once during the first call of SLN_AMP_WriteAudioNoWait. */
        if (s_AudioNoWaitTaskHandle == NULL)
        {
            if (xTaskCreate(SLN_AMP_PlayAudioAsyncTask, PLAY_AUDIO_ASYNC_TASK_NAME, PLAY_AUDIO_ASYNC_TASK_STACK_SIZE,
                            NULL, PLAY_AUDIO_ASYNC_TASK_PRIORITY, &s_AudioNoWaitTaskHandle) != pdPASS)
            {
                s_AudioNoWaitTaskHandle = NULL;
                configPRINTF(("Failed to create s_AudioNoWaitTaskHandle\r\n"));

                ret = kStatus_Fail;
            }
        }
    }

    if (ret == kStatus_Success)
    {
        /* If another audio is playing by SLN_AMP_WriteAudioBlocking or SLN_AMP_WriteStreamerNoWait,
         * do not play the current chunk. */
        if (s_SlnAmpState == kSlnAmpIdle)
        {
            s_SlnAmpState = kSlnAmpPlayNonBlocking;
        }
        else
        {
            ret = kStatus_Fail;
        }
    }

    if (ret == kStatus_Success)
    {
        /* If PLAY_AUDIO_ASYNC_TASK_NAME will be created successfully, it will be responsible to free
         * the memory allocated for packet (inside SLN_AMP_PlayAudioAsyncTask function).
         * Otherwise, packet memory is freed at the end of the current function. */
        packet = pvPortMalloc(sizeof(sln_amp_write_packet_t));
        if (packet == NULL)
        {
            s_SlnAmpState = kSlnAmpIdle;
            ret           = kStatus_Fail;
        }
    }

    if (ret == kStatus_Success)
    {
        packet->data     = data;
        packet->length   = length;
        packet->slotSize = slotSize;
        packet->slotCnt  = slotCnt;
        packet->inFlash  = inFlash;

        if (xQueueSendToBack(s_AudioNoWaitTaskQueue, &packet, 0) != pdTRUE)
        {
            vPortFree(packet);
            s_SlnAmpState = kSlnAmpIdle;
            ret           = kStatus_Fail;
        }
    }

    return ret;
}

status_t SLN_AMP_WriteStreamerNoWait(uint8_t *data, uint32_t length)
{
    status_t ret = kStatus_Success;

    if ((data == NULL) || (length == 0))
    {
        ret = kStatus_Fail;
    }

    if (ret == kStatus_Success)
    {
        /* If another audio is playing by SLN_AMP_WriteAudioBlocking or SLN_AMP_WriteAudioNoWait,
         * do not play the current chunk. */
        if ((s_SlnAmpState == kSlnAmpIdle) || (s_SlnAmpState == kSlnAmpStreamer))
        {
            s_SlnAmpState = kSlnAmpStreamer;
        }
        else
        {
            ret = kStatus_Fail;
        }
    }

    /* Play ONLY the first chunk (max PCM_AMP_DMA_CHUNK_MAX_SIZE bytes) of the provided data. */
    if (ret == kStatus_Success)
    {
        ret = SLN_AMP_TransferChunk(data, length);
    }

    return ret;
}

void SLN_AMP_AbortWrite(void)
{
    SAI_TransferTerminateSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandle);

    if (s_AbortPlaying == false)
    {
        if ((s_SlnAmpState == kSlnAmpPlayBlocking) || (s_SlnAmpState == kSlnAmpPlayNonBlocking))
        {
            s_AbortPlaying = true;

            while (1)
            {
                if (s_SlnAmpState == kSlnAmpIdle)
                {
                    break;
                }
                vTaskDelay(1);
            }

            s_AbortPlaying = false;
        }
        else
        {
            s_SlnAmpState = kSlnAmpIdle;
        }
    }
}

void SLN_AMP_SetVolume(uint8_t volume)
{
    /* Set Volume between 0(min) and 100 (max) */
    CODEC_SetVolume(&s_CodecHandle, kCODEC_PlayChannelLeft0 | kCODEC_PlayChannelRight0, (uint32_t)volume);
}

void *SLN_AMP_GetAmpTxHandler(void)
{
    return &s_AmpTxHandle;
}

sln_amp_state_t SLN_AMP_GetState(void)
{
    return s_SlnAmpState;
}

/*******************************************************************************
 * Loopback API
 ******************************************************************************/

#if ENABLE_AEC
#if USE_MQS
SemaphoreHandle_t SLN_AMP_GetLoopBackMutex(void)
{
    return s_LoopBackMutex;
}

ringbuf_t *SLN_AMP_GetRingBuffer(void)
{
    return s_AmpRxDataRingBuffer;
}

uint32_t SLN_AMP_GetTimestamp(void)
{
    return GPT_GetCurrentTimerCount(AMP_LOOPBACK_GPT);
}

void SLN_AMP_UpdateTimestamp(uint32_t timestamp)
{
    s_PdmPcmTimestamp = timestamp;
}
#endif /* USE_MQS */

void SLN_AMP_LoopbackEnable(void)
{
#if USE_MQS
    xSemaphoreTake(s_LoopBackStateMutex, portMAX_DELAY);
    s_LoopbackState = kLoopbackNeedSync;
    xSemaphoreGive(s_LoopBackStateMutex);
#endif /* USE_MQS */
}

void SLN_AMP_LoopbackDisable(void)
{
#if USE_MQS
    xSemaphoreTake(s_LoopBackStateMutex, portMAX_DELAY);

    /* Clear the loopback ringbuffer for a future clean start */
    xSemaphoreTake(s_LoopBackMutex, portMAX_DELAY);
    ringbuf_clear(s_AmpRxDataRingBuffer);
    xSemaphoreGive(s_LoopBackMutex);

    s_LoopbackState = kLoopbackDisabled;
    xSemaphoreGive(s_LoopBackStateMutex);
#endif /* USE_MQS */
}
#endif /* ENABLE_AEC */

/*******************************************************************************
 * Static Functions
 ******************************************************************************/

/**
 * @brief  Extract and start to play the first chunk of the provided audio.
 *         For MQS, also do:
 *         - Update the audio chunk according to the current volume.
 *         - Sync (if needed) chunk start play with PDM_to_PCM for a better loopback performance.
 *         - Keep loopback synced with PDM_to_PCM for a better loopback performance.
 *         - Write audio chunk into the loopback ringbuffer in order to be used for barge-in.
 *
 * @param  data Pointer to the audio data.
 * @param  length Length of the audio data.
 *
 * @return kStatus_Success if the audio was started to play.
 */
static status_t SLN_AMP_TransferChunk(uint8_t *data, uint32_t length)
{
    status_t status           = kStatus_Success;
    sai_transfer_t write_xfer = {0};

    if ((data == NULL) || (length == 0))
    {
        status = kStatus_Fail;
    }

    /* Prepare the first chunk (max PCM_AMP_DMA_CHUNK_MAX_SIZE bytes) of the provided data. */
    if (status == kStatus_Success)
    {
        write_xfer.data = data;

        /* Audio data should be less than PCM_AMP_DMA_CHUNK_MAX_SIZE and rounded to 32 */
        if (length > PCM_AMP_DMA_CHUNK_MAX_SIZE)
        {
            write_xfer.dataSize = PCM_AMP_DMA_CHUNK_MAX_SIZE;
        }
        else
        {
            write_xfer.dataSize = length;
        }
        write_xfer.dataSize -= (write_xfer.dataSize % 32);

        /* After write_xfer.dataSize is rounded to 32, it may become 0.
         * In this case, don't play it, but act as this audio chunk was already played,
         * call the callback to increase the slots number and return success. */
        if (write_xfer.dataSize == 0)
        {
            SLN_AMP_TxCallback(NULL, NULL, kStatus_Success, NULL);
        }
    }

    /* Play prepared audio chunk.
     * After write_xfer.dataSize is rounded to 32, it may become 0.
     * In this case, don't play any sound, but still return success. */
    if ((status == kStatus_Success) && (write_xfer.dataSize > 0))
    {
#if USE_MQS
        SLN_AMP_VolAndDiffInputControl(write_xfer.data, write_xfer.dataSize);

        /* Apply low pass filter for better quality of audio
         * The output will be in differential format */
        SLN_AMP_FirLowPassFilterForAMP((int16_t *)write_xfer.data, write_xfer.dataSize / 2);

#if !ENABLE_AEC
        status = SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandle, &write_xfer);

#else
        uint16_t i               = 0;
        uint8_t dummyZero        = 0;
        uint32_t ringbufOcc      = 0;
        uint32_t slnAmpTimestamp = 0;
        uint32_t delayTicks      = 0;
        uint32_t delayUs         = 0;
        uint32_t delayBytes      = 0;

        if ((s_LoopBackStateMutex == NULL) || (s_LoopBackMutex == NULL) || (s_AmpRxDataRingBuffer == NULL) ||
            (s_PdmPcmTimestamp == -1))
        {
            /* Loopback is not ready, just send the sound chunk to dma */
            status = SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandle, &write_xfer);
            return status;
        }

        /*
         * In case of NOT being already synchronized, calculate the delay between the last Ping/Pong event and the
         * current call. Add this delay as zeroes to the ringbuffer. After synchronization (if was needed), start the
         * playback and place the playback data into the ringbuffer.
         */
        xSemaphoreTake(s_LoopBackStateMutex, portMAX_DELAY);
        if (s_LoopbackState == kLoopbackNeedSync)
        {
            /* Give time to pdm_to_pcm_task to restart its activities */
            vTaskDelay(AMP_LOOPBACK_START_DELAY_MS);

            if ((s_SlnAmpState == kSlnAmpPlayBlocking) || (s_SlnAmpState == kSlnAmpPlayNonBlocking))
            {
                /* Wait for all the registered AMP packets to be played.
                 * This is needed for a new synchronization to be performed after a loopback disable-enable. */
                for (i = 0; i < AMP_LOOPBACK_NEW_SYNC_MAX_WAIT_MS; i++)
                {
                    if (s_AmplifierFreeBuffs == s_AmplifierMaxBuffs)
                    {
                        break;
                    }
                    vTaskDelay(1);
                }
            }
            else
            {
                /* Wait for all the registered AMP packets to be played.
                 * This is needed for a new synchronization to be performed after a loopback disable-enable. */
                for (i = 0; i < AMP_LOOPBACK_NEW_SYNC_MAX_WAIT_MS; i++)
                {
                    if ((s_StreamerFreeBuffs != NULL) && ((*s_StreamerFreeBuffs) == s_StreamerMaxBuffs))
                    {
                        break;
                    }
                    vTaskDelay(1);
                }
            }

            s_LoopbackState = kLoopbackEnabled;
        }

        if (s_LoopbackState == kLoopbackEnabled)
        {
            xSemaphoreTake(s_LoopBackMutex, portMAX_DELAY);

            status = SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandle, &write_xfer);
            if (status == kStatus_Success)
            {
                /* Check if the current packet is the first one of a playback session.
                 * If it is the first packet, add delay data to the ringbuffer. */
                ringbufOcc = ringbuf_get_occupancy(s_AmpRxDataRingBuffer);
                if (ringbufOcc == 0)
                {
                    slnAmpTimestamp = GPT_GetCurrentTimerCount(AMP_LOOPBACK_GPT);

                    /* Add the delay data in order to sync the microphones with the amplifier. */
                    if (slnAmpTimestamp > s_PdmPcmTimestamp)
                    {
                        delayTicks = slnAmpTimestamp - s_PdmPcmTimestamp;
                    }
                    else
                    {
                        delayTicks = (UINT32_MAX - s_PdmPcmTimestamp) + slnAmpTimestamp;
                    }

                    delayUs = AMP_LOOPBACK_GPT_TICKS_TO_US(delayTicks) + AMP_LOOPBACK_CONST_DELAY_US;

                    /* Delay in bytes should be multiple of 4. Number 4 is selected because it is needed
                     * to keep samples grouped by 2(positive and negative) and one sample is an int16 (on 2 bytes). */
                    delayBytes = (delayUs * PCM_AMP_DATA_SIZE_1_MS) / 1000;
                    delayBytes = delayBytes - (delayBytes % 4);

                    if (delayBytes > AMP_LOOPBACK_MAX_DELAY_BYTES)
                    {
                        /* Should not happen, but better safe */
                        configPRINTF(
                            ("WARNING: loopback desync of %d packets\r\n",
                             delayBytes - (AMP_LOOPBACK_MAX_DELAY_BYTES - (AMP_LOOPBACK_MAX_DELAY_BYTES % 4))));
                        delayBytes = AMP_LOOPBACK_MAX_DELAY_BYTES - (AMP_LOOPBACK_MAX_DELAY_BYTES % 4);
                    }

                    for (i = 0; i < delayBytes; i++)
                    {
                        ringbuf_write(s_AmpRxDataRingBuffer, &dummyZero, 1);
                    }

                    ringbufOcc = delayBytes;
                }

                /* Place the data in the ringbuffer. This data will be used for barge-in.
                 * It should not happen, but skip the packet if the ring buffer is full. */
                if (ringbufOcc + write_xfer.dataSize <= AMP_LOOPBACK_RINGBUF_SIZE)
                {
                    ringbuf_write(s_AmpRxDataRingBuffer, write_xfer.data, write_xfer.dataSize);
                }
                else
                {
                    configPRINTF(
                        ("Failed to write data to the loopback ringbuffer. data length = %d, free space = %d\r\n",
                         write_xfer.dataSize, AMP_LOOPBACK_RINGBUF_SIZE - ringbufOcc));
                }
            }

            xSemaphoreGive(s_LoopBackMutex);
        }
        else
        {
            status = SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandle, &write_xfer);
        }

        xSemaphoreGive(s_LoopBackStateMutex);
#endif /* !ENABLE_AEC */
#endif /* USE_MQS */
    }

    return status;
}

/**
 * @brief  Play the audio in a blocking manner using an internal simple Streamer which has a pool
 *         of buffers and use them in a pipeline manner.
 *         This function will exit with success only after the whole audio is played
 *         or the play was aborted (by calling SLN_AMP_AbortWrite).
 *         NOTE: For MQS, if the data is in flash, it is mandatory to pass inFlash as true.
 *               For TFA, inFlash parameter has no effect.
 *         NOTE: For MQS, if the data is in flash, this function
 *               will need to allocate (slotSize * slotCnt) memory in RAM.
 *         NOTE: This function assumes that no other audio is already playing.
 *
 * @param  data Pointer to the audio data stored in RAM or Flash.
 * @param  length Length of the audio data.
 * @param  slotSize Size of a chunk of audio to be scheduled to play. Recommended: OPTIMAL_AMP_SLOT_SIZE.
 * @param  slotCnt Number of slots in the amplifier pool. Recommended: 2 or more. Optimal: OPTIMAL_AMP_SLOT_CNT.
 * @param  inFlash Is the data stored in flash? For TFA has no effect. For MQS, provide TRUE if the data is in Flash.
 *
 * @return kStatus_Success if the audio was played or aborted (by calling SLN_AMP_AbortWrite).
 */
static status_t SLN_AMP_WriteAudioPipeline(
    uint8_t *data, uint32_t length, uint32_t slotSize, uint8_t slotCnt, bool inFlash)
{
    status_t ret             = kStatus_Success;
    uint32_t soundDataPlayed = 0;
    uint32_t packetSize      = 0;
    bool copyBuf             = false;

    uint8_t slotPoolIdx = 0;
    uint8_t *slotPools  = NULL;

    if ((data == NULL) || (length == 0) || (slotSize == 0) || (slotCnt == 0))
    {
        ret = kStatus_Fail;
    }

    if (ret == kStatus_Success)
    {
#if USE_MQS
        /* For MQS, we cannot play data directly from flash, because we need to modify
         * the data according to the set volume. Copy the data into RAM. */
        if (inFlash)
        {
            copyBuf = true;
        }
#endif /* USE_MQS */

        if (copyBuf)
        {
            slotPools = pvPortMalloc(slotSize * slotCnt);
            if (slotPools == NULL)
            {
                ret = kStatus_Fail;
            }
        }
    }

    if (ret == kStatus_Success)
    {
        s_AmplifierMaxBuffs  = slotCnt;
        s_AmplifierFreeBuffs = slotCnt;

        /* Block until the entire audio is scheduled to be played. */
        while ((ret == kStatus_Success) && (soundDataPlayed < length) && (s_AbortPlaying == false))
        {
            /* In case there is an empty slot, send a new packet to the Amplifier */
            if (s_AmplifierFreeBuffs > 0)
            {
                if (length - soundDataPlayed >= slotSize)
                {
                    packetSize = slotSize;
                }
                else
                {
                    packetSize = length - soundDataPlayed;
                }

                if (copyBuf)
                {
                    /* MQS requires the data to be in RAM, so copy it in case original data is in flash. */
                    memcpy(&slotPools[slotPoolIdx * slotSize], &data[soundDataPlayed], packetSize);

                    ret = SLN_AMP_TransferChunk(&slotPools[slotPoolIdx * slotSize], packetSize);
                }
                else
                {
                    ret = SLN_AMP_TransferChunk(&data[soundDataPlayed], packetSize);
                }

                if (ret == kStatus_Success)
                {
                    s_AmplifierFreeBuffs--;

                    slotPoolIdx = (slotPoolIdx + 1) % slotCnt;
                    soundDataPlayed += packetSize;
                }
            }

            vTaskDelay(1);
        }

        /* Wait for the last scheduled packets to be played. */
        while ((s_AmplifierFreeBuffs != s_AmplifierMaxBuffs) && (s_AbortPlaying == false))
        {
            vTaskDelay(1);
        }

        s_AmplifierMaxBuffs  = 0;
        s_AmplifierFreeBuffs = 0;
    }

    if (copyBuf)
    {
        vPortFree(slotPools);
    }

    return ret;
}

/**
 * @brief  Play the audio in a blocking manner.
 *         This is the core function for the Play_Audio_Async_Task created
 *         by calling SLN_AMP_WriteAudioNoWait to play audio from a separate task.
 *
 * @param  pvParameters Information about the audio to be played.
 */
static void SLN_AMP_PlayAudioAsyncTask(void *pvParameters)
{
    status_t status                = kStatus_Success;
    sln_amp_write_packet_t *packet = NULL;

    while (1)
    {
        xQueueReceive(s_AudioNoWaitTaskQueue, &packet, portMAX_DELAY);

        if (packet == NULL)
        {
            configPRINTF(("[ERROR] SLN_AMP_PlayAudioAsyncTask received empty packet\r\n"));
            continue;
        }

        status = SLN_AMP_WriteAudioPipeline(packet->data, packet->length, packet->slotSize, packet->slotCnt,
                                         packet->inFlash);
        if (status != kStatus_Success)
        {
            configPRINTF(("[ERROR] SLN_AMP_PlayAudioAsyncTask failed %d\r\n", status));
        }

        vPortFree(packet);
        packet = NULL;

        s_SlnAmpState = kSlnAmpIdle;
    }
}

/**
 * @brief  Callback triggered when AMP TX (audio chunk played) previously scheduled by:
 *         SLN_AMP_TransferChunk -> SAI_TransferSendEDMA;
 *         Increase the number of available buffers in the pool.
 *
 * @param  base
 * @param  handle
 * @param  status
 * @param  userData
 */
static void SLN_AMP_TxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    switch (s_SlnAmpState)
    {
        case kSlnAmpIdle:
            /* Streamer play was aborted. Do nothing. Streamer will handle it. */
            break;

        case kSlnAmpStreamer:
            /* Increase external free buffers count.
             * Audio should have been scheduled to play by SLN_AMP_WriteStreamerNoWait.
             * Used by the Streamer. */
            if (s_StreamerFreeBuffs != NULL)
            {
                (*s_StreamerFreeBuffs)++;

                if (*s_StreamerFreeBuffs == s_StreamerMaxBuffs)
                {
                    s_SlnAmpState = kSlnAmpIdle;
                }
            }
            else
            {
                s_SlnAmpState = kSlnAmpIdle;
            }
            break;

        case kSlnAmpPlayBlocking:
        case kSlnAmpPlayNonBlocking:
            /* Increase internal free buffers count.
             * Audio should have been scheduled to play by SLN_AMP_WriteAudioBlocking or SLN_AMP_WriteAudioNoWait.
             * Used by the Amplifier's SLN_AMP_WriteAudioBlocking and SLN_AMP_WriteAudioNoWait. */
            s_AmplifierFreeBuffs++;
            break;

        default:
            break;
    }
}

#if USE_MQS
/**
 * @brief  Adjust the data according to the previously set volume.
 *
 * @param  data Data to be adjusted.
 * @param  length Length of data to be adjusted.
 */
static void SLN_AMP_VolAndDiffInputControl(void *data, uint32_t length)
{
    uint32_t i;
    int16_t *data16 = (int16_t *)data;
    float volume    = ((mqs_config_t *)(s_CodecHandle.codecConfig->codecDevConfig))->volume;
    uint32_t len16  = length / sizeof(int16_t);

    for (i = 0; i < len16 - 1; i += 2)
    {
        /* The volume is decreased by multiplying the samples with values between 0 and 1 */
        data16[i]     = (int16_t)(data16[i] * volume);
        data16[i + 1] = -data16[i];
    }
}
#endif /* USE_MQS */

/*******************************************************************************
 * Loopback Static Functions
 ******************************************************************************/

#if ENABLE_AEC
#if USE_MQS

/**
 * @brief  Configure and start the timer used for MQS synchronization with PDM_to_PCM.
 */
static void SLN_AMP_StartLoopbackTimer(void)
{
    /* PERF and LOOPBACK mechanisms use the same GPT.
     * In case PERF is enabled, its initialization is called first (before Loopback initialization),
     * so there is no need to execute the initialization for LOOPBACK timer here.
     * In case PERF is disabled, initialize the Loopback GPT here. */
#if !SLN_TRACE_CPU_USAGE
    gpt_config_t gpt;

    /* The timer is set with a AMP_LOOPBACK_GPT_FREQ_MHZ clock freq. */
    CLOCK_SetMux(kCLOCK_PerclkMux, 1);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0);

    GPT_GetDefaultConfig(&gpt);
    gpt.divider = AMP_LOOPBACK_GPT_PS;
    GPT_Init(AMP_LOOPBACK_GPT, &gpt);

    GPT_StartTimer(AMP_LOOPBACK_GPT);
#endif /* SLN_TRACE_CPU_USAGE */
}
#endif /* USE_MQS */
#endif /* ENABLE_AEC */
#endif /* ENABLE_AMPLIFIER */
