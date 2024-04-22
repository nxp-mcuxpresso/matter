/*
 * Copyright 2019-2020, 2022 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "fsl_codec_adapter.h"
#include "fsl_codec_common.h"
#include "math.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define HAL_MQS_PLAY_CAPABILITY (kCODEC_SupportPlayChannelLeft0 | kCODEC_SupportPlayChannelRight0)
#define HAL_MQS_VOLUME_CAPABILITY (kCODEC_VolumeLeft0 | kCODEC_VolumeRight0)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const codec_capability_t s_mqs_capability = {
    .codecPlayCapability = HAL_MQS_PLAY_CAPABILITY,
	.codecVolumeCapability = HAL_MQS_VOLUME_CAPABILITY
};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * brief Codec initilization.
 *
 * param handle codec handle.
 * param config codec configuration.
 * return kStatus_Success is success, else initial failed.
 */
status_t HAL_CODEC_Init(void *handle, void *config)
{
    assert((config != NULL) && (handle != NULL));

    ((mqs_config_t *)((codec_config_t *)config)->codecDevConfig)->s_AmpRxDataRingBuffer =
        ringbuf_create(((mqs_config_t *)((codec_config_t *)config)->codecDevConfig)->ringbuf_size);

    ((mqs_config_t *)((codec_config_t *)config)->codecDevConfig)->s_AmpRxDataRingBuffer->tail =
        ((mqs_config_t *)((codec_config_t *)config)->codecDevConfig)->ringbuf_delay;

    ((mqs_config_t *)((codec_config_t *)config)->codecDevConfig)->s_AmpRxDataRingBuffer->occ =
        ((mqs_config_t *)((codec_config_t *)config)->codecDevConfig)->ringbuf_delay;

    ((codec_handle_t *)handle)->codecCapability = &s_mqs_capability;

    return kStatus_Success;
}

/*!
 * brief Codec de-initilization.
 *
 * param handle codec handle.
 * return kStatus_Success is success, else de-initial failed.
 */
status_t HAL_CODEC_Deinit(void *handle)
{
    assert(handle != NULL);

    ringbuf_destroy(((mqs_config_t *)((codec_handle_t *)handle)->codecConfig->codecDevConfig)->s_AmpRxDataRingBuffer);

    return kStatus_Success;
}

/*!
 * brief set audio data format.
 *
 * param handle codec handle.
 * param mclk master clock frequency in HZ.
 * param sampleRate sample rate in HZ.
 * param bitWidth bit width.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_SetFormat(void *handle, uint32_t mclk, uint32_t sampleRate, uint32_t bitWidth)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief set audio codec module volume.
 *
 * param handle codec handle.
 * param channel audio codec play channel, can be a value or combine value of _codec_play_channel.
 * param volume volume value, support 0 ~ 100, only in multiples of 10, 0 is mute, 100 is the maximum volume value.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_SetVolume(void *handle, uint32_t playChannel, uint32_t volume)
{
    assert(handle != NULL);

    assert(volume <= 100);

    volume /= 10;

    /*
     * This is the function used for generating a nice polynomial curve for the volume levels.
     *
     *                  y = -0.0018 * x ^ 3 + 0.028 * x ^ 2
     *
     * In this case it's more like a linear function with the lower and upper ends slightly curved.
     *
     * The levels go from 0 to 1, making sure that level 1 stays low at 0.0262
     * while still being able to reach the value 1 at level 10.
     *
     * This function is called once for every volume change, so these operations shouldn't be
     * that much of a burden
     */
    ((mqs_config_t *)(((codec_handle_t *)handle)->codecConfig->codecDevConfig))->volume =
        -0.0018 * pow(volume, 3) + 0.028 * pow(volume, 2);

    return kStatus_Success;
}

/*!
 * brief set audio codec module mute.
 *
 * param handle codec handle.
 * param channel audio codec play channel, can be a value or combine value of _codec_play_channel.
 * param isMute true is mute, false is unmute.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_SetMute(void *handle, uint32_t playChannel, bool isMute)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief set audio codec module power.
 *
 * param handle codec handle.
 * param module audio codec module.
 * param powerOn true is power on, false is power down.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_SetPower(void *handle, uint32_t module, bool powerOn)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief codec set record channel.
 *
 * param handle codec handle.
 * param leftRecordChannel audio codec record channel, reference _codec_record_channel, can be a value or combine value
 of member in _codec_record_channel.
 * param rightRecordChannel audio codec record channel, reference _codec_record_channel, can be a value combine of
 member in _codec_record_channel.

 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_SetRecordChannel(void *handle, uint32_t leftRecordChannel, uint32_t rightRecordChannel)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief codec set record source.
 *
 * param handle codec handle.
 * param source audio codec record source, can be a value or combine value of _codec_record_source.
 *
 * @return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_SetRecord(void *handle, uint32_t recordSource)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief codec set play source.
 *
 * param handle codec handle.
 * param playSource audio codec play source, can be a value or combine value of _codec_play_source.
 *
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_SetPlay(void *handle, uint32_t playSource)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief codec module control.
 *
 * param handle codec handle.
 * param cmd module control cmd, reference _codec_module_ctrl_cmd.
 * param data value to write, when cmd is kCODEC_ModuleRecordSourceChannel, the data should be a value combine
 *  of channel and source, please reference macro CODEC_MODULE_RECORD_SOURCE_CHANNEL(source, LP, LN, RP, RN), reference
 *  codec specific driver for detail configurations.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_ModuleControl(void *handle, uint32_t cmd, uint32_t data)
{
    return kStatus_CODEC_NotSupport;
}
