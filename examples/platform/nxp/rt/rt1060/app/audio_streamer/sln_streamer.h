/*
 * Copyright 2018-2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_STREAMER_H_
#define _SLN_STREAMER_H_

#if ENABLE_STREAMER

#include "fsl_common.h"
/* streamer library includes. */
#include "streamer_api.h"
#include "streamer_element_properties.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Maximum streamer volume */
#define MAX_STREAMER_VOLUME 100

typedef void (*tvStreamerErrorCallback)();

/*! @brief Streamer decoder algorithm values */
typedef enum
{
    DECODER_OPUS,
    DECODER_MP3
} streamer_decoder_t;

/*! @brief Streamer interface structure */
typedef struct _streamer_handle_t
{
    STREAMER_T *streamer;
    bool audioPlaying;
    bool eos;
    tvStreamerErrorCallback pvExceptionCallback;
} streamer_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the streamer interface
 *
 * This function initializes the streamer library, and initializes the PCM
 * output interface.  This function should be called before creating a streamer
 * handle.
 */
void SLN_STREAMER_Init(void);

/*!
 * @brief Create an streamer interface handle
 *
 * This function creates an streamer interface and starts a task for
 * handling media playback, and a task for sending status and error messages
 * back to the application.
 *
 * @param handle Pointer to input handle
 * @param decoder Decoder algorithm to use for audio playback
 * @return kStatus_Success on success, otherwise an error.
 */
status_t SLN_STREAMER_Create(streamer_handle_t *handle, streamer_decoder_t decoder);

/*!
 * @brief Destroy an streamer interface handle
 *
 * This function destroys an streamer interface and frees associated memory.
 *
 * @param handle Pointer to input handle
 */
void SLN_STREAMER_Destroy(streamer_handle_t *handle);

/*!
 * @brief Start audio playback for the streamer interface
 *
 * This function puts the streamer in a playing state, and begins pulling data
 * from the internal ring buffer, filled with calls to STREAMER_Start.
 *
 * @param handle Pointer to input handle
 */
void SLN_STREAMER_Start(streamer_handle_t *handle);

/*!
 * @brief Stop audio playback for the streamer interface
 *
 * This function puts the streamer in a stopped state, and ends playback from
 * the audio buffer.  The internal audio buffer is cleared of any data.
 *
 * @param handle Pointer to input handle
 * @returns Number of bytes that were still queued and got flushed
 */
uint32_t SLN_STREAMER_Stop(streamer_handle_t *handle);

/*!
 * @brief Pause audio playback for the streamer interface
 *
 * This function puts the streamer in a paused state, and ends playback from
 * the audio buffer.  It does not empty the unplayed audio buffer.
 *
 * @param handle Pointer to input handle
 */
void SLN_STREAMER_Pause(streamer_handle_t *handle);

/*!
 * @brief Check if streamer interface is playing
 *
 * This function returns true/false of the playing state for the interface
 *
 * @param handle Pointer to input handle
 * @return true if playing, false if not
 */
bool SLN_STREAMER_IsPlaying(streamer_handle_t *handle);

/*!
 * @brief Set volume for streamer playback interface
 *
 * @param volume Volume with range from 0-100
 */
void SLN_STREAMER_SetVolume(uint32_t volume);

/*!
 * @brief Read audio data from the internal audio ring buffer
 *
 * This function is called internally by the streamer (passed as a callback
 * function) to consume and process data from the ring buffer.
 *
 * @param data Pointer to buffer to copy audio data into
 * @param size Size in bytes of the buffer to fill
 * @return Number of bytes successfully read.  If this is less than the
 *         'size' parameter, an underflow has occured.
 */
int SLN_STREAMER_Read(uint8_t *data, uint32_t size);

/*!
 * @brief Sets a local file to interupt existing playback if already running
 *
 * This function sets a OPUS file to be played locally
 *
 * @param handle Streamer Handle
 * @param fileName name of the file to be played
 * @return status of function
 */
uint32_t SLN_STREAMER_SetLocalSound(streamer_handle_t *handle, char *fileName);

/*!
 * @brief Locks the internal streamer recursive mutex
 * @return Void
 */
void SLN_STREAMER_MutexLock(void);

/*!
 * @brief Unlocks the internal streamer recursive mutex
 * @return Void
 */
void SLN_STREAMER_MutexUnlock(void);

#endif /* ENABLE_STREAMER */

#if defined(__cplusplus)
}
#endif

#endif
