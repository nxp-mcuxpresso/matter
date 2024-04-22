/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_VIT_ASR

#include "demo_definitions_vit.h"
#include "sln_voice_demo.h"

#include "en_voice_demos_vit.h"

sln_voice_demo_t const * const all_voice_demos_vit[] = {
    &demo_smart_home_en,
    NULL // end with NULL to signal list ending
};

#endif /* ENABLE_VIT_ASR */
