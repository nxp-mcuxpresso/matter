/*
 * Copyright 2023-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef VIT_EN_EN_STRINGS_TO_PROMPTS_VIT_H_
#define VIT_EN_EN_STRINGS_TO_PROMPTS_VIT_H_

#if ENABLE_VIT_ASR

#include "sln_flash_files.h"

const void * const prompts_ww_en[] = {
    AUDIO_WW_DETECTED, // "Hey NXP"
};

const void * const prompts_smart_home_en[] = {
    AUDIO_OK_EN,    // "Turn on the lights"
    AUDIO_OK_EN,    // "Turn off the lights"
    AUDIO_OK_EN,    // "Open the window"
    AUDIO_OK_EN,    // "Close the window"
    AUDIO_OK_EN,    // "Make it brighter"
    AUDIO_OK_EN,    // "Make it darker"
};

#endif /* ENABLE_VIT_ASR */
#endif /* VIT_EN_EN_STRINGS_TO_PROMPTS_VIT_H_ */
