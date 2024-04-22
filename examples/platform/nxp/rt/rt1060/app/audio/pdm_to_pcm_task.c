/*
 * Copyright 2018-2022, 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sln_mic_config.h"

#if (MICS_TYPE == MICS_PDM)

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "task.h"
#include "timers.h"

/* NXP includes. */
#include "board.h"
#include "pdm_to_pcm_task.h"
#include "sln_amplifier_processing.h"
#include "sln_pdm_mic.h"

#if ENABLE_AEC
#if USE_MQS
#include "semphr.h"
#include "ringbuffer.h"
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

#if USE_NEW_PDM_PCM_LIB
#include "PdmToPcm_LibHead.h"
#else
#include "sln_dsp_toolbox.h"
#endif /* USE_NEW_PDM_PCM_LIB */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AMP_DSP_STREAM  0U
#define MIC1_DSP_STREAM 1U
#define MIC2_DSP_STREAM 2U
#define MIC3_DSP_STREAM 3U

#define MIC3_START_IDX (SAI1_CH_COUNT * PCM_SINGLE_CH_SMPL_COUNT)

#define PDM_PCM_EVENT_TIMEOUT_MS      500
#define PDM_PCM_EVENT_TIMEOUT_RETRIES 1

#define EVT_PING_MASK (MIC1_PING_EVENT | MIC2_PING_EVENT | MIC3_PING_EVENT)
#define EVT_PONG_MASK (MIC1_PONG_EVENT | MIC2_PONG_EVENT | MIC3_PONG_EVENT)

#define PDM_PCM_EVENT_MASK (EVT_PING_MASK | EVT_PONG_MASK | AMP_REFERENCE_SIGNAL | PDM_ERROR_FLAG | AMP_ERROR_FLAG)

#if USE_NEW_PDM_PCM_LIB
#define MIC_SCALE_FACTOR            10
#define PDM_TO_PCM_CONVERT_MEM_SIZE 2900
#else
#define MIC_SCALE_FACTOR 4
#endif /* USE_NEW_PDM_PCM_LIB */

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

#if SAI1_CH_COUNT
__attribute__((aligned(8))) uint32_t g_Sai1PdmPingPong[EDMA_TCD_COUNT][PDM_SAMPLE_COUNT * SAI1_CH_COUNT];

sai_mic_config_t g_pdmMicSai1 = {SAI1, USE_SAI1_CH_MASK, PDM_SAMPLE_COUNT, PDM_MIC_SAMPLING_EDGE};

__attribute__((aligned(32))) sln_mic_handle_t g_pdmMicSai1Handle = {0U};
#endif

#if USE_SAI2_MIC
__attribute__((aligned(8))) uint32_t g_Sai2PdmPingPong[EDMA_TCD_COUNT][PDM_SAMPLE_COUNT];

sai_mic_config_t g_pdmMicSai2 = {SAI2, kMicChannel1, PDM_SAMPLE_COUNT, PDM_MIC_SAMPLING_EDGE};

__attribute__((aligned(32))) sln_mic_handle_t g_pdmMicSai2Handle = {0U};
#endif

#if USE_NEW_PDM_PCM_LIB
__attribute__((aligned(8))) static uint8_t s_pdmPcmConvertMemBuff[PDM_TO_PCM_CONVERT_MEM_SIZE];
__attribute__((aligned(8))) static uint32_t s_OneMicPdmData[PDM_SAMPLE_COUNT];
/* s_OneMicPcmData buffer will be used for initial PDM to PCM conversion to 32KHz (hence double size). */
__attribute__((aligned(8))) static float s_OneMicPcmData[PCM_SINGLE_CH_SMPL_COUNT * 2];
#else
uint8_t *dspMemPool = NULL;
#endif /* USE_NEW_PDM_PCM_LIB */

static mic_task_config_t s_config;
static EventGroupHandle_t s_PdmDmaEventGroup;
__attribute__((aligned(2))) static pcmPingPong_t s_pcmStream;

bool g_micsOn            = false;
bool g_decimationStarted = false;

