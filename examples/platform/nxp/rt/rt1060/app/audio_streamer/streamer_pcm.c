/*
 * Copyright 2018-2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_STREAMER

#include "osa_common.h"

#include "board.h"
#include "sln_mic_config.h"
#include "sln_amplifier.h"
#include "streamer_pcm.h"

static pcm_rtos_t pcmHandle = {0};

void streamer_pcm_init(void)
{
    status_t ret;

    /* Set SLN_AMP max number of empty blocks which will be used for correct
     * updating of SLN_AMP's state (kSlnAmpIdle or kSlnAmpStreamer) */
    pcmHandle.emptyBlock = SAI_XFER_QUEUE_SIZE;

    ret = SLN_AMP_Init(&pcmHandle.emptyBlock);
    if (ret != kStatus_Success)
    {
        configPRINTF(("SLN_AMP_Init failed!\r\n"));
    }
}

pcm_rtos_t *streamer_pcm_open(uint32_t num_buffers)
{
    if (num_buffers != SAI_XFER_QUEUE_SIZE)
    {
        configPRINTF(("Error. num_buffers(%d) differs from SAI_XFER_QUEUE_SIZE\r\n", num_buffers));
    }
    else
    {
        pcmHandle.emptyBlock = pcmHandle.numBlocks = num_buffers;
    }

    return &pcmHandle;
}

void streamer_pcm_start(pcm_rtos_t *pcm)
{
    /* Interrupts already enabled - nothing to do.
     * App/streamer can begin writing data to SAI. */
}

void streamer_pcm_clean(pcm_rtos_t *pcm)
{
    /* Stop playback. This will flush the SAI transmit buffers. */
    SLN_AMP_AbortWrite();

    configPRINTF(("SAI DMA transfer aborted, emptyBlock=%d\r\n", pcm->emptyBlock));

    /* Reset block index */
    pcm->emptyBlock = pcm->numBlocks;
}

void streamer_pcm_close(pcm_rtos_t *pcm)
{
    uint8_t waitLoop = STREAMER_PCM_CLOSE_TIMEOUT_MSEC;
    /* Wait (up to 100ms) until the queue is emptied. */
    while ((pcm->emptyBlock < pcm->numBlocks) && (waitLoop > 0))
    {
        osa_time_delay(1);
        waitLoop--;
    }
    /* If still full DMA might be stalled - clean and restart - VOIS-890 */
    if (waitLoop == 0)
    {
        streamer_pcm_clean(pcm);
    }
}

int streamer_pcm_write(pcm_rtos_t *pcm, uint8_t *data, uint32_t size)
{
    status_t ret;

    uint8_t waitLoop = STREAMER_PCM_WRITE_TIMEOUT_MSEC;
    /* Block (up to 30ms) until there is free space to write. */
    while ((pcm->emptyBlock == 0) && (waitLoop > 0))
    {
        osa_time_delay(1);
        waitLoop--;
    }
    /* If still full DMA might be stalled - clean and restart - VOIS-890 */
    if (waitLoop == 0)
    {
        streamer_pcm_clean(pcm);
    }

    pcm->saiTx.dataSize = size;
    pcm->saiTx.data     = data;

    /* Ensure write size is a multiple of 32, otherwise EDMA will assert
     * failure. Round down for the last chunk of a file/stream. */
    if (size % 32)
    {
        pcm->saiTx.dataSize = size - (size % 32);
    }

    ret = SLN_AMP_WriteStreamerNoWait(pcm->saiTx.data, pcm->saiTx.dataSize);
    if (ret == kStatus_Success)
    {
        pcm->emptyBlock--;
    }
    else
    {
        configPRINTF(("pcm_write failed, sai_error=%d, emptyBlocks=%d\r\n", ret, pcm->emptyBlock));
        return 1;
    }

    return 0;
}

/*
 * Some of the functions below are currently unused. This may change in later iterations.
 */
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
/*! @brief Map an integer sample rate (Hz) to internal SAI enum */
static sai_sample_rate_t _pcm_map_sample_rate(uint32_t sample_rate)
{
    switch (sample_rate)
    {
        case 8000:
            return kSAI_SampleRate8KHz;
        case 16000:
            return kSAI_SampleRate16KHz;
        case 24000:
            return kSAI_SampleRate24KHz;
        case 32000:
            return kSAI_SampleRate32KHz;
        case 44100:
            return kSAI_SampleRate44100Hz;
        case 48000:
        default:
            return kSAI_SampleRate48KHz;
    }
}

/*! @brief Map an integer bit width (bits) to internal SAI enum */
static sai_word_width_t _pcm_map_word_width(uint32_t bit_width)
{
    switch (bit_width)
    {
        case 8:
            return kSAI_WordWidth8bits;
        case 16:
            return kSAI_WordWidth16bits;
        case 24:
            return kSAI_WordWidth24bits;
        case 32:
            return kSAI_WordWidth32bits;
        default:
            return kSAI_WordWidth16bits;
    }
}

/*! @brief Map an integer number of channels to internal SAI enum */
static sai_mono_stereo_t _pcm_map_channels(uint8_t num_channels)
{
    if (num_channels >= 2)
        return kSAI_Stereo;
    else
        return kSAI_MonoRight;
}
#pragma GCC diagnostic pop
#endif /* GNUC pragma for Unused Functions */

int streamer_pcm_setparams(pcm_rtos_t *pcm, uint32_t sample_rate, uint32_t bit_width, uint8_t num_channels)
{
    int ret = 1;

    if (NULL != pcm)
    {
        pcm->sample_rate  = sample_rate;
        pcm->bit_width    = bit_width;
        pcm->num_channels = num_channels;
        ret               = 0;
    }

    return ret;
}

void streamer_pcm_getparams(pcm_rtos_t *pcm, uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels)
{
#if 1
    *sample_rate  = 48000;
    *bit_width    = 16;
    *num_channels = 1;
#else
    *sample_rate  = pcm->sample_rate;
    *bit_width    = pcm->bit_width;
    *num_channels = pcm->num_channels;
#endif
}

int streamer_pcm_mute(pcm_rtos_t *pcm, bool mute)
{
    SLN_AMP_SetVolume(0);
    return 0;
}

int streamer_pcm_set_volume(uint32_t volume)
{
    SLN_AMP_SetVolume(volume);
    return 0;
}

#endif /* ENABLE_STREAMER */
