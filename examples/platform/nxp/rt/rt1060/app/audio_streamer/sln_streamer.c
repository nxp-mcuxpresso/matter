/*
 * Copyright 2018-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_STREAMER

#include "osa_common.h"
#include "fsl_common.h"

#include "sln_streamer.h"
#include "streamer_pcm.h"
#include "af_error.h"
#include "sln_flash_fs_ops.h"

#define APP_STREAMER_MSG_QUEUE     "app_queue"
#define STREAMER_TASK_NAME         "Streamer"
#define STREAMER_MESSAGE_TASK_NAME "StreamerMessage"

#define STREAMER_TASK_STACK_SIZE         1536
#define STREAMER_MESSAGE_TASK_STACK_SIZE 512
#define STREAMER_DEFAULT_VOLUME          60

/*! @brief local OPUS file internal structure definition */
typedef struct _streamer_local_file
{
    uint32_t len;
    uint32_t offset;
    char *filename;
} streamer_local_file_t;

/* Declaration of OPUS file used for playing OPUS audio locally */
static streamer_local_file_t local_active_file_desc;

static uint32_t _SLN_STREAMER_ReadLocalFile(uint8_t *buffer, uint32_t size);

/* internal mutex for accessing the audio buffer */
static OsaMutex audioBufMutex;

/*!
 * @brief Streamer task for communicating messages
 *
 * This function is the entry point of a task that is manually created by
 * STREAMER_Create.  It listens on a message queue and receives status updates
 * about errors, audio playback state and position.  The application can make
 * use of this data.
 *
 * @param arg Data to be passed to the task
 */
static void SLN_STREAMER_MessageTask(void *arg)
{
    OsaMq mq;
    STREAMER_MSG_T msg;
    streamer_handle_t *handle;
    bool exit_thread = false;
    int ret;

    handle = (streamer_handle_t *)arg;

    configPRINTF(("[STREAMER] Message Task started\r\n"));

    ret = osa_mq_open(&mq, APP_STREAMER_MSG_QUEUE, STREAMER_MSG_SIZE, true);
    if (ERRCODE_NO_ERROR != ret)
    {
        configPRINTF(("osa_mq_open failed: %d\r\n", ret));
        return;
    }

    do
    {
        ret = osa_mq_receive(&mq, (void *)&msg, STREAMER_MSG_SIZE, 0, NULL);
        if (ret != ERRCODE_NO_ERROR)
        {
            configPRINTF(("osa_mq_receiver error: %d\r\n", ret));
            continue;
        }

        switch (msg.id)
        {
            case STREAM_MSG_ERROR:
                configPRINTF(("STREAM_MSG_ERROR %d\r\n", msg.errorcode));

                if (handle->pvExceptionCallback != NULL)
                {
                    handle->pvExceptionCallback();
                }

                break;
            case STREAM_MSG_EOS:
//                configPRINTF(("STREAM_MSG_EOS\r\n"));
                xSemaphoreTake(audioBufMutex, portMAX_DELAY);
                if (local_active_file_desc.filename != NULL)
                {
                    /* Stop the streamer so we don't send speaker closed
                     * Don't flush the streamer just in case there is pending data */
                    /* add extra delay to mask prompt tail */
                    vTaskDelay(100);
                    handle->audioPlaying = false;
                    streamer_set_state(handle->streamer, 0, STATE_NULL, true);
                    local_active_file_desc.filename = NULL;

                    /* power off the amp */
                    GPIO_PinWrite(GPIO2, 2, 0);
                }
                else
                {
                    /* Indicate to other software layers that playing has ended. */
                    handle->eos = true;
                }
                xSemaphoreGive(audioBufMutex);
                break;
            case STREAM_MSG_UPDATE_POSITION:
//                configPRINTF(("STREAM_MSG_UPDATE_POSITION\r\n"));
//                configPRINTF(("  position: %d ms\r\n", msg.event_data));
                break;
            case STREAM_MSG_CLOSE_TASK:
                configPRINTF(("STREAM_MSG_CLOSE_TASK\r\n"));
                exit_thread = true;
                break;
            default:
                break;
        }

    } while (!exit_thread);

    osa_mq_close(&mq);
    osa_mq_destroy(APP_STREAMER_MSG_QUEUE);
}

