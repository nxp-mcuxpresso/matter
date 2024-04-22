/*
 * Copyright 2022-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_LAYER_H_
#define APP_LAYER_H_

#include "sln_local_voice_common.h"
#include "stdbool.h"

/**
 * @brief Handle a detected Wake Word.
 *
 * @param commandConfig Structure describing the detected the Wake Word.
 *
 * @return Status of the operation.
 */
status_t APP_LAYER_ProcessWakeWord(oob_demo_control_t *commandConfig);

#if ENABLE_S2I_ASR
/**
 * @brief Handle a detected Voice Intent.
 *
 * @return Status of the operation.
 */
status_t APP_LAYER_ProcessIntent(void);

/**
 * @brief Filters a VIT intent detection, depending on the state machine logic.
 *
 * @return true if command should be filtered, false otherwise.
 */
bool APP_LAYER_FilterIntent(void);
#else
/**
 * @brief Handle a detected Voice Command.
 *
 * @param commandConfig Structure describing the detected the Voice Command.
 *
 * @return Status of the operation.
 */
status_t APP_LAYER_ProcessVoiceCommand(oob_demo_control_t *commandConfig);
#endif /* ENABLE_S2I_ASR */

/**
 * @brief Handle a Voice Command timeout.
 *
 * @param commandConfig Structure describing expected Voice Command.
 *
 * @return Status of the operation.
 */
status_t APP_LAYER_ProcessTimeout(oob_demo_control_t *commandConfig);

#if ENABLE_VIT_ASR
/**
 * @brief Filters a VIT command detection, depending on the state machine logic.
 *
 * @param commandId Id of the command detected.
 * @param activeDemo Id of the active demo.
 *
 * @return true if command should be filtered, false otherwise.
 */
bool APP_LAYER_FilterVitDetection(unsigned short commandId, asr_inference_t activeDemo);
#endif /* ENABLE_VIT_ASR */

/**
 * @brief Function which is called when SW2 button is pressed
 *        The purpose of usage is to cycle between demos through the press of a button.
 * @return void
 */
void APP_LAYER_SwitchToNextDemo(void);

/**
 * @brief If project logic requires handling first board boot, it can be done by
 *        implementing this function.
 *        BOOT_ASR_CMD_DEMO should also be different from DEFAULT_ASR_CMD_DEMO.
 * @return void
 */
void APP_LAYER_HandleFirstBoardBoot(void);

#endif /* APP_LAYER_H_ */