#if ENABLE_AEC
#if USE_MQS
static int16_t s_ampOutput[PCM_SINGLE_CH_SMPL_COUNT * 2];
volatile static uint32_t s_pingPongTimestamp = 0;
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if ENABLE_AEC
#if USE_MQS
/*!
 * @brief * Get the current ticks of the Loopback's timer.
 */
static void pdm_to_pcm_update_timestamp(void);
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/*******************************************************************************
 * Code
 ******************************************************************************/
#if USE_SAI2_MIC
void DMA0_DMA16_IRQHandler(void)
{
    PDM_MIC_DmaCallback(&g_pdmMicSai2Handle);
}
#endif

#if SAI1_CH_COUNT
void DMA1_DMA17_IRQHandler(void)
{
    PDM_MIC_DmaCallback(&g_pdmMicSai1Handle);
}
#endif

#if USE_NEW_PDM_PCM_LIB

#else
static int32_t pdm_to_pcm_dsp_init(uint8_t **memPool)
{
    int32_t dspStatus = kDspSuccess;

    dspStatus = SLN_DSP_Init(memPool, pvPortMalloc);

    if (kDspSuccess == dspStatus)
    {
        dspStatus = SLN_DSP_SetCaptureLength(memPool, PDM_SAMPLE_COUNT * PDM_CAPTURE_SIZE_BYTES);
    }

    if (kDspSuccess == dspStatus)
    {
        dspStatus = SLN_DSP_SetGainFactor(memPool, 3);
    }

    return dspStatus;
}

int32_t pdm_to_pcm_set_gain(uint8_t u8Gain)
{
    return SLN_DSP_SetGainFactor(&dspMemPool, u8Gain);
}

void pdm_to_pcm_stream_formatter(int16_t *pcmBuffer, pdm_pcm_input_event_t micEvent, uint8_t pcmFormat)
{
    uint32_t validEvents =
        (MIC1_PING_EVENT | MIC2_PING_EVENT | MIC3_PING_EVENT | MIC1_PONG_EVENT | MIC2_PONG_EVENT | MIC3_PONG_EVENT);

    if (validEvents & micEvent)
    {
        uint32_t idxStart    = 0;
        uint32_t idxEnd      = 0;
        uint32_t idxIter     = 0;
        uint32_t pingPongIdx = 0;

        bool isPingEvent = (micEvent & MIC1_PING_EVENT) || (micEvent & MIC2_PING_EVENT) || (micEvent & MIC3_PING_EVENT);
        bool isMicTwoEvent = (micEvent & MIC2_PING_EVENT) || (micEvent & MIC2_PONG_EVENT);
        bool isMicTreEvent = (micEvent & MIC3_PING_EVENT) || (micEvent & MIC3_PONG_EVENT);

        if (isPingEvent)
        {
            pingPongIdx = 0;
        }
        else
        {
            pingPongIdx = 1;
        }

#if SLN_MIC_COUNT == 3
        if (isMicTreEvent)
        {
            idxStart = (pcmFormat) ? 2 : (2 * PCM_SINGLE_CH_SMPL_COUNT);
        }
#else
        isMicTwoEvent = isMicTreEvent;
#endif

        if (isMicTwoEvent)
        {
            idxStart = (pcmFormat) ? 1 : PCM_SINGLE_CH_SMPL_COUNT;
        }

        idxEnd = (pcmFormat) ? PCM_SAMPLE_COUNT : (PCM_SINGLE_CH_SMPL_COUNT + idxStart);

        idxIter = (pcmFormat) ? 2U : 1U;

        for (uint32_t idx = idxStart; idx < idxEnd; idx += idxIter)
        {
            s_pcmStream[pingPongIdx][idx] = *pcmBuffer;
            pcmBuffer++;
        }
    }
}

uint8_t **pdm_to_pcm_get_mempool(void)
{
    return &dspMemPool;
}
#endif /* USE_NEW_PDM_PCM_LIB */

