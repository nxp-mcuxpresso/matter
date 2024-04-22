/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DSMT_EN_EN_STRINGS_TO_ACTIONS_DSMT_H_
#define DSMT_EN_EN_STRINGS_TO_ACTIONS_DSMT_H_

#if ENABLE_DSMT_ASR

#include "demo_actions_dsmt.h"
#include "stdint.h"

const int16_t actions_ww_en[] = {
    kWakeWord_Detected, // "Hey NXP"
    kWakeWord_Detected, // "Hey NXP ^1"
    kWakeWord_Detected, // "Hey NXP ^2"
    kWakeWord_Detected, // "Hey NXP ^3"
    kWakeWord_Detected, // "Hey NXP ^4"
};

const int16_t actions_smart_home_en[] = {
    kSmartHome_TurnOnTheLights,     // "Turn on the lights"
    kSmartHome_TurnOnTheLights,     // "Turn on the lights ^1"
    kSmartHome_TurnOnTheLights,     // "Turn on the lights ^2"
    kSmartHome_TurnOnTheLights,     // "Turn on the lights ^3"
    kSmartHome_TurnOnTheLights,     // "Turn on the lights ^4"
    kSmartHome_TurnOnTheLights,     // "Turn on the lights ^5"
    kSmartHome_TurnOffTheLights,    // "Turn off the lights"
    kSmartHome_TurnOffTheLights,    // "Turn off the lights ^1"
    kSmartHome_TurnOffTheLights,    // "Turn off the lights ^2"
    kSmartHome_OpenTheWindow,       // "Open the window"
    kSmartHome_OpenTheWindow,       // "Open the window ^1"
    kSmartHome_OpenTheWindow,       // "Open the window ^2"
    kSmartHome_CloseTheWindow,      // "Close the window"
    kSmartHome_CloseTheWindow,      // "Close the window ^1"
    kSmartHome_CloseTheWindow,      // "Close the window ^2"
    kSmartHome_CloseTheWindow,      // "Close the window ^3"
    kSmartHome_CloseTheWindow,      // "Close the window ^4"
    kSmartHome_CloseTheWindow,      // "Close the window ^5"
    kSmartHome_MakeItBrighter,      // "Make it brighter"
    kSmartHome_MakeItDarker,        // "Make it darker"

};

#endif /* ENABLE_DSMT_ASR */
#endif /* DSMT_EN_EN_STRINGS_TO_ACTIONS_DSMT_H_ */
