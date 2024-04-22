/*
 * Copyright 2022-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sln_mic_config.h"

#if (MICS_TYPE == MICS_I2S)

#include "stdbool.h"
#include "stdint.h"

#include "FreeRTOS.h"
#include "event_groups.h"
#if USE_MQS
#include "ringbuffer.h"
#include "semphr.h"
#endif

#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_sai.h"

#include "sln_amplifier_processing.h"
#include "sln_i2s_mic_processing.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define I2S_MIC_EVENT_MASK       (PCM_PING_EVENT | PCM_PONG_EVENT)
#define I2S_MIC_EVENT_TIMEOUT_MS 1000

#if (I2S_MIC_RAW_SAMPLE_SIZE==2)
#define I2S_MIC_DMA_READ_BYTES  2U
#define I2S_MIC_DMA_CHUNK_BYTES kEDMA_TransferSize2Bytes
#elif (I2S_MIC_RAW_SAMPLE_SIZE==4)
#define I2S_MIC_DMA_READ_BYTES  4U
#define I2S_MIC_DMA_CHUNK_BYTES kEDMA_TransferSize4Bytes
#else
#error "I2S_MIC_RAW_SAMPLE_SIZE should be set to 2 or 4"
#endif /* I2S_MIC_RAW_SAMPLE_SIZE */

/* I2S mics SAI */
#define I2S_MIC_SAI SAI1
/* Select Audio/Video PLL (786.48 MHz) as SAI clock source */
#define I2S_MIC_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for SAI clock source */
#define I2S_MIC_SAI_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for SAI clock source */
#define I2S_MIC_SAI_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of SAI clock */
#define I2S_MIC_SAI_CLK_FREQ \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (I2S_MIC_SAI_CLOCK_SOURCE_DIVIDER + 1U) / (I2S_MIC_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

#define I2S_MIC_DMA_IRQ_PRIO (configMAX_SYSCALL_INTERRUPT_PRIORITY - 1)

/* Skip first chunks of mic data because mics are not reliable right after boot. */
#define SKIP_DIRTY_FRAMES 4

typedef struct _i2s_mic_edma_config
{
    uint32_t startIdx;              /* SAI channel index whose data to be read first. (ex: startIdx = 2 => start reading from channel 2). */
    uint32_t burstBytes;            /* Number of total bytes to be read in a minor loop. (Minor loop should cover all the channels application is interested in). */
    edma_transfer_size_t burstSize; /* Number of bytes to be read in a single "read" ("read" compose a minor loop). */
    uint32_t burstStride;           /* Number of bytes to add to the current read address after a "read". */
    edma_modulo_t burstMod;         /* Modulo feature. Most significant bits of the read addresses do not change and only the last burstMod bits are dynamic. */
    uint32_t smloff;                /* Number of bytes to add to the current read address after a minor loop (helps to get the initial read address after a minor loop). */
} i2s_mic_edma_config_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Current state of the mics */
static volatile bool s_micsOn = false;

/* Skip first chunks of mic data because mics are not reliable right after boot. */
static volatile uint8_t s_skipDirtyFrames = 0;

/* Used for communication between DMA and I2S processing task */
static EventGroupHandle_t s_i2sMicDmaEventGroup = NULL;

/*  Structure describing current I2S processing task configuration */
static mic_task_config_t s_taskConfig = {0};

/* Structure describing current SAI configuration */
static sai_mic_config_t g_i2sMicSaiConfig = {I2S_MIC_SAI, I2S_MIC_CHANNEL_MASK, I2S_MIC_RAW_FRAME_SAMPLES_COUNT, I2S_MIC_SAMPLING_EDGE};

/* Structure describing current configuration (EDMA, SAI, mics etc.) */
__attribute__((aligned(32))) static sln_mic_handle_t __attribute__((section(".bss.$SRAM_OC_NON_CACHEABLE"))) s_i2sMicSaiHandle = {0U};

/* Buffer to store RAW data from all enabled mics. It has 2 slots for Ping and Pong (write one while processing the other). */
__attribute__((aligned(32))) static uint8_t __attribute__((section(".bss.$SRAM_ITC"))) s_i2sMicRawData[EDMA_TCD_COUNT][I2S_MIC_RAW_FRAME_SAMPLES_COUNT * I2S_MIC_RAW_SAMPLE_SIZE * SLN_MIC_COUNT];