status_t pcm_to_pcm_set_config(mic_task_config_t *config)
{
    status_t status = kStatus_Fail;

    if (NULL != config)
    {
        status = kStatus_Success;
    }

    if (kStatus_Success == status)
    {
        memcpy(&s_config, config, sizeof(s_config));

#if ENABLE_AEC
#if USE_MQS
        if ((s_config.loopbackRingBuffer == NULL) || (s_config.loopbackMutex == NULL) ||
            (s_config.updateTimestamp == NULL) || (s_config.getTimestamp == NULL))
        {
            status = kStatus_InvalidArgument;
        }
#endif /* USE_MQS */
#endif /* ENABLE_AEC */
    }

    return status;
}

mic_task_config_t *pcm_to_pcm_get_config(void)
{
    return &s_config;
}

void pdm_to_pcm_set_task_handle(TaskHandle_t *handle)
{
    if (NULL != handle)
    {
        s_config.thisTask = handle;
    }
}

void pdm_to_pcm_set_audio_proces_task_handle(TaskHandle_t *handle)
{
    if (NULL != handle)
    {
        s_config.processingTask = handle;
    }
}

TaskHandle_t pdm_to_pcm_get_task_handle(void)
{
    return *(s_config.thisTask);
}

#if ENABLE_AEC
#if USE_MQS
void pdm_to_pcm_set_loopback_mutex(SemaphoreHandle_t *mutex)
{
    s_config.loopbackMutex = *mutex;
}

void pdm_to_pcm_set_loopback_ring_buffer(ringbuf_t *ring_buf)
{
    s_config.loopbackRingBuffer = ring_buf;
}
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

int16_t *pdm_to_pcm_get_amp_output(void)
{
#if ENABLE_AEC
    return s_ampOutput;
#else
    return NULL;
#endif /* ENABLE_AEC */
}

int16_t *pdm_to_pcm_get_pcm_output(void)
{
    return (int16_t *)s_pcmStream;
}

static volatile EventBits_t preProcessEvents  = 0U;
static volatile EventBits_t postProcessEvents = 0U;
static uint32_t u32AmpIndex                   = 0;

#if ENABLE_AEC
#if USE_MQS
static void pdm_to_pcm_update_timestamp(void)
{
    if (s_config.getTimestamp != NULL)
    {
        s_pingPongTimestamp = s_config.getTimestamp();
    }
}
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

