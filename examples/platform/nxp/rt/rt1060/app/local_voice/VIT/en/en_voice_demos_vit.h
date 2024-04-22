/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef VIT_EN_EN_VOICE_DEMOS_VIT_H_
#define VIT_EN_EN_VOICE_DEMOS_VIT_H_

#if ENABLE_VIT_ASR

#include "sln_voice_demo.h"
#include "en_strings_vit.h"
#include "en_strings_to_actions_vit.h"
#include "en_strings_to_prompts_vit.h"
#include "demo_definitions_vit.h"
#include "stddef.h"

#include "PL_platformTypes_CortexM.h"
#include "VIT_Model_en_SmartHome.h"

const sln_voice_demo_t demo_smart_home_en =
{
        ww_en,
        cmd_smart_home_en,
        actions_ww_en,
        actions_smart_home_en,
        prompts_ww_en,
        prompts_smart_home_en,
        AUDIO_SMART_HOME_DEMO_EN,
        NUM_ELEMENTS(ww_en),
        NUM_ELEMENTS(cmd_smart_home_en),
        (void *)VIT_Model_en_smarthome,
        ASR_ENGLISH,
        ASR_CMD_SMART_HOME,
        LANG_STR_EN,
        DEMO_STR_SMART_HOME,
};

#endif /* ENABLE_VIT_ASR */
#endif /* VIT_EN_EN_VOICE_DEMOS_VIT_H_ */
