/*
 * Copyright 2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SLN_I2S_MIC_PROCESSING_H_
#define SLN_I2S_MIC_PROCESSING_H_

#include "sln_mic_config.h"

#if (MICS_TYPE == MICS_I2S)

#include "stdint.h"

/**
 * @brief Process microphones samples.
 *        Samples should be arranged in the following format: mic1 mic2 .. micN, mic1 mic2 .. micN, etc.
 *        Re-arange the samples in the following format: mic1 mic1 .. mic1, mic2 mic2 .. mic2, micN micN .. micN.
 *        Apply High Pass Filter to center audio amplitudes to zero.
 *
 * @param in Pointer to the buffer containing the samples.
 * @param out Pointer where to store processed data.
 */
void I2S_MIC_ProcessMicStream(uint8_t *in, int16_t *out);

#endif /* (MICS_TYPE == MICS_I2S) */

#endif /* SLN_I2S_MIC_PROCESSING_H_ */