void pdm_to_pcm_task(void *pvParameters)
{
    uint8_t timeout_retries = 0;

#if USE_NEW_PDM_PCM_LIB
    PdmConvertingLibStatus pdmPcmStatus  = Status_SUCCESS;
    uint32_t pdmPcmConvertMemRequiredMem = 0;

    pdmPcmStatus = PdmToPcm_Init(PdmConvertingCfg4, SLN_MIC_COUNT, PCM_SINGLE_CH_SMPL_COUNT * 2);
    if (pdmPcmStatus == Status_SUCCESS)
    {
        pdmPcmConvertMemRequiredMem = PdmToPcm_GetHeapSizeNeededForPdmConverting();
        if (pdmPcmConvertMemRequiredMem <= PDM_TO_PCM_CONVERT_MEM_SIZE)
        {
            PdmToPcm_Create(s_pdmPcmConvertMemBuff);
            g_decimationStarted = true;
        }
        else
        {
            configPRINTF(("[ERROR] Failed to create PDM to PCM conversion library instance. "
                    "Allocated memory %d is smaller than required memory %d\r\n",
                    PDM_TO_PCM_CONVERT_MEM_SIZE,
                    pdmPcmConvertMemRequiredMem));
            pdmPcmStatus = Status_FAIL;
        }
    }
    else
    {
        configPRINTF(("[ERROR] Failed to initialize PDM to PCM conversion library.\r\n"));
    }
#else
    int32_t dspStatus    = kDspSuccess;
    uint32_t *dspScratch = NULL;

    dspStatus = pdm_to_pcm_dsp_init(&dspMemPool);
    if (kDspSuccess == dspStatus)
    {
        dspStatus = SLN_DSP_create_scratch_buf(&dspMemPool, &dspScratch, pvPortMalloc);
        if (kDspSuccess == dspStatus)
        {
            pdm_to_pcm_set_gain(MIC_SCALE_FACTOR);
            g_decimationStarted = true;
        }
        else
        {
            configPRINTF(("ERROR [%d]: DSP Toolbox initialization has failed!\r\n", dspStatus));
        }
    }
    else
    {
        configPRINTF(("ERROR [%d]: PDM to PCM DSP initialization has failed!\r\n", dspStatus));
    }
#endif /* USE_NEW_PDM_PCM_LIB */

    s_PdmDmaEventGroup = xEventGroupCreate();
    if (s_PdmDmaEventGroup == NULL)
    {
        configPRINTF(("Failed to create s_PdmDmaEventGroup\r\n"));
    }

#if SAI1_CH_COUNT
    g_pdmMicSai1Handle.eventGroup        = s_PdmDmaEventGroup;
    g_pdmMicSai1Handle.config            = &g_pdmMicSai1;
    g_pdmMicSai1Handle.dma               = DMA0;
    g_pdmMicSai1Handle.dmaChannel        = 1U;
    g_pdmMicSai1Handle.dmaIrqNum         = DMA1_DMA17_IRQn;
    g_pdmMicSai1Handle.dmaRequest        = (uint8_t)kDmaRequestMuxSai1Rx;
    g_pdmMicSai1Handle.pongFlag          = MIC1_PONG_EVENT;
    g_pdmMicSai1Handle.pingFlag          = MIC1_PING_EVENT;
    g_pdmMicSai1Handle.errorFlag         = PDM_ERROR_FLAG;
    g_pdmMicSai1Handle.pingPongBuffer[0] = (uint32_t *)(&g_Sai1PdmPingPong[0][0]);
    g_pdmMicSai1Handle.pingPongBuffer[1] = (uint32_t *)(&g_Sai1PdmPingPong[1][0]);
#if ENABLE_AEC
#if USE_MQS
    g_pdmMicSai1Handle.pdmMicUpdateTimestamp = pdm_to_pcm_update_timestamp;
#endif /* USE_MQS */
#endif /* ENABLE_AEC */
#endif /* SAI1_CH_COUNT */

#if USE_SAI2_MIC
    g_pdmMicSai2Handle.eventGroup        = s_PdmDmaEventGroup;
    g_pdmMicSai2Handle.config            = &g_pdmMicSai2;
    g_pdmMicSai2Handle.dma               = DMA0;
    g_pdmMicSai2Handle.dmaChannel        = 0U;
    g_pdmMicSai2Handle.dmaIrqNum         = DMA0_DMA16_IRQn;
    g_pdmMicSai2Handle.dmaRequest        = (uint8_t)kDmaRequestMuxSai2Rx;
    g_pdmMicSai2Handle.pongFlag          = MIC3_PONG_EVENT;
    g_pdmMicSai2Handle.pingFlag          = MIC3_PING_EVENT;
    g_pdmMicSai2Handle.errorFlag         = PDM_ERROR_FLAG;
    g_pdmMicSai2Handle.pingPongBuffer[0] = (uint32_t *)(&g_Sai2PdmPingPong[0][0]);
    g_pdmMicSai2Handle.pingPongBuffer[1] = (uint32_t *)(&g_Sai2PdmPingPong[1][0]);
#endif

    pdm_to_pcm_mics_on();

    for (;;)
    {
        preProcessEvents = xEventGroupWaitBits(s_PdmDmaEventGroup, PDM_PCM_EVENT_MASK, pdTRUE, pdFALSE,
                (PDM_PCM_EVENT_TIMEOUT_MS * portTICK_PERIOD_MS));

        /* If no event group bit is set it means that the timeout was triggered */
        if ((preProcessEvents & PDM_PCM_EVENT_MASK) == 0)
        {
            /* The timeout is triggered so it means that we are not receiving any data from the mics.
             * This can be cause by an error in the SAI interface and we need to reset the mics to recover
             * or the mics are off, in which case we just continue.
             */
            if (true == g_micsOn)
            {
                if (timeout_retries >= PDM_PCM_EVENT_TIMEOUT_RETRIES)
                {
                    timeout_retries = 0;
                    configPRINTF(("SAI stopped working. Repairing it.\r\n"));

                    pdm_to_pcm_mics_off();
                    pdm_to_pcm_mics_on();
                }
                else
                {
                    timeout_retries++;
                }
            }
            else
            {
                timeout_retries = 0;
            }

            continue;
        }
        else
        {
            timeout_retries = 0;
        }

        if (preProcessEvents & MIC1_PING_EVENT)
        {
#if SAI1_CH_COUNT == 2

#if ENABLE_AEC
#if USE_MQS
            SLN_AMP_GetAmpStream(&s_config, &s_ampOutput[0], &s_pingPongTimestamp);
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

#if USE_NEW_PDM_PCM_LIB
            /* MIC 1 */
            for (uint32_t i = 0; i < PDM_SAMPLE_COUNT; i++)
            {
                s_OneMicPdmData[i] = g_Sai1PdmPingPong[0][2 * i];
            }
            pdmPcmStatus = PdmToPcm_ConvertOneFrame_Cfg4_WithHpf2(s_OneMicPdmData, s_OneMicPcmData, 0);
            if (pdmPcmStatus == Status_SUCCESS)
            {
                for (uint32_t i = 0; i < PCM_SINGLE_CH_SMPL_COUNT; i++)
                {
                    s_pcmStream[0][i] = (int16_t)(s_OneMicPcmData[i] * MIC_SCALE_FACTOR * UINT16_MAX);
                }
            }
            else
            {
                configPRINTF(("[ERROR] PdmToPcm_ConvertOneFrame failed for mic 0 ping\r\n"));
            }

            /* MIC 2 */
            for (uint32_t i = 0; i < PDM_SAMPLE_COUNT; i++)
            {
                s_OneMicPdmData[i] = g_Sai1PdmPingPong[0][2 * i + 1];
            }
            pdmPcmStatus = PdmToPcm_ConvertOneFrame_Cfg4_WithHpf2(s_OneMicPdmData, s_OneMicPcmData, 1);
            if (pdmPcmStatus == Status_SUCCESS)
            {
                for (uint32_t i = 0; i < PCM_SINGLE_CH_SMPL_COUNT; i++)
                {
                    s_pcmStream[0][PCM_SINGLE_CH_SMPL_COUNT + i] = (int16_t)(s_OneMicPcmData[i] * MIC_SCALE_FACTOR * UINT16_MAX);
                }
            }
            else
            {
                configPRINTF(("[ERROR] PdmToPcm_ConvertOneFrame failed for mic 1 ping\r\n"));
            }
#else
            if (kDspSuccess != SLN_DSP_pdm_to_pcm_multi_ch(&dspMemPool, 1U, 2U, &(g_Sai1PdmPingPong[0U][0U]),
                                                           &(s_pcmStream[0][0]), dspScratch))
            {
                configPRINTF(("PDM to PCM Conversion error: %d\r\n", kDspSuccess));
            }
#endif /* USE_NEW_PDM_PCM_LIB */
#else
            /* Perform PDM to PCM Conversion */
            SLN_DSP_pdm_to_pcm(&dspMemPool, MIC1_DSP_STREAM, (uint8_t *)(&g_Sai1PdmPingPong[0U][0U]),
                               &(s_pcmStream[0U][0U]));

#endif

            postProcessEvents |= MIC1_PING_EVENT;
            postProcessEvents |= MIC2_PING_EVENT;

            preProcessEvents &= ~MIC1_PING_EVENT;
            preProcessEvents &= ~MIC2_PING_EVENT;
        }

        if (preProcessEvents & MIC1_PONG_EVENT)
        {
#if SAI1_CH_COUNT == 2

#if ENABLE_AEC
#if USE_MQS
            SLN_AMP_GetAmpStream(&s_config, &s_ampOutput[PCM_SINGLE_CH_SMPL_COUNT], &s_pingPongTimestamp);
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

#if USE_NEW_PDM_PCM_LIB
            /* MIC 1 */
            for (uint32_t i = 0; i < PDM_SAMPLE_COUNT; i++)
            {
                s_OneMicPdmData[i] = g_Sai1PdmPingPong[1][2 * i];
            }
            pdmPcmStatus = PdmToPcm_ConvertOneFrame_Cfg4_WithHpf2(s_OneMicPdmData, s_OneMicPcmData, 0);
            if (pdmPcmStatus == Status_SUCCESS)
            {
                for (uint32_t i = 0; i < PCM_SINGLE_CH_SMPL_COUNT; i++)
                {
                    s_pcmStream[1][i] = (int16_t)(s_OneMicPcmData[i] * MIC_SCALE_FACTOR * UINT16_MAX);
                }
            }
            else
            {
                configPRINTF(("[ERROR] PdmToPcm_ConvertOneFrame failed for mic 0 pong\r\n"));
            }

            /* MIC 2 */
            for (uint32_t i = 0; i < PDM_SAMPLE_COUNT; i++)
            {
                s_OneMicPdmData[i] = g_Sai1PdmPingPong[1][2 * i + 1];
            }
            pdmPcmStatus = PdmToPcm_ConvertOneFrame_Cfg4_WithHpf2(s_OneMicPdmData, s_OneMicPcmData, 1);
            if (pdmPcmStatus == Status_SUCCESS)
            {
                for (uint32_t i = 0; i < PCM_SINGLE_CH_SMPL_COUNT; i++)
                {
                    s_pcmStream[1][PCM_SINGLE_CH_SMPL_COUNT + i] = (int16_t)(s_OneMicPcmData[i] * MIC_SCALE_FACTOR * UINT16_MAX);
                }
            }
            else
            {
                configPRINTF(("[ERROR] PdmToPcm_ConvertOneFrame failed for mic 1 pong\r\n"));
            }
#else
            if (kDspSuccess != SLN_DSP_pdm_to_pcm_multi_ch(&dspMemPool, MIC1_DSP_STREAM, SAI1_CH_COUNT,
                                                           &(g_Sai1PdmPingPong[1U][0U]), &(s_pcmStream[1U][0U]),
                                                           dspScratch))
            {
                configPRINTF(("PDM to PCM Conversion error: %d\r\n", kDspSuccess));
            }
#endif /* USE_NEW_PDM_PCM_LIB */
#else

            /* Perform PDM to PCM Conversion */
            SLN_DSP_pdm_to_pcm(&dspMemPool, MIC1_DSP_STREAM, (uint8_t *)(&g_Sai1PdmPingPong[1U][0U]),
                               &(s_pcmStream[1U][0U]));

#endif

            postProcessEvents |= MIC1_PONG_EVENT;
            postProcessEvents |= MIC2_PONG_EVENT;

            preProcessEvents &= ~MIC1_PONG_EVENT;
            preProcessEvents &= ~MIC2_PONG_EVENT;
        }

#if (USE_SAI2_MIC)
        if (preProcessEvents & MIC3_PING_EVENT)
        {

            /* Perform PDM to PCM Conversion */
            SLN_DSP_pdm_to_pcm(&dspMemPool, MIC3_DSP_STREAM, (uint8_t *)(&g_Sai2PdmPingPong[0U][0U]),
                               &(s_pcmStream[0U][MIC3_START_IDX]));

            postProcessEvents |= MIC3_PING_EVENT;
            preProcessEvents &= ~MIC3_PING_EVENT;

        }

        if (preProcessEvents & MIC3_PONG_EVENT)
        {
            /* Perform PDM to PCM Conversion */
            SLN_DSP_pdm_to_pcm(&dspMemPool, MIC3_DSP_STREAM, (uint8_t *)(&g_Sai2PdmPingPong[1U][0U]),
                               &(s_pcmStream[1U][MIC3_START_IDX]));

            postProcessEvents |= MIC3_PONG_EVENT;
            preProcessEvents &= ~MIC3_PONG_EVENT;
        }

        if (preProcessEvents & PDM_ERROR_FLAG)
        {
            configPRINTF(("[PDM-PCM] - Missed Event \r\n"));
            preProcessEvents &= ~PDM_ERROR_FLAG;
        }
#else
        postProcessEvents |= MIC3_PING_EVENT;
        postProcessEvents |= MIC3_PONG_EVENT;
#endif

        if (EVT_PING_MASK == (postProcessEvents & EVT_PING_MASK))
        {
            if (NULL == *(s_config.processingTask))
            {
                configPRINTF(("ERROR: Audio Processing Task Handle NULL!\r\n"));
            }
            else
            {
                xTaskNotify(*(s_config.processingTask), PCM_PING_EVENT, eSetBits);
            }

            postProcessEvents &= ~(EVT_PING_MASK);
        }
        else if (EVT_PONG_MASK == (postProcessEvents & EVT_PONG_MASK))
        {
            if (NULL == *(s_config.processingTask))
            {
                configPRINTF(("ERROR: Audio Processing Task Handle NULL!\r\n"));
            }
            else
            {
                xTaskNotify(*(s_config.processingTask), PCM_PONG_EVENT, eSetBits);
            }

            postProcessEvents &= ~(EVT_PONG_MASK);
        }
    }
}

