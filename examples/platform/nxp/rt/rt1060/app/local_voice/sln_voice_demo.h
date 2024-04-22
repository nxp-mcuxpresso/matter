/*
 * Copyright 2023-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SLN_VOICE_DEMO_H_
#define SLN_VOICE_DEMO_H_

#if ENABLE_VIT_ASR || ENABLE_DSMT_ASR

#include "stdint.h"

#define NUM_ELEMENTS(x) (sizeof(x) / sizeof(x[0]))

typedef struct _sln_voice_demo
{
    const char * const * ww_strings;
    const char * const *cmd_strings;
    int16_t const * ww_actions;
    int16_t const * cmd_actions;
    const void * const * ww_prompts;
    const void * const * cmd_prompts;
    const void *demo_prompt;
    int16_t num_ww_strings;
    int16_t num_cmd_strings;
    void *model;
    int language_id;
    int demo_id;
    const char *language_str;
    const char *demo_str;
} sln_voice_demo_t;

#endif /* ENABLE_VIT_ASR || ENABLE_DSMT_ASR */
#endif /* SLN_VOICE_DEMO_H_ */
