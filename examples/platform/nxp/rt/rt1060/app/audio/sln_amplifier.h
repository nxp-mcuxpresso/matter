/*
 * Copyright 2019-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_AMPLIFIER

#ifndef _SLN_AMPLIFIER_H_
#define _SLN_AMPLIFIER_H_

#include <stdint.h>
#include <stdbool.h>

#include "sln_mic_config.h"

#if ENABLE_AEC
#include "FreeRTOS.h"
#include "event_groups.h"
#if USE_MQS
#include "semphr.h"
#include "ringbuffer.h"
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Default values for a Streamer pipeline approach:
 * A pool of 4 buffers. Each buffer has 1920 Bytes (20 ms). */
#define DEFAULT_AMP_SLOT_SIZE PCM_AMP_DATA_SIZE_20_MS
#define DEFAULT_AMP_SLOT_CNT  AMP_WRITE_SLOTS

typedef enum _sln_amp_state
{
    kSlnAmpIdle,            /* Not playing */
    kSlnAmpPlayBlocking,    /* Playing in a blocking mode (SLN_AMP_WriteAudioBlocking). */
    kSlnAmpPlayNonBlocking, /* Playing in a dedicated Amplifier task (SLN_AMP_WriteAudioNoWait). */
    kSlnAmpStreamer,        /* Playing driven by an external Streamer (SLN_AMP_WriteStreamerNoWait). */
} sln_amp_state_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * @brief  Initialize SAI and DMA for amplifier streaming.
 *         Initialize and calibrate the amplifier.
 *         If ENABLE_AEC is set, initialize the loopback mechanism (for barge-in).
 *
 * @param  extStreamerBuffsCnt Pointer to the number of available buffers for the transmission queue SAI->DMA->AMP.
 *
 * @return kStatus_Success if success.
 */
status_t SLN_AMP_Init(volatile uint8_t *extStreamerBuffsCnt);

/**
 * @brief  Play the audio in a blocking manner using an internal simple Streamer which has a pool
 *         of buffers and use them in a pipeline manner.
 *         This function will exit with success only after the whole audio is played
 *         or the play was aborted (by calling SLN_AMP_AbortWrite).
 *         If another audio is already playing, this call will return fail.
 *         NOTE: For MQS, if the data is in flash, it is mandatory to pass inFlash as true.
 *               For TFA, inFlash parameter has no effect.
 *         NOTE: For MQS, if the data is in flash, this function
 *               will need to allocate (slotSize * slotCnt) memory in RAM.
 *         NOTE: For MQS with loopback, (slotSize * slotCnt) should not
 *               exceed (AMP_WRITE_SLOTS * PCM_AMP_DATA_SIZE_20_MS).
 *
 * @param  data Pointer to the audio data stored in RAM or Flash.
 * @param  length Length of the audio data.
 * @param  slotSize Size of a chunk of audio to be scheduled to play. Example: DEFAULT_AMP_SLOT_SIZE.
 * @param  slotCnt Number of slots in the amplifier pool. Recommended: 2 or more. Example: DEFAULT_AMP_SLOT_CNT.
 * @param  inFlash Is the data stored in flash? For TFA has no effect. For MQS, provide TRUE if the data is in Flash.
 *
 * @return kStatus_Success if the audio was played or aborted (by calling SLN_AMP_AbortWrite).
 */
status_t SLN_AMP_WriteAudioBlocking(uint8_t *data, uint32_t length, uint32_t slotSize, uint8_t slotCnt, bool inFlash);