/* Buffer to store PCM data from all enabled mics. It has 2 slots for Ping and Pong (write one while processing the other). */
__attribute__((aligned(32))) static pcmPingPong_t __attribute__((section(".bss.$SRAM_ITC"))) s_i2sMicPcmData = {0};

#if ENABLE_AEC
#if USE_MQS
/* Buffer to store 16KHz PCM data from the speaker after downsampling. */
static int16_t s_amp16KhzData[PCM_SINGLE_CH_SMPL_COUNT * EDMA_TCD_COUNT] = {0};

/* Variable which helps to align Mic with AMP streams based on the timestamps */
volatile static uint32_t s_pingPongTimestamp = 0;
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void I2S_MIC_GetEdmaConfiguration(uint8_t channelMask, i2s_mic_edma_config_t *dmaConfig);
static uint32_t I2S_MIC_ConvertSizeToOffset(edma_transfer_size_t transferSize);
static void I2S_MIC_ConfigureEdma(sln_mic_handle_t *handle);
static void I2S_MIC_Configure(sln_mic_handle_t *handle);
static void I2S_MIC_StartMic(sln_mic_handle_t *handle);
static void I2S_MIC_StopMic(sln_mic_handle_t *handle);
static void PDM_MIC_DmaCallback(sln_mic_handle_t *handle);

/*******************************************************************************
 * Code
 ******************************************************************************/

#if ENABLE_AEC
#if USE_MQS
/**
 * @brief Update the last ping/pong event time stamp. Used to align Mic and Amp streams.
 */
static void I2S_MIC_UpdateTimestamp(void)
{
    if (s_taskConfig.getTimestamp != NULL)
    {
        s_pingPongTimestamp = s_taskConfig.getTimestamp();
    }
}
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/**
 * @brief Get EDMA configuration for current SAI channel mask.
 *
 * @param channelMask Current SAI channel mask for enabled mics.
 * @param dmaConfig Pointer where to store the configration.
 */