int SLN_STREAMER_Read(uint8_t *data, uint32_t size)
{
    volatile uint32_t bytes_read = 0;

    /* The streamer reads blocks of data but the decoder only decodes frames
       This means there could be incomplete frames the decoder has got but the
       application could interrupt before sending the full frame.

       The following ensures that only full OPUS frames are given to the streamer
       so we can ensure a transition without Error 252*/
    size = (size - (size % STREAMER_PCM_OPUS_FRAME_SIZE));

    xSemaphoreTake(audioBufMutex, portMAX_DELAY);

    /* If a sound is being played at the moment, read frames from that source */
    if ((local_active_file_desc.filename != NULL) && (data != NULL))
    {
        if (local_active_file_desc.len == -1)
        {
            local_active_file_desc.filename = NULL;
        }
        else if (local_active_file_desc.len == 0)
        {
            local_active_file_desc.len = -1;
        }
        else if (local_active_file_desc.len != 0)
        {
            bytes_read = _SLN_STREAMER_ReadLocalFile(data, size);
        }
    }

    xSemaphoreGive(audioBufMutex);

    if (bytes_read != size)
    {
        /* Don't print this warning under normal conditions.
         * * Excessive calls can clog the logging and hide other messages. */
        /*
               configPRINTF(("[STREAMER WARN] read underrun: size: %d, read: %d\r\n", size, bytes_read));
        */

        bytes_read = (bytes_read - (bytes_read % STREAMER_PCM_OPUS_FRAME_SIZE));
    }

    return bytes_read;
}

uint32_t SLN_STREAMER_SetLocalSound(streamer_handle_t *handle, char *filename)
{
    uint32_t status = kStatus_Fail;
    uint32_t statusFlash     = 0;
    uint32_t len = 0;

    xSemaphoreTake(audioBufMutex, portMAX_DELAY);

    if (local_active_file_desc.filename == NULL)
    {
        statusFlash = sln_flash_fs_ops_read((const char *)filename, NULL, 0, &len);

        if (statusFlash != SLN_FLASH_FS_OK)
        {
            configPRINTF(("Failed reading audio file info from flash memory.\r\n"));
        }
        else
        {
            local_active_file_desc.filename = filename;
            local_active_file_desc.len  = len;
            local_active_file_desc.offset = 0;
            status = kStatus_Success;
        }
    }

    xSemaphoreGive(audioBufMutex);

    return status;
}


bool SLN_STREAMER_IsPlaying(streamer_handle_t *handle)
{
    return handle->audioPlaying;
}

void SLN_STREAMER_SetVolume(uint32_t volume)
{
    /* Protect against an uninitialized volume */
    if (volume == -1)
    {
        volume = STREAMER_DEFAULT_VOLUME;
    }

    streamer_pcm_set_volume(volume);
}

void SLN_STREAMER_Start(streamer_handle_t *handle)
{
//    configPRINTF(("[STREAMER] start playback\r\n"));
    /* power on the amp */
    GPIO_PinWrite(GPIO2, 2, 1);
    vTaskDelay(150);

    handle->audioPlaying = true;
    streamer_set_state(handle->streamer, 0, STATE_PLAYING, true);
}


uint32_t SLN_STREAMER_Stop(streamer_handle_t *handle)
{
    uint32_t flushedSize = 0;

//    configPRINTF(("[STREAMER] stop playback\r\n"));

    /* add extra delay to mask prompt tail */
    vTaskDelay(100);
    handle->audioPlaying = false;
    streamer_set_state(handle->streamer, 0, STATE_NULL, true);

    /* power off the amp */
    GPIO_PinWrite(GPIO2, 2, 0);

    /* Flush input ringbuffer. */
    xSemaphoreTake(audioBufMutex, portMAX_DELAY);

    local_active_file_desc.filename = NULL;
    local_active_file_desc.len  = -1;

    xSemaphoreGive(audioBufMutex);

    return flushedSize;
}

