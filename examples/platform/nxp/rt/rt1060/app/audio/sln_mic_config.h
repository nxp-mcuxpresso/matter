/*
 * Copyright 2022-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SLN_MIC_CONFIG_H_
#define SLN_MIC_CONFIG_H_

#include "FreeRTOS.h"
#include "event_groups.h"

#include "stdint.h"

#include "fsl_common.h"
#include "fsl_edma.h"
#include "fsl_sai.h"

#if USE_MQS
#include "semphr.h"
#include "ringbuffer.h"
#endif /* USE_MQS */

/*******************************************************************************
 * Microphone type selection
 ******************************************************************************/

#define MICS_PDM 1
#define MICS_I2S 2

#ifndef MICS_TYPE
#define MICS_TYPE MICS_I2S
#endif /* MICS_TYPE */

/*******************************************************************************
 * Microphone configuration
 ******************************************************************************/

#define EDMA_TCD_COUNT           2
#define PCM_SINGLE_CH_SMPL_COUNT 160U

#if (MICS_TYPE == MICS_PDM)

#define USE_NEW_PDM_PCM_LIB 1

#define PDM_BUFFER_COUNT       (EDMA_TCD_COUNT)
#define PDM_SAMPLE_COUNT       (PCM_SINGLE_CH_SMPL_COUNT * 4)
#define PDM_CAPTURE_SIZE_BYTES (4U)

#define PDM_MIC_SAI_CLK_HZ    2048000U
#define PDM_MIC_SAMPLES_HZ    kSAI_SampleRate32KHz
#define PDM_MIC_SAMPLING_EDGE kSAI_SampleOnRisingEdge

#define USE_SAI1_RX_DATA0_MIC (1U)
#define USE_SAI1_RX_DATA1_MIC (1U)
#define USE_SAI1_RX_DATA2_MIC (0U) /* Microphone to be connected on the extension connector J4.4 (data) & J4.3 (clock) */
#define USE_SAI1_RX_DATA3_MIC (0U) /* Microphone to be connected on the extension connector J4.5 (data) & J4.3 (clock) */

#define USE_SAI1_CH_MASK                                             \
    ((USE_SAI1_RX_DATA0_MIC << 0U) | (USE_SAI1_RX_DATA1_MIC << 1U) | \
     (USE_SAI1_RX_DATA2_MIC << 2U) |(USE_SAI1_RX_DATA3_MIC << 3U))

/* Smart Voice does not have SAI2 mic */
#define USE_SAI2_MIC     (0U)
#define USE_SAI2_CH_MASK (USE_SAI2_MIC << 0U)

#define PDM_MIC_COUNT \
    (USE_SAI1_RX_DATA0_MIC + USE_SAI1_RX_DATA1_MIC + USE_SAI1_RX_DATA2_MIC + USE_SAI1_RX_DATA3_MIC + USE_SAI2_MIC)
#define SAI1_CH_COUNT (USE_SAI1_RX_DATA0_MIC + USE_SAI1_RX_DATA1_MIC + USE_SAI1_RX_DATA2_MIC + USE_SAI1_RX_DATA3_MIC)
#define SAI_USE_COUNT ((USE_SAI2_MIC) + ((SAI1_CH_COUNT > 0) ? 1 : 0))

#define SLN_MIC_GET_PCM_BUFFER_POINTER() pdm_to_pcm_get_pcm_output()
#define SLN_MIC_GET_AMP_BUFFER_POINTER() pdm_to_pcm_get_amp_output()
#define SLN_MIC_SET_TASK_CONFIG(x)       pcm_to_pcm_set_config(x)
#define SLN_MIC_ON                       pdm_to_pcm_mics_on
#define SLN_MIC_OFF                      pdm_to_pcm_mics_off
#define SLN_MIC_GET_STATE()              pdm_to_pcm_get_mics_state()
#define SLN_MIC_GUARD                    {pdm_to_pcm_mics_off, pdm_to_pcm_mics_on}

#define SLN_MIC_TASK_FUNCTION            pdm_to_pcm_task
#define SLN_MIC_TASK_NAME                "pdm_to_pcm_task"
#define SLN_MIC_TASK_STACK_SIZE          1024
#define SLN_MIC_TASK_PRIORITY            (configMAX_PRIORITIES - 2)

#define SLN_MIC_COUNT                    2

#define SLN_MIC1_POS_X -10
#define SLN_MIC1_POS_Y 0

#define SLN_MIC2_POS_X 10
#define SLN_MIC2_POS_Y 0

