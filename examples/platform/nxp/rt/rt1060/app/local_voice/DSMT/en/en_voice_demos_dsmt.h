/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DSMT_EN_EN_VOICE_DEMOS_DSMT_H_
#define DSMT_EN_EN_VOICE_DEMOS_DSMT_H_

#if ENABLE_DSMT_ASR

#include "sln_voice_demo.h"
#include "en_strings_dsmt.h"
#include "en_strings_to_actions_dsmt.h"
#include "en_strings_to_prompts_dsmt.h"
#include "stddef.h"

extern unsigned int en_model_begin;

const sln_voice_demo_t demo_smart_home_en =
{
        ww_en,
        cmd_smart_home_en,
        actions_ww_en,
        actions_smart_home_en,
        prompts_ww_en,
        prompts_smart_home_en,
        AUDIO_SMART_HOME_MULTILINGUAL_EN,
        NUM_ELEMENTS(ww_en),
        NUM_ELEMENTS(cmd_smart_home_en),
        &en_model_begin,
        ASR_ENGLISH,
        ASR_CMD_SMART_HOME,
        LANG_STR_EN,
        DEMO_STR_SMART_HOME,
};

#endif /* ENABLE_DSMT_ASR */
#endif /* DSMT_EN_EN_VOICE_DEMOS_DSMT_H_ */
