/*
 * Copyright 2018-2021, 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _AUDIO_PROCESSING_TASK_H_
#define _AUDIO_PROCESSING_TASK_H_

#include "stdint.h"
#include "stdbool.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Task responsible for mics audio processing and ASR engine feeding.
 *
 * @param pvParameters   pointer to task structure
 */
void audio_processing_task(void *pvParameters);

/*!
 * @brief Set the mic input buffer pointer
 *
 * @param buf   Pointer to the mic input buffer
 */
void audio_processing_set_mic_input_buffer(int16_t *buf);

/*!
 * @brief Set the amp input buffer pointer
 *
 * @param major   Pointer to the amp input buffer
 */
void audio_processing_set_amp_input_buffer(int16_t *buf);

#if ENABLE_VAD
/*!
 * @brief Set the local voice task handle
 *
 * @param major   Pointer to the task handle
 */
void audio_processing_set_local_voice_task_handle(TaskHandle_t handle);
/*!
 * @brief Force a VAD event. This aims to get the board out of low power state
 *        and resume ASR processing when some events happen.
 */
void audio_processing_force_vad_event(void);
#endif /* ENABLE_VAD */

#if ENABLE_AEC
/*!
 * @brief Set the bypass aec mode at runtime. This allows the AEC to be dynamically
 *        bypassed or not.
 *
 * @param value   true or false
 */
void audio_processing_set_bypass_aec(bool value);
/*!
 * @brief Get the bypass aec mode
 */
bool audio_processing_get_bypass_aec(void);
#endif /* ENABLE_AEC */

#if defined(__cplusplus)
}
#endif

#endif /* _AUDIO_PROCESSING_TASK_H_ */