#define SLN_MIC3_POS_X 0
#define SLN_MIC3_POS_Y -17.3

#elif (MICS_TYPE == MICS_I2S)

/* --- Dynamic configurations --- */
/* Set the number of I2S mics to be used. Acceptable values: 1, 2 or 3. */
#define SLN_MIC_COUNT 2

/* Set the number of bytes to read from the mics. Acceptable values: 2 or 4.
 * Note: Microphones offer 18 bits of data per sample.
 * I2S_MIC_RAW_SAMPLE_SIZE = 2 --> We lose last 2 precision bits, but we use less memory to store raw mic data: 640B per one mic.
 * I2S_MIC_RAW_SAMPLE_SIZE = 4 --> We don't lose any precision bits, but we use more memory to store raw mic data: 1280B per one mic. */
#define I2S_MIC_RAW_SAMPLE_SIZE 4

/* --- Static configurations --- */
#define SLN_MIC_GET_PCM_BUFFER_POINTER() I2S_MIC_GetPcmBufferPointer()
#define SLN_MIC_GET_AMP_BUFFER_POINTER() I2S_MIC_GetAmpBufferPointer()
#define SLN_MIC_SET_TASK_CONFIG(x)       I2S_MIC_SetTaskConfig(x)
#define SLN_MIC_ON                       I2S_MIC_MicsOn
#define SLN_MIC_OFF                      I2S_MIC_MicsOff
#define SLN_MIC_GET_STATE()              I2S_MIC_GetMicsState()
#define SLN_MIC_GUARD                    {I2S_MIC_MicsOff, I2S_MIC_MicsOn}

#define SLN_MIC_TASK_FUNCTION            I2S_MIC_Task
#define SLN_MIC_TASK_NAME                "i2s_pcm_task"
#define SLN_MIC_TASK_STACK_SIZE          256
#define SLN_MIC_TASK_PRIORITY            (configMAX_PRIORITIES - 2)

#define I2S_MIC_RAW_FREQUENCY_HZ        kSAI_SampleRate16KHz
#define I2S_MIC_RAW_FRAME_SAMPLES_COUNT (I2S_MIC_RAW_FREQUENCY_HZ / 100)
#define I2S_MIC_SAMPLING_EDGE           kSAI_SampleOnFallingEdge

#if (SLN_MIC_COUNT==1)
#define I2S_MIC_CHANNEL_MASK (kSAI_Channel0Mask)
#elif (SLN_MIC_COUNT==2)
#define I2S_MIC_CHANNEL_MASK (kSAI_Channel0Mask | kSAI_Channel1Mask)
#elif (SLN_MIC_COUNT==3)
#define I2S_MIC_CHANNEL_MASK (kSAI_Channel0Mask | kSAI_Channel1Mask | kSAI_Channel2Mask)
#elif (SLN_MIC_COUNT==4)
#define I2S_MIC_CHANNEL_MASK (kSAI_Channel0Mask | kSAI_Channel1Mask | kSAI_Channel2Mask | kSAI_Channel3Mask)
#endif /* SLN_MIC_COUNT */

#define SLN_MIC1_POS_X -26
#define SLN_MIC1_POS_Y 0

#define SLN_MIC2_POS_X 26
#define SLN_MIC2_POS_Y 0

#define SLN_MIC3_POS_X 2
#define SLN_MIC3_POS_Y 48

#else
#error "MICS_TYPE should be set to either 1 (MICS_PDM) or 2 (MICS_I2S)"
#endif /* MICS_TYPE */

/*******************************************************************************
 * PCM Stream Sample Definitions
 ******************************************************************************/

#define PCM_SAMPLE_SIZE_BYTES (2U)
#define PCM_SAMPLE_RATE_HZ    (16000U)
#define PCM_SAMPLE_COUNT      (PCM_SINGLE_CH_SMPL_COUNT * SLN_MIC_COUNT)
#define PCM_BUFFER_COUNT      (EDMA_TCD_COUNT)

/*******************************************************************************
 * Amplifier Stream Sample Definitions
 ******************************************************************************/

#define PCM_AMP_SAMPLE_RATE_HZ (48000U)
#define PCM_AMP_SAMPLE_COUNT   (PCM_SINGLE_CH_SMPL_COUNT * PCM_AMP_SAMPLE_RATE_HZ / PCM_SAMPLE_RATE_HZ)

#define AMP_WRITE_SLOTS 4

