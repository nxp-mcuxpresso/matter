/*
 * Copyright 2022-2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_STREAMER

#include "sln_streamer.h"
#include "FreeRTOSConfig.h"
#include "fsl_common.h"
#include "stdint.h"
#include "stdbool.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static streamer_handle_t s_streamerHandle = {0};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void LOCAL_SOUNDS_ErrorCallback(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief Error callback which will be called from the streamer
 *        to report an error.
 */
static void LOCAL_SOUNDS_ErrorCallback(void)
{
    configPRINTF(("LOCAL_SOUNDS_ErrorCallback\r\n"));
}

/*******************************************************************************
 * API
 ******************************************************************************/

status_t LOCAL_SOUNDS_InitStreamer(void)
{
    status_t ret = kStatus_Success;

    /* Initialize the Streamer */
    SLN_STREAMER_Init();

    /* Setup the exception handler callback */
    s_streamerHandle.pvExceptionCallback = &LOCAL_SOUNDS_ErrorCallback;

    ret = SLN_STREAMER_Create(&s_streamerHandle, DECODER_OPUS);
    if (ret != kStatus_Success)
    {
        configPRINTF(("STREAMER_Create failed\r\n"));
    }

    return ret;
}

status_t LOCAL_SOUNDS_PlayAudioFile(char *audioFileName, int32_t volume)
{
    status_t status = kStatus_Success;

    if (s_streamerHandle.streamer != NULL)
    {
        if ((audioFileName != NULL) && (!SLN_STREAMER_IsPlaying(&s_streamerHandle)))
        {
            status = SLN_STREAMER_SetLocalSound(&s_streamerHandle, audioFileName);
            if (kStatus_Success == status)
            {
                if (!SLN_STREAMER_IsPlaying(&s_streamerHandle))
                {
                    SLN_STREAMER_Start(&s_streamerHandle);
                    SLN_STREAMER_SetVolume(volume);
                }
                else
                {
                    configPRINTF(("[WARNING] Streamer is playing another audio.\r\n"));
                    status = kStatus_Fail;
                }
            }
            else
            {
                configPRINTF(("[WARNING] Failed to set local sound for playback.\r\n"));
            }
        }
        else
        {
            configPRINTF(("[ERROR] Offline audio pointer is NULL.\r\n"));
            status = kStatus_Fail;
        }
    }
    else
    {
        configPRINTF(("[WARNING] Streamer NOT ready\r\n"));

        status = kStatus_Fail;
    }

    return status;
}

bool LOCAL_SOUNDS_isPlaying(void)
{
    return SLN_STREAMER_IsPlaying(&s_streamerHandle);
}

#endif /* ENABLE_STREAMER */