/**
 * @brief  Create Play_Audio_Async_Task task to play the audio asynchronously.
 *         Play_Audio_Async_Task will play the audio in a blocking manner (exactly as SLN_AMP_WriteAudioBlocking).
 *         After the audio is played or aborted (by calling SLN_AMP_AbortWrite), Play_Audio_Async_Task will auto delete.
 *         Play_Audio_Async_Task can be stopped in advance by calling SLN_AMP_AbortWrite.
 *         If another audio is already playing, this call will return fail.
 *         NOTE: For MQS, if the data is in flash, it is mandatory to pass inFlash as true.
 *               For TFA, inFlash parameter has no effect.
 *         NOTE: For MQS, if the data is in flash, this function
 *               will need to allocate (slotSize * slotCnt) memory in RAM.
 *         NOTE: For MQS with loopback, (slotSize * slotCnt) should not
 *               exceed (AMP_WRITE_SLOTS * PCM_AMP_DATA_SIZE_20_MS).
 *         NOTE: SLN_AMP_WriteAudioNoWait is an asynchronous function,
 *               it will exit immediately after Play_Audio_Async_Task is created
 *               (will not wait the audio to be played).
 *
 * @param  data Pointer to the audio data stored in RAM or Flash.
 * @param  length Length of the audio data.
 * @param  slotSize Size of a chunk of audio to be scheduled to play. Example: DEFAULT_AMP_SLOT_SIZE.
 * @param  slotCnt Number of slots in the amplifier pool. Recommended: 2 or more. Example: DEFAULT_AMP_SLOT_CNT.
 * @param  inFlash Is the data stored in flash? For TFA has no effect. For MQS, provide TRUE if the data is in Flash.
 *
 * @return kStatus_Success if the Play_Audio_Async_Task was created successfully.
 */
status_t SLN_AMP_WriteAudioNoWait(uint8_t *data, uint32_t length, uint32_t slotSize, uint8_t slotCnt, bool inFlash);

/**
 * @brief  Start to play the first chunk of the audio provided via parameters.
 *         If the audio data is bigger than a chunk (PCM_AMP_DMA_CHUNK_SIZE),
 *         ignore the rest of the audio.
 *         NOTE: This function is supposed to be called by an external STREAMER.
 *         NOTE: For MQS, the data should be mandatory in RAM.
 *
 * @param  data Pointer to the audio data stored in RAM.
 * @param  length Length of the audio data.
 *
 * @return kStatus_Success if the audio chunk was started to play.
 */
status_t SLN_AMP_WriteStreamerNoWait(uint8_t *data, uint32_t length);

/**
 * @brief  Stop the amplifier (if running).
 *         Stop SLN_AMP_WriteAudioBlocking. SLN_AMP_WriteAudioBlocking will exit with success.
 *         Stop and delete Play_Audio_Async_Task created by SLN_AMP_WriteAudioNoWait.
 *         Stop the audio play started SLN_AMP_WriteStreamerNoWait.
 */
void SLN_AMP_AbortWrite(void);

/**
 * @brief  Set the amplifier volume.
 *
 * @param  volume The amplifier volume from 0 to 100.
 */
void SLN_AMP_SetVolume(uint8_t volume);

/**
 * @brief  Get the amplifier TX handler.
 *
 * @return Pointer to the amplifier TX handler.
 */
void *SLN_AMP_GetAmpTxHandler(void);

/**
 * @brief  Get the current state of the Amplifier.
 *
 * @return Return one of the states from sln_amp_state_t.
 */
sln_amp_state_t SLN_AMP_GetState(void);

/*******************************************************************************
 * Loopback API
 ******************************************************************************/

#if ENABLE_AEC
#if USE_MQS

/**
 * @brief  Get loopback mutex.
           Used for exclusive access to the loopback ringbuffer.
 *
 * @return Loopback mutex handler.
 */
SemaphoreHandle_t SLN_AMP_GetLoopBackMutex(void);

/**
 * @brief  Get loopback ringbuffer.
           Used for storing and retrieving amplifier data (used for barge-in).
 *
 * @return Pointer to the ringbuffer.
 */
ringbuf_t *SLN_AMP_GetRingBuffer(void);

/**
 * @brief  Get current tick count of the LOOPBACK_GPT.
 *         Used for synchronization between the amplifier and the microphones (Ping/Pong).
 *
 * @return Number of ticks.
 */
uint32_t SLN_AMP_GetTimestamp(void);

/**
 * @brief  Set the tick count of the LOOPBACK_GPT of the last Ping/Pong event.
 *         Used for synchronization between the amplifier and the microphones (Ping/Pong).
 *
 * @param  timestamp number of ticks.
 */
void SLN_AMP_UpdateTimestamp(uint32_t timestamp);
#endif /* USE_MQS */

/**
 * @brief Enable AMP loopback mechanism.
 *
 */
void SLN_AMP_LoopbackEnable(void);

/**
 * @brief Disable AMP loopback mechanism.
 *
 */
void SLN_AMP_LoopbackDisable(void);
#endif /* ENABLE_AEC */

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _SLN_AMPLIFIER_H_ */
#endif /* ENABLE_AMPLIFIER */