#define PCM_AMP_DATA_SIZE_1_MS  ((PCM_AMP_SAMPLE_COUNT * PCM_SAMPLE_SIZE_BYTES) / 10)
#define PCM_AMP_DATA_SIZE_10_MS (10 * PCM_AMP_DATA_SIZE_1_MS)
#define PCM_AMP_DATA_SIZE_20_MS (20 * PCM_AMP_DATA_SIZE_1_MS)

#if USE_MQS
/* Set the loopback constant delay to 1ms. Assuming that the amplifier starts to play exactly when
 * a ping/pong event is triggered, AMP_LOOPBACK_CONST_DELAY_US is the only delay required for synchronization.
 * This value was manually calculated using usb_aec_alignment_tool. */
#define AMP_LOOPBACK_CONST_DELAY_US    2070
#define AMP_LOOPBACK_CONST_DELAY_BYTES ((AMP_LOOPBACK_CONST_DELAY_US * PCM_AMP_DATA_SIZE_1_MS) / 1000)

/* Set the loopback variable max delay to 11ms. This value represents the time delay difference between the
 * current amplifier start and previous ping/pong event.
 * Since ping/pong event is triggered once ~ 10ms, keep the maximum value to 11ms. */
#define AMP_LOOPBACK_MAX_VAR_DELAY_US    12070
#define AMP_LOOPBACK_MAX_VAR_DELAY_BYTES ((AMP_LOOPBACK_MAX_VAR_DELAY_US * PCM_AMP_DATA_SIZE_1_MS) / 1000)

/* The loopback mechanism requires extra space inside the ringbuffer to store the delay zeroes:
 * Constant  delay: ~12ms equal to AMP_LOOPBACK_CONST_DELAY_US
 * Variable delay: <11ms. This one is calculated at the beginning of a playback using LOOPBACK_GPT. */
#define AMP_LOOPBACK_MAX_DELAY_BYTES (AMP_LOOPBACK_CONST_DELAY_BYTES + AMP_LOOPBACK_MAX_VAR_DELAY_BYTES)

/* Set the loopback ringbuf size */
#define AMP_LOOPBACK_RINGBUF_SIZE ((AMP_WRITE_SLOTS * PCM_AMP_DATA_SIZE_20_MS) + AMP_LOOPBACK_MAX_DELAY_BYTES)
#endif /* USE_MQS */

/*******************************************************************************
 * Typedefs, enumerations and structures
 ******************************************************************************/

typedef int16_t pcmPingPong_t[PCM_BUFFER_COUNT][PCM_SAMPLE_COUNT];

typedef enum _pcm_event
{
    PCM_PING_EVENT  = (1 << 0),
    PCM_PONG_EVENT  = (1 << 1),
    PCM_ERROR_EVENT = (1 << 2),
} pcm_event_t;

typedef struct _sai_mic_config
{
    I2S_Type *sai;
    uint8_t saiChannelMask;
    uint32_t saiCaptureCount;
    sai_clock_polarity_t micClockPolarity;
} sai_mic_config_t;

typedef struct _mic_task_config
{
    TaskHandle_t *thisTask;
    TaskHandle_t *processingTask;
#if ENABLE_AEC
    void (*feedbackEnable)(void);
    void (*feedbackDisable)(void);
#if USE_MQS
    ringbuf_t *loopbackRingBuffer;
    SemaphoreHandle_t loopbackMutex;
    void (*updateTimestamp)(uint32_t);
    uint32_t (*getTimestamp)(void);
#endif /* USE_MQS */
#endif /* ENABLE_AEC */
} mic_task_config_t;

typedef struct _sln_mic_handle
{
    edma_tcd_t dmaTcd[EDMA_TCD_COUNT]; /* This structure is same as TCD register which is described in reference manual */
    sai_mic_config_t *config;          /* Microphone configuration */
    DMA_Type *dma;                     /* DMA interface */
    uint32_t dmaChannel;               /* DMA channel */
    uint32_t dmaIrqNum;                /* DMA interrupt number */
    uint8_t dmaRequest;                /* DMA request */
    EventGroupHandle_t eventGroup;     /* Event group handler */
    EventBits_t pongFlag;
    EventBits_t pingFlag;
    EventBits_t errorFlag;
    uint32_t pingPongTracker;
    uint32_t *pingPongBuffer[EDMA_TCD_COUNT];
#if ENABLE_AEC
#if USE_MQS
    void (*pdmMicUpdateTimestamp)(void);
#endif /* USE_MQS */
#endif /* ENABLE_AEC */
} sln_mic_handle_t;

#endif /* SLN_MIC_CONFIG_H_ */
