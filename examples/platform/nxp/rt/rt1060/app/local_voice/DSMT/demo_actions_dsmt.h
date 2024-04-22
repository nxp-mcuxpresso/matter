/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DSMT_DEMO_ACTIONS_DSMT_H_
#define DSMT_DEMO_ACTIONS_DSMT_H_

#if ENABLE_DSMT_ASR

/* Global actions for all languages */

enum _wake_word_action
{
    kWakeWord_Detected = 0,
    kWakeWord_ActionInvalid
};

enum _smart_home_action
{
    kSmartHome_TurnOnTheLights = 0,
    kSmartHome_TurnOffTheLights,
    kSmartHome_OpenTheWindow,
    kSmartHome_CloseTheWindow,
    kSmartHome_MakeItBrighter,
    kSmartHome_MakeItDarker,
    kSmartHome_ActionInvalid
};
#endif /* ENABLE_DSMT_ASR */
#endif /* DSMT_DEMO_ACTIONS_DSMT_H_ */