void SLN_STREAMER_Pause(streamer_handle_t *handle)
{
//    configPRINTF(("[STREAMER] pause playback\r\n"));

    /* add extra delay to mask prompt tail */
    vTaskDelay(100);
    handle->audioPlaying = false;
    streamer_set_state(handle->streamer, 0, STATE_PAUSED, true);

    /* power off the amp */
    GPIO_PinWrite(GPIO2, 2, 0);
}

status_t SLN_STREAMER_Create(streamer_handle_t *handle, streamer_decoder_t decoder)
{
    STREAMER_CREATE_PARAM params;
    ELEMENT_PROPERTY_T prop;
    OsaThread msg_thread;
    OsaThreadAttr thread_attr;
    int ret;

    audioBufMutex = xSemaphoreCreateMutex();
    if (!audioBufMutex)
    {
        return kStatus_Fail;
    }

    /* Create message process thread */
    osa_thread_attr_init(&thread_attr);
    osa_thread_attr_set_name(&thread_attr, STREAMER_MESSAGE_TASK_NAME);
    osa_thread_attr_set_stack_size(&thread_attr, STREAMER_MESSAGE_TASK_STACK_SIZE);
    ret = osa_thread_create(&msg_thread, &thread_attr, SLN_STREAMER_MessageTask, (void *)handle);
    osa_thread_attr_destroy(&thread_attr);
    if (ERRCODE_NO_ERROR != ret)
    {
        return kStatus_Fail;
    }

    /* Create streamer */
    strcpy(params.out_mq_name, APP_STREAMER_MSG_QUEUE);
    params.stack_size    = STREAMER_TASK_STACK_SIZE;
    params.pipeline_type = STREAM_PIPELINE_NETBUF;
    params.task_name     = STREAMER_TASK_NAME;
    params.in_dev_name   = "";
    params.out_dev_name  = "";

    handle->streamer = streamer_create(&params);
    if (!handle->streamer)
    {
        return kStatus_Fail;
    }

    prop.prop = PROP_NETBUFSRC_SET_CALLBACK;
    prop.val  = (uintptr_t)SLN_STREAMER_Read;

    streamer_set_property(handle->streamer, prop, true);

    prop.prop = PROP_DECODER_DECODER_TYPE;
    if (decoder == DECODER_OPUS)
    {
        prop.val = DECODER_TYPE_OPUS;
    }
    else if (decoder == DECODER_MP3)
    {
        prop.val = DECODER_TYPE_MP3;
    }
    else
    {
        return kStatus_Fail;
    }

    streamer_set_property(handle->streamer, prop, true);

    handle->audioPlaying = false;
    handle->eos          = false;

    return kStatus_Success;
}

void SLN_STREAMER_Destroy(streamer_handle_t *handle)
{
    streamer_destroy(handle->streamer);

    vSemaphoreDelete(audioBufMutex);
}

void SLN_STREAMER_Init(void)
{
    /* Initialize OSA*/
    osa_init();

    /* Initialize logging */
    init_logging();

    add_module_name(LOGMDL_STREAMER, "STREAMER");

    /* Uncomment below to turn on full debug logging for the streamer. */
    // set_debug_module(0xffffffff);
    // set_debug_level(0xff);

    /* Initialize streamer PCM management library. */
    streamer_pcm_init();
}

static uint32_t _SLN_STREAMER_ReadLocalFile(uint8_t *buffer, uint32_t size)
{
    uint32_t read_size   = size;
    uint32_t statusFlash = 0;

    /* If the requested size is greater than what's left */
    if (size >= local_active_file_desc.len)
    {
        read_size = local_active_file_desc.len;
    }

    statusFlash = sln_flash_fs_ops_read((const char *)local_active_file_desc.filename, buffer, local_active_file_desc.offset, &read_size);
    if (statusFlash != SLN_FLASH_FS_OK)
    {
        configPRINTF(("Failed reading audio file from flash memory.\r\n"));
        read_size = 0;
    }
    else
    {
        if (0 != (read_size - local_active_file_desc.len))
        {
            local_active_file_desc.offset += read_size;
        }

        local_active_file_desc.len -= read_size;

    }

    return read_size;
}

#endif /* ENABLE_STREAMER */
