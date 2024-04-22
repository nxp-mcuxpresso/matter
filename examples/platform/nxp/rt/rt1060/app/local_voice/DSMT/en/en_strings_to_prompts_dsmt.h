/*
 * Copyright 2023-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DSMT_EN_EN_STRINGS_TO_PROMPTS_DSMT_H_
#define DSMT_EN_EN_STRINGS_TO_PROMPTS_DSMT_H_

#if ENABLE_DSMT_ASR

#include "sln_flash_files.h"

const void * const prompts_ww_en[] = {
    AUDIO_WW_DETECTED, // "Hey NXP"
    AUDIO_WW_DETECTED, // "Hey NXP ^1"
    AUDIO_WW_DETECTED, // "Hey NXP ^2"
    AUDIO_WW_DETECTED, // "Hey NXP ^3"
    AUDIO_WW_DETECTED, // "Hey NXP ^4"
};

const void * const prompts_smart_home_en[] = {
    AUDIO_OK_EN,    // "Turn on the lights"
    AUDIO_OK_EN,    // "Turn on the lights ^1"
    AUDIO_OK_EN,    // "Turn on the lights ^2"
    AUDIO_OK_EN,    // "Turn on the lights ^3"
    AUDIO_OK_EN,    // "Turn on the lights ^4"
    AUDIO_OK_EN,    // "Turn on the lights ^5"
    AUDIO_OK_EN,    // "Turn off the lights"
    AUDIO_OK_EN,    // "Turn off the lights ^1"
    AUDIO_OK_EN,    // "Turn off the lights ^2"
    AUDIO_OK_EN,    // "Open the window"
    AUDIO_OK_EN,    // "Open the window ^1"
    AUDIO_OK_EN,    // "Open the window ^2"
    AUDIO_OK_EN,    // "Close the window"
    AUDIO_OK_EN,    // "Close the window ^1"
    AUDIO_OK_EN,    // "Close the window ^2"
    AUDIO_OK_EN,    // "Close the window ^3"
    AUDIO_OK_EN,    // "Close the window ^4"
    AUDIO_OK_EN,    // "Close the window ^5"
    AUDIO_OK_EN,    // "Make it brighter"
    AUDIO_OK_EN,    // "Make it darker"
};

#endif /* ENABLE_DSMT_ASR */
#endif /* DSMT_EN_EN_STRINGS_TO_PROMPTS_DSMT_H_ */
