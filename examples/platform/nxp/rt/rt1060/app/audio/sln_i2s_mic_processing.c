/*
 * Copyright 2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sln_mic_config.h"

#if (MICS_TYPE == MICS_I2S)

#include "stdint.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* High Pass Filter configuration for mics */
#define HPF_CUT_OFF_HZ 60
#define HPF_SAMPLE_RATE PCM_SAMPLE_RATE_HZ

/* Amplifier factor for mics */
#define I2S_MIC_AMP_FACTOR 6

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Buffer used by HPF processing on mic raw data. */
static float s_micFilteredArray[I2S_MIC_RAW_FRAME_SAMPLES_COUNT] = {0};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void I2S_MIC_MicHighPassFilter(int16_t *samples, uint32_t samplesCnt, uint8_t micId);

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief Apply High Pass Filter on mic data.
 *
 * @param samples Pointer to the buffer containing 2B samples of data.
 * @param samplesCnt Number of samples in the buffer.
 * @param micId Microphone id (in order to use the appropriate previously saved fields).
 */
static void I2S_MIC_MicHighPassFilter(int16_t *samples, uint32_t samplesCnt, uint8_t micId)
{
    static float last_element_record[SLN_MIC_COUNT] = {0};
    static float last_element_filter[SLN_MIC_COUNT] = {0};
    static uint8_t first_time[SLN_MIC_COUNT]        = {0};

    float RC    = 1.0 / (HPF_CUT_OFF_HZ * 2 * 3.14);
    float dt    = 1.0 / HPF_SAMPLE_RATE;
    float alpha = RC / (RC + dt);

    memset(s_micFilteredArray, 0, sizeof(s_micFilteredArray));

    if (first_time[micId] == 0)
    {
        first_time[micId] = 1;
        s_micFilteredArray[0]  = samples[0];
    }
    else
    {
        s_micFilteredArray[0] = alpha * (last_element_filter[micId] + samples[0] - last_element_record[micId]);
    }

    for (uint32_t i = 1; i < samplesCnt; i++)
    {
        s_micFilteredArray[i] = alpha * (s_micFilteredArray[i - 1] + samples[i] - samples[i - 1]);
    }

    last_element_record[micId] = samples[samplesCnt - 1];
    last_element_filter[micId] = s_micFilteredArray[samplesCnt - 1];

    for (uint32_t i = 0; i < samplesCnt; i++)
    {
        samples[i] = (int16_t)(s_micFilteredArray[i]);
    }
}

/*******************************************************************************
 * API
 ******************************************************************************/

void I2S_MIC_ProcessMicStream(uint8_t *in, int16_t *out)
{
    uint8_t micId;
    uint32_t i;

    int16_t *currentMicOut = NULL;

#if (I2S_MIC_RAW_SAMPLE_SIZE==2)
    int16_t *inBuff = (int16_t *)in;
#elif (I2S_MIC_RAW_SAMPLE_SIZE==4)
    int32_t *inBuff = (int32_t *)in;
#endif /* I2S_MIC_RAW_SAMPLE_SIZE */

    for (micId = 0; micId < SLN_MIC_COUNT; micId++)
    {
        currentMicOut = &out[micId * PCM_SINGLE_CH_SMPL_COUNT];

        for (i = 0; i < PCM_SINGLE_CH_SMPL_COUNT; i++)
        {
#if (I2S_MIC_RAW_SAMPLE_SIZE==2)
            currentMicOut[i] = (inBuff[i * SLN_MIC_COUNT + micId] * I2S_MIC_AMP_FACTOR);
#elif (I2S_MIC_RAW_SAMPLE_SIZE==4)
            currentMicOut[i] = ((inBuff[i * SLN_MIC_COUNT + micId] * I2S_MIC_AMP_FACTOR) >> 16);
#endif /* I2S_MIC_RAW_SAMPLE_SIZE */
        }

        /* Apply High Pass Filter to center audio amplitudes to zero */
        I2S_MIC_MicHighPassFilter(currentMicOut, PCM_SINGLE_CH_SMPL_COUNT, micId);
    }
}

#endif /* (MICS_TYPE == MICS_I2S) */