static void I2S_MIC_GetEdmaConfiguration(uint8_t channelMask, i2s_mic_edma_config_t *dmaConfig)
{
    memset(dmaConfig, 0, sizeof(i2s_mic_edma_config_t));

    /* Get DMA configuration for each supported combination of channels. */

    if ((channelMask == kSAI_Channel0Mask) ||
        (channelMask == kSAI_Channel1Mask) ||
        (channelMask == kSAI_Channel2Mask) ||
        (channelMask == kSAI_Channel3Mask))
    {
        /* Get configuration for 1 microphone configuration.
         * Determine index of the SAI register for the appropriate microphone
         * and set the desired number of bytes to be read to 2/4 in chunks of 2/4 bytes
         * depending on I2S_MIC_RAW_SAMPLE_SIZE.
         * The read address is not changed, so there is no need to add other configurations to alter
         * the read address. */

        if (channelMask == kSAI_Channel0Mask)
        {
            dmaConfig->startIdx = 0;
        }
        else if (channelMask == kSAI_Channel1Mask)
        {
            dmaConfig->startIdx = 1;
        }
        else if (channelMask == kSAI_Channel2Mask)
        {
            dmaConfig->startIdx = 2;
        }
        else if (channelMask == kSAI_Channel3Mask)
        {
            dmaConfig->startIdx = 3;
        }

        dmaConfig->burstBytes = I2S_MIC_DMA_READ_BYTES;
        dmaConfig->burstSize  = I2S_MIC_DMA_CHUNK_BYTES;
    }

    else if ((channelMask == (kSAI_Channel0Mask | kSAI_Channel1Mask)) ||
        (channelMask == (kSAI_Channel1Mask | kSAI_Channel2Mask)) ||
        (channelMask == (kSAI_Channel2Mask | kSAI_Channel3Mask)))
    {
        /* Get configuration for 2 adjacent microphones configuration.
         * Determine index of the SAI register for the first microphone
         * and set the desired number of bytes to be read to 2/4 * 2 in chunks of 2/4 bytes
         * depending on I2S_MIC_RAW_SAMPLE_SIZE.
         * It is also needed to jump 4 bytes after each read and jump back 8 bytes
         * to get to the initial channel after each minor loop. */

        if (channelMask == (kSAI_Channel0Mask | kSAI_Channel1Mask))
        {
            dmaConfig->startIdx = 0;
        }
        if (channelMask == (kSAI_Channel1Mask | kSAI_Channel2Mask))
        {
            dmaConfig->startIdx = 1;
        }
        else if (channelMask == (kSAI_Channel2Mask | kSAI_Channel3Mask))
        {
            dmaConfig->startIdx = 2;
        }

        dmaConfig->burstBytes = I2S_MIC_DMA_READ_BYTES * 2;
        dmaConfig->burstSize  = I2S_MIC_DMA_CHUNK_BYTES;

        dmaConfig->burstStride = 4;
        dmaConfig->smloff      = (uint32_t) -8;
    }

    else if ((channelMask == (kSAI_Channel0Mask | kSAI_Channel2Mask)) ||
        (channelMask == (kSAI_Channel1Mask | kSAI_Channel3Mask)))
    {
        /* Get configuration for 2 non-adjacent microphones configuration.
         * Determine index of the SAI register for the first microphone
         * and set the desired number of bytes to be read to 2/4 * 2 in chunks of 2/4 bytes
         * depending on I2S_MIC_RAW_SAMPLE_SIZE.
         * It is also needed to jump 8 bytes after each read and modulo the address
         * to circulate through 16 bytes of the 4 SAI channels. */

        if (channelMask == (kSAI_Channel0Mask | kSAI_Channel2Mask))
        {
            dmaConfig->startIdx = 0;
        }
        else if (channelMask == (kSAI_Channel1Mask | kSAI_Channel3Mask))
        {
            dmaConfig->startIdx = 1;
        }

        dmaConfig->burstBytes = I2S_MIC_DMA_READ_BYTES * 2;
        dmaConfig->burstSize  = I2S_MIC_DMA_CHUNK_BYTES;

        dmaConfig->burstStride = 8;
        dmaConfig->burstMod    = kEDMA_Modulo16bytes;
    }

    else if ((channelMask == (kSAI_Channel0Mask | kSAI_Channel1Mask | kSAI_Channel2Mask)) ||
        (channelMask == (kSAI_Channel1Mask | kSAI_Channel2Mask | kSAI_Channel3Mask)))
    {
        /* Get configuration for 3 adjacent microphones configuration.
         * Determine index of the SAI register for the first microphone
         * and set the desired number of bytes to be read to 2/4 * 3 in chunks of 2/4 bytes.
         * It is also needed to jump 4 bytes after each read and jump back 12 bytes
         * to get to the initial channel after each minor loop. */

        if (channelMask == (kSAI_Channel0Mask | kSAI_Channel1Mask | kSAI_Channel2Mask))
        {
            dmaConfig->startIdx = 0;
        }
        else if (channelMask == (kSAI_Channel1Mask | kSAI_Channel2Mask | kSAI_Channel3Mask))
        {
            dmaConfig->startIdx = 1;
        }

        dmaConfig->burstBytes = I2S_MIC_DMA_READ_BYTES * 3;
        dmaConfig->burstSize  = I2S_MIC_DMA_CHUNK_BYTES;

        dmaConfig->burstStride = 4;
        dmaConfig->smloff      = (uint32_t) -12;
    }

    else if (channelMask == (kSAI_Channel0Mask | kSAI_Channel1Mask | kSAI_Channel2Mask | kSAI_Channel3Mask))
    {
        /* Get configuration for 4 adjacent microphones configuration.
         * Determine index of the SAI register for the first microphone
         * and set the desired number of bytes to be read to 2/4 * 4 in chunks of 2/4 bytes.
         * It is also needed to jump 4 bytes after each read and modulo the address
         * to circulate through 16 bytes of the 4 SAI channels. */

        dmaConfig->startIdx = 0;

        dmaConfig->burstBytes = I2S_MIC_DMA_READ_BYTES * 4;
        dmaConfig->burstSize  = I2S_MIC_DMA_CHUNK_BYTES;

        dmaConfig->burstStride = 4;
        dmaConfig->burstMod    = kEDMA_Modulo16bytes;
    }

    else
    {
        /* Other channels configurations are not supported because they are not used and are not trivial to be implemented. */
        while(1);
    }
}

