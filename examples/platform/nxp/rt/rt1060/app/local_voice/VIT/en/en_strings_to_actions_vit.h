/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef VIT_EN_EN_STRINGS_TO_ACTIONS_VIT_H_
#define VIT_EN_EN_STRINGS_TO_ACTIONS_VIT_H_

#if ENABLE_VIT_ASR

#include "demo_actions_vit.h"
#include "stdint.h"

const int16_t actions_ww_en[] = {
    kWakeWord_Detected, // "Hey NXP"
};

const int16_t actions_smart_home_en[] = {
    kSmartHome_TurnOnTheLights,     // "Turn on the lights"
    kSmartHome_TurnOffTheLights,    // "Turn off the lights"
    kSmartHome_OpenTheWindow,       // "Open the window"
    kSmartHome_CloseTheWindow,      // "Close the window"
    kSmartHome_MakeItBrighter,      // "Make it brighter"
    kSmartHome_MakeItDarker,        // "Make it darker"
};


#endif /* ENABLE_VIT_ASR */
#endif /* VIT_EN_EN_STRINGS_TO_ACTIONS_VIT_H_ */