void pdm_to_pcm_mics_off(void)
{
    /* Do nothing if already off or decimation not started */
    if ((true == g_micsOn) && (true == g_decimationStarted))
    {
#if SAI1_CH_COUNT
        PDM_MIC_StopMic(&g_pdmMicSai1Handle);
#endif

#if USE_SAI2_MIC
        PDM_MIC_StopMic(&g_pdmMicSai2Handle);
#endif

#if SAI1_CH_COUNT
        g_pdmMicSai1Handle.pingPongTracker = 0;
#endif

#if USE_SAI2_MIC
        g_pdmMicSai2Handle.pingPongTracker = 0;
#endif

        memset(s_pcmStream, 0, sizeof(pcmPingPong_t));

#if ENABLE_AEC
        /* amplifier loopback */
        if (NULL != s_config.feedbackDisable)
        {
            s_config.feedbackDisable();
        }
#endif /* ENABLE_AEC */

        /* update flag */
        g_micsOn = false;
    }
}

void pdm_to_pcm_mics_on(void)
{
    /* Do nothing if already on or decimation not started */
    if ((false == g_micsOn) && (true == g_decimationStarted))
    {
        if (s_PdmDmaEventGroup != NULL)
        {
            xEventGroupClearBits(s_PdmDmaEventGroup, 0x00FFFFFF);
        }
        preProcessEvents  = 0;
        postProcessEvents = 0;
        u32AmpIndex       = 0;

#if ENABLE_AEC
        if (NULL != s_config.feedbackEnable)
        {
            s_config.feedbackEnable();
        }
#endif /* ENABLE_AEC */
#if SAI1_CH_COUNT
        memset(g_Sai1PdmPingPong, 0, sizeof(g_Sai1PdmPingPong));
        PDM_MIC_ConfigMic(&g_pdmMicSai1Handle);
#endif

#if USE_SAI2_MIC
        memset(g_Sai2PdmPingPong, 0, sizeof(g_Sai2PdmPingPong));
        PDM_MIC_ConfigMic(&g_pdmMicSai2Handle);
#endif

#if USE_SAI1_CH_MASK
        PDM_MIC_StartMic(&g_pdmMicSai1Handle);
#endif

#if USE_SAI2_MIC
        PDM_MIC_StartMic(&g_pdmMicSai2Handle);
#endif

        /* amplifier loopback */

        /* update flag */
        g_micsOn = true;
    }
}
bool pdm_to_pcm_get_mics_state(void)
{
    return g_micsOn;
}

#endif /* (MICS_TYPE == MICS_PDM) */