/**
 * @brief Convert transfer size in an appropriate mask.
 *
 * @param transferSize Transfer Size.
 */
static uint32_t I2S_MIC_ConvertSizeToOffset(edma_transfer_size_t transferSize)
{
    uint32_t retVal = 0;

    switch(transferSize)
    {
        case kEDMA_TransferSize1Bytes:
        case kEDMA_TransferSize2Bytes:
        case kEDMA_TransferSize4Bytes:
        case kEDMA_TransferSize8Bytes:
        case kEDMA_TransferSize32Bytes:
        {
            retVal = 1 << transferSize;
            break;
        }

        default:
        {
            while (1);
            break;
        }
    }

    return retVal;
}

/**
 * @brief Configure EDMA to work with SAI configuration.
 *
 * @param handle Microphone handle.
 */
static void I2S_MIC_ConfigureEdma(sln_mic_handle_t *handle)
{
    i2s_mic_edma_config_t dmaConfig = {0};

    I2S_MIC_GetEdmaConfiguration(handle->config->saiChannelMask, &dmaConfig);

#if (I2S_MIC_RAW_SAMPLE_SIZE==2)
    handle->dmaTcd[0].SADDR = (uint32_t)(&handle->config->sai->RDR[dmaConfig.startIdx]) + 2;
    handle->dmaTcd[1].SADDR = (uint32_t)(&handle->config->sai->RDR[dmaConfig.startIdx]) + 2;
#elif (I2S_MIC_RAW_SAMPLE_SIZE==4)
    handle->dmaTcd[0].SADDR = (uint32_t)(&handle->config->sai->RDR[dmaConfig.startIdx]);
    handle->dmaTcd[1].SADDR = (uint32_t)(&handle->config->sai->RDR[dmaConfig.startIdx]);
#endif /* I2S_MIC_RAW_SAMPLE_SIZE */

    handle->dmaTcd[0].SOFF = dmaConfig.burstStride;
    handle->dmaTcd[1].SOFF = dmaConfig.burstStride;

    handle->dmaTcd[0].ATTR = (DMA_ATTR_SMOD(dmaConfig.burstMod) | DMA_ATTR_SSIZE(dmaConfig.burstSize) | DMA_ATTR_DSIZE(dmaConfig.burstSize));
    handle->dmaTcd[1].ATTR = (DMA_ATTR_SMOD(dmaConfig.burstMod) | DMA_ATTR_SSIZE(dmaConfig.burstSize) | DMA_ATTR_DSIZE(dmaConfig.burstSize));

    handle->dmaTcd[0].NBYTES = dmaConfig.burstBytes;
    handle->dmaTcd[1].NBYTES = dmaConfig.burstBytes;

    if (dmaConfig.smloff != 0)
    {
        handle->dma->CR |= DMA_CR_EMLM(1U);

        handle->dmaTcd[0].NBYTES |= (DMA_NBYTES_MLOFFYES_SMLOE(1U) | DMA_NBYTES_MLOFFYES_MLOFF(dmaConfig.smloff));
        handle->dmaTcd[1].NBYTES |= (DMA_NBYTES_MLOFFYES_SMLOE(1U) | DMA_NBYTES_MLOFFYES_MLOFF(dmaConfig.smloff));
    }

    handle->dmaTcd[0].SLAST = 0U;
    handle->dmaTcd[1].SLAST = 0U;

    handle->dmaTcd[0].DADDR = (uint32_t)handle->pingPongBuffer[0];
    handle->dmaTcd[1].DADDR = (uint32_t)handle->pingPongBuffer[1];

    handle->dmaTcd[0].DOFF = I2S_MIC_ConvertSizeToOffset(dmaConfig.burstSize);
    handle->dmaTcd[1].DOFF = handle->dmaTcd[0].DOFF;

    handle->dmaTcd[0].CITER = handle->config->saiCaptureCount;
    handle->dmaTcd[1].CITER = handle->config->saiCaptureCount;

    handle->dmaTcd[0].DLAST_SGA = (uint32_t)&handle->dmaTcd[1];
    handle->dmaTcd[1].DLAST_SGA = (uint32_t)&handle->dmaTcd[0];

    handle->dmaTcd[0].CSR = (DMA_CSR_INTMAJOR_MASK | DMA_CSR_ESG_MASK);
    handle->dmaTcd[1].CSR = (DMA_CSR_INTMAJOR_MASK | DMA_CSR_ESG_MASK);

    handle->dmaTcd[0].BITER = handle->config->saiCaptureCount;
    handle->dmaTcd[1].BITER = handle->config->saiCaptureCount;

    EDMA_InstallTCD(handle->dma, handle->dmaChannel, &handle->dmaTcd[0]);

    DMAMUX_SetSource(DMAMUX, handle->dmaChannel, handle->dmaRequest);
    DMAMUX_EnableChannel(DMAMUX, handle->dmaChannel);

    NVIC_SetPriority(handle->dmaIrqNum, I2S_MIC_DMA_IRQ_PRIO);
    NVIC_EnableIRQ(handle->dmaIrqNum);
}

