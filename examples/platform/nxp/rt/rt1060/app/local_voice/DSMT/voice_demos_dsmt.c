/*
 * Copyright 2023-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_DSMT_ASR

#include "demo_definitions_dsmt.h"
#include "en_voice_demos_dsmt.h"
#include "sln_voice_demo.h"

sln_voice_demo_t const * const all_voice_demos_dsmt[] = {
    &demo_smart_home_en,
    NULL // end with NULL to signal list ending
};

#endif /* ENABLE_DSMT_ASR */
