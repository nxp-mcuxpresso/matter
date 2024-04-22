/*
 * Copyright 2018-2022, 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PDM_TO_PCM_TASK_H_
#define _PDM_TO_PCM_TASK_H_

#include "sln_mic_config.h"

#if (MICS_TYPE == MICS_PDM)

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"
#include "fsl_common.h"

#if ENABLE_AEC
#if USE_MQS
#include "semphr.h"
#include "ringbuffer.h"
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/*!
 * @addtogroup pdm_to_pcm_task
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum __pdm_pcm_input_event
{
    MIC1_PING_EVENT      = (1 << 0U),
    MIC2_PING_EVENT      = (1 << 1U),
    MIC1_PONG_EVENT      = (1 << 2U),
    MIC2_PONG_EVENT      = (1 << 3U),
    MIC3_PONG_EVENT      = (1 << 4U),
    MIC3_PING_EVENT      = (1 << 5U),
    AMP_REFERENCE_SIGNAL = (1 << 6U),
    PDM_ERROR_FLAG       = (1 << 7U),
    AMP_ERROR_FLAG       = (1 << 8U),
} pdm_pcm_input_event_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Task to handle PDM to PCM conversion; and AFE barge-in capture
 *
 * @param pvParameters Optional parameters to pass into task
 */
void pdm_to_pcm_task(void *pvParameters);

/*!
 * @brief Sets pdm to pcm task configuration
 *
 * @param *config Reference to application specific configuration
 */
status_t pcm_to_pcm_set_config(mic_task_config_t *config);

/*!
 * @brief Gets pdm to pcm task configuration
 *
 * @return Reference to application specific configuration
 */
mic_task_config_t *pcm_to_pcm_get_config(void);

/*!
 * @brief Sets task handle for PDM to PCM task
 *
 * @param *handle Reference to task handle
 */
void pdm_to_pcm_set_task_handle(TaskHandle_t *handle);

/*!
 * @brief Set task handle for passing notifications to audio processing task
 *
 * @param *handle Reference to task handle
 */
void pdm_to_pcm_set_audio_proces_task_handle(TaskHandle_t *handle);

/*!
 * @brief Get task handle for PDM to PCM task
 *
 * @returns Task handle stored for PDM to PCM task
 */
TaskHandle_t pdm_to_pcm_get_task_handle(void);

#if ENABLE_AEC
#if USE_MQS

/*!
 * @brief Set the loopback mutex.
          Used for exclusive access to the loopback ringbuffer.
 *
 * @param mutex    Pointer to the mutex handler
 */
void pdm_to_pcm_set_loopback_mutex(SemaphoreHandle_t *mutex);

/*!
 * @brief Set the loopback ringbuffer.
          Used for setting the pointer to the ringbuffer.
 *
 * @param *ring_buf Reference to the ring buffer for amp loopback
 */
void pdm_to_pcm_set_loopback_ring_buffer(ringbuf_t *ring_buf);
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/*!
 * @brief Get output buffer from amp loopback; to pass onto consuming task
 *
 * @returns 16-bit pointer to amp loopback buffer
 */
int16_t *pdm_to_pcm_get_amp_output(void);

/*!
 * @brief Get pointer to PCM output for consuming task
 *
 * @returns 16-bit pointer to microphone PCM data
 */
int16_t *pdm_to_pcm_get_pcm_output(void);

#if USE_NEW_PDM_PCM_LIB

#else
/*!
 * @brief Formats PCM data based on type of triggered event and desired pattern of channels
 *
 * @param *pcmBuffer A single channel PCM buffer to merge into output stream
 * @param micEvent Ping or pong event for given microphone
 * @param pcmFormat Channel format type 1 for LRLR, 0 for LLRR
 */
void pdm_to_pcm_stream_formatter(int16_t *pcmBuffer, pdm_pcm_input_event_t micEvent, uint8_t pcmFormat);

/*!
 * @brief Sets the microphone gain for all mic streams
 *
 * @param u8Gain The gain shift in all microphones
 */
int32_t pdm_to_pcm_set_gain(uint8_t u8Gain);
#endif /* USE_NEW_PDM_PCM_LIB */

/*!
 * @brief Turns microphones off by disabling DMA / SAI interfaces settings
 */
void pdm_to_pcm_mics_off(void);

/*!
 * @brief Turns microphones on by enabling DMA / SAI interfaces settings
 */
void pdm_to_pcm_mics_on(void);

bool pdm_to_pcm_get_mics_state(void);
#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* (MICS_TYPE == MICS_PDM) */

#endif /* _PDM_TO_PCM_TASK_H_ */

/*******************************************************************************
 * API
 ******************************************************************************/