static void I2S_MIC_Configure(sln_mic_handle_t *handle)
{
    sai_transceiver_t saiConfig;

    CLOCK_SetMux(kCLOCK_Sai1Mux, I2S_MIC_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, I2S_MIC_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, I2S_MIC_SAI_CLOCK_SOURCE_DIVIDER);
    CLOCK_EnableClock(kCLOCK_Sai1);

    /* Configure EDMA for the current microphone configuration. */
    I2S_MIC_ConfigureEdma(handle);

    /* Configure SAI for the current microphone configuration. */
    SAI_Init(I2S_MIC_SAI);

    /* Generate I2S mode SAI configuration. */
    SAI_GetClassicI2SConfig(&saiConfig, kSAI_WordWidth32bits, kSAI_MonoRight, handle->config->saiChannelMask);

    saiConfig.masterSlave                 = kSAI_Master;
    saiConfig.bitClock.bclkSource         = kSAI_BclkSourceMclkDiv;
    saiConfig.bitClock.bclkPolarity       = handle->config->micClockPolarity;
    saiConfig.frameSync.frameSyncPolarity = kSAI_SampleOnRisingEdge;
    saiConfig.frameSync.frameSyncEarly    = false;
    saiConfig.serialData.dataOrder        = kSAI_DataMSB;

    SAI_RxSetConfig(I2S_MIC_SAI, &saiConfig);

    SAI_RxSetBitClockRate(I2S_MIC_SAI, I2S_MIC_SAI_CLK_FREQ, I2S_MIC_RAW_FREQUENCY_HZ, kSAI_WordWidth32bits, 2);

    handle->dma->EEI |= (1 << handle->dmaChannel);
    handle->dma->SERQ = DMA_SERQ_SERQ(handle->dmaChannel);

    SAI_RxEnableDMA(I2S_MIC_SAI, kSAI_FIFORequestDMAEnable, true);
}

/**
 * @brief Start the microphones.
 *
 * @param handle Microphone handle.
 */
static void I2S_MIC_StartMic(sln_mic_handle_t *handle)
{
    s_skipDirtyFrames = SKIP_DIRTY_FRAMES;

    EDMA_InstallTCD(handle->dma, handle->dmaChannel, &handle->dmaTcd[0]);
    handle->dma->SERQ = DMA_SERQ_SERQ(handle->dmaChannel);

    SAI_RxEnable(I2S_MIC_SAI, true);

    /* Wait for the dirty frames to be skipped so the mics are fully running. */
    while (s_skipDirtyFrames)
    {
        vTaskDelay(1);
    }
}

/**
 * @brief Stop the microphones.
 *
 * @param handle Microphone handle.
 */
static void I2S_MIC_StopMic(sln_mic_handle_t *handle)
{
    SAI_RxEnable(I2S_MIC_SAI, false);
    /* RE(Receiver Enable) bit and BCE(Bit Clock Enable) bit will be actually written to the RCSR register
     * at the end of the current frame. We should wait for the write to be done, because the following
     * reads of this register could use the old values. */
    while ((handle->config->sai->RCSR & I2S_RCSR_RE_MASK) != 0)
        ;

    handle->config->sai->RCSR |= (I2S_RCSR_FWF_MASK | I2S_RCSR_FEF_MASK | I2S_TCSR_SEF_MASK | I2S_TCSR_WSF_MASK | I2S_RCSR_FR_MASK);
    handle->dma->CERQ = DMA_CERQ_CERQ(handle->dmaChannel);
    EDMA_ResetChannel(handle->dma, handle->dmaChannel);
}

