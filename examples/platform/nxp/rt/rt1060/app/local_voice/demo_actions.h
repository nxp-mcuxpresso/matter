/*
 * Copyright 2023-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DEMO_ACTIONS_H_

#define DEMO_ACTIONS_H_

#if ENABLE_DSMT_ASR
#include "demo_actions_dsmt.h"
#elif ENABLE_VIT_ASR
#include "demo_actions_vit.h"
#endif /* ENABLE_DSMT_ASR */

typedef enum
{
    kMatterActionColorChange = 0,
    kMatterActionOnOff,
    kMatterActionBrightnessChange,
    kMatterActionBlindsControl,
} matterActionType_t;

typedef struct
{
    matterActionType_t action;
    char *command;
    char *location;
    void *value;
} matterActionStruct;

#endif /* DEMO_ACTIONS_H_ */
