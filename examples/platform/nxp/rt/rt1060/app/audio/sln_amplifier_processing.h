/*
 * Copyright 2022, 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SLN_AMPLIFIER_PROCESSING_H_
#define SLN_AMPLIFIER_PROCESSING_H_

#include "stdint.h"
#include "sln_mic_config.h"

#if ENABLE_AEC
/**
 * @brief Retrieve amplifier's data from its ring buffer, down sample the data and copy it to the output buffer.
 *        In case amplifier's ring buffer is empty, schedule future clears of the output buffer.
 *
 * @param s_taskConfig Pointer to the structure containing information about current audio chain.
 * @param buffOut Pointer to the buffer where to store PCM data.
 * @param s_pingPongTimestamp Pointer to the latest timestamp updated by mic callbacks.
 */
void SLN_AMP_GetAmpStream(mic_task_config_t *s_taskConfig, int16_t *buffOut, volatile uint32_t *s_pingPongTimestamp);
#endif /* ENABLE_AEC */

/**
 * @brief Apply FIR Low Pass Filter on amp differential data.
 *        Differential data means that Filter should skip every second sample.
 *
 * @param samples Pointer to the buffer containing 2B samples of data.
 * @param samplesCnt Number of samples in the buffer.
 */
void SLN_AMP_FirLowPassFilterForAMP(int16_t *samples, uint32_t samplesCnt);

#endif /* SLN_AMPLIFIER_PROCESSING_H_ */