/**
 * @brief Process newly received microphone data.
 *
 * @param handle Microphone handle.
 */
static void PDM_MIC_DmaCallback(sln_mic_handle_t *handle)
{
    handle->dma->INT |= (1 << handle->dmaChannel);

    handle->dma->TCD[handle->dmaChannel].CSR &= ~DMA_CSR_DONE_MASK;

    handle->dma->TCD[handle->dmaChannel].CSR |= DMA_CSR_ESG_MASK;

    handle->dma->SERQ = DMA_SERQ_SERQ(handle->dmaChannel);

    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;

#if ENABLE_AEC
#if USE_MQS
    if (handle->pdmMicUpdateTimestamp != NULL)
    {
        handle->pdmMicUpdateTimestamp();
    }
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

    if (s_skipDirtyFrames > 0)
    {
        s_skipDirtyFrames--;
    }
    else
    {
        if (handle->pingPongTracker & 0x01U)
        {
            if (xEventGroupGetBitsFromISR(handle->eventGroup) & handle->pongFlag)
            {
                xEventGroupSetBitsFromISR((handle->eventGroup), handle->errorFlag, &xHigherPriorityTaskWoken);
            }
            xEventGroupSetBitsFromISR((handle->eventGroup), handle->pongFlag, &xHigherPriorityTaskWoken);
        }
        else
        {
            if (xEventGroupGetBitsFromISR(handle->eventGroup) & handle->pingFlag)
            {
                xEventGroupSetBitsFromISR((handle->eventGroup), handle->errorFlag, &xHigherPriorityTaskWoken);
            }
            xEventGroupSetBitsFromISR((handle->eventGroup), handle->pingFlag, &xHigherPriorityTaskWoken);
        }
    }

    handle->pingPongTracker++;
}

/**
 * @brief DMA interrupt handler triggered when new mic data is available.
 *        It overrides the default handler which is WEAK.
 */
void DMA6_DMA22_IRQHandler(void)
{
    PDM_MIC_DmaCallback(&s_i2sMicSaiHandle);
}

/*******************************************************************************
 * API
 ******************************************************************************/

int16_t *I2S_MIC_GetPcmBufferPointer(void)
{
    return (int16_t *)s_i2sMicPcmData;
}

int16_t *I2S_MIC_GetAmpBufferPointer(void)
{
#if ENABLE_AEC
    return s_amp16KhzData;
#else
    return NULL;
#endif /* ENABLE_AEC */
}

void I2S_MIC_SetTaskConfig(mic_task_config_t *config)
{
    if (NULL != config)
    {
        memcpy(&s_taskConfig, config, sizeof(s_taskConfig));
    }
}

void I2S_MIC_MicsOn(void)
{
    /* Do nothing if already started */
    if (s_micsOn == false)
    {
        if (s_i2sMicDmaEventGroup != NULL)
        {
            xEventGroupClearBits(s_i2sMicDmaEventGroup, 0x00FFFFFF);
        }

        s_i2sMicSaiHandle.pingPongTracker = 0;

        memset(s_i2sMicRawData, 0, sizeof(s_i2sMicRawData));
        memset(s_i2sMicPcmData, 0, sizeof(s_i2sMicPcmData));
#if ENABLE_AEC
#if USE_MQS
        memset(s_amp16KhzData, 0, sizeof(s_amp16KhzData));
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

        I2S_MIC_StartMic(&s_i2sMicSaiHandle);

#if ENABLE_AEC
        /* amplifier loopback */
        if (NULL != s_taskConfig.feedbackEnable)
        {
            s_taskConfig.feedbackEnable();
        }
#endif /* ENABLE_AEC */

        s_micsOn = true;
    }
}

void I2S_MIC_MicsOff(void)
{
    if (s_micsOn == true)
    {
        I2S_MIC_StopMic(&s_i2sMicSaiHandle);

#if ENABLE_AEC
        /* Amplifier loopback */
        if (NULL != s_taskConfig.feedbackDisable)
        {
            s_taskConfig.feedbackDisable();
        }
#endif /* ENABLE_AEC */

        s_micsOn = false;
    }
}

bool I2S_MIC_GetMicsState(void)
{
    return s_micsOn;
}

void I2S_MIC_Task(void *pvParameters)
{
    EventBits_t preProcessEvents = 0U;

    s_i2sMicDmaEventGroup = xEventGroupCreate();
    if (s_i2sMicDmaEventGroup == NULL)
    {
        configPRINTF(("Failed to create s_i2sMicDmaEventGroup\r\n"));
    }

    s_i2sMicSaiHandle.eventGroup        = s_i2sMicDmaEventGroup;
    s_i2sMicSaiHandle.config            = &g_i2sMicSaiConfig;
    s_i2sMicSaiHandle.dma               = DMA0;
    s_i2sMicSaiHandle.dmaChannel        = 6U;
    s_i2sMicSaiHandle.dmaIrqNum         = DMA6_DMA22_IRQn;
    s_i2sMicSaiHandle.dmaRequest        = (uint8_t)kDmaRequestMuxSai1Rx;
    s_i2sMicSaiHandle.pongFlag          = PCM_PONG_EVENT;
    s_i2sMicSaiHandle.pingFlag          = PCM_PING_EVENT;
    s_i2sMicSaiHandle.errorFlag         = PCM_ERROR_EVENT;
    s_i2sMicSaiHandle.pingPongBuffer[0] = (uint32_t *)(&s_i2sMicRawData[0][0]);
    s_i2sMicSaiHandle.pingPongBuffer[1] = (uint32_t *)(&s_i2sMicRawData[1][0]);
#if ENABLE_AEC
#if USE_MQS
    s_i2sMicSaiHandle.pdmMicUpdateTimestamp = I2S_MIC_UpdateTimestamp;
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

    I2S_MIC_Configure(&s_i2sMicSaiHandle);

    I2S_MIC_MicsOn();

    for (;;)
    {
        preProcessEvents = xEventGroupWaitBits(s_i2sMicDmaEventGroup, I2S_MIC_EVENT_MASK, pdTRUE, pdFALSE,
                                               portTICK_PERIOD_MS * I2S_MIC_EVENT_TIMEOUT_MS);

        /* If no event group bit is set it means that the timeout was triggered */
        if ((preProcessEvents & I2S_MIC_EVENT_MASK) == 0)
        {
            /* The timeout is triggered so it means that we are not receiving any data from the mics.
             * This can be cause by an error in the SAI interface and we need to reset the mics to recover
             * or the mics are off, in which case we just continue. */
            if (true == s_micsOn)
            {
                configPRINTF(("SAI stopped working. Repairing it.\r\n"));
                I2S_MIC_MicsOff();
                I2S_MIC_MicsOn();
            }

            continue;
        }

        if (preProcessEvents & PCM_PING_EVENT)
        {
#if ENABLE_AEC
            SLN_AMP_GetAmpStream(&s_taskConfig, &s_amp16KhzData[0], &s_pingPongTimestamp);
#endif /* ENABLE_AEC */

            I2S_MIC_ProcessMicStream(s_i2sMicRawData[0], (int16_t *)s_i2sMicPcmData[0]);

            xTaskNotify(*(s_taskConfig.processingTask), PCM_PING_EVENT, eSetBits);
        }

        if (preProcessEvents & PCM_PONG_EVENT)
        {
#if ENABLE_AEC
            SLN_AMP_GetAmpStream(&s_taskConfig, &s_amp16KhzData[PCM_SINGLE_CH_SMPL_COUNT], &s_pingPongTimestamp);
#endif /* ENABLE_AEC */

            I2S_MIC_ProcessMicStream(s_i2sMicRawData[1], (int16_t *)s_i2sMicPcmData[1]);

            xTaskNotify(*(s_taskConfig.processingTask), PCM_PONG_EVENT, eSetBits);
        }
    }
}

#endif /* (MICS_TYPE == MICS_I2S) */
