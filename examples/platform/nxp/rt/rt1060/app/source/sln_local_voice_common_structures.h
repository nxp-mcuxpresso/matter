/*
 * Copyright 2023-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SLN_LOCAL_VOICE_COMMON_STRUCTURES_H_
#define SLN_LOCAL_VOICE_COMMON_STRUCTURES_H_

#define APP_ASR_SHELL_VERSION 5

typedef enum _audio_processing_states
{
    kWakeWordDetected     = (1 << 0U),
    kVoiceCommandDetected = (1 << 1U),
    kMicUpdate            = (1 << 2U),
    kVolumeUpdate         = (1 << 3U),
    kTimeOut              = (1 << 4U),
    kAsrModelChanged      = (1 << 5U),
    kAsrModeChanged       = (1 << 6U),
    kDefault              = (1 << 7U)
} audio_processing_states_t;

typedef struct _oob_demo_control
{
    uint16_t language;
    uint16_t commandSet;
    uint8_t commandId;
    uint8_t skipWW;                // Set to 1 to skip Wake Word phase and go directly to Voice Command phase. Will self-clear automatically.
    uint8_t changeDemoFlow;
    uint8_t changeLanguageFlow;
} oob_demo_control_t;

typedef enum _app_flash_status
{
    READ_SUCCESS  = (1U << 0U),
    READ_FAIL     = (1U << 1U),
    READ_READY    = (1U << 2U),
    WRITE_SUCCESS = (1U << 3U),
    WRITE_FAIL    = (1U << 4U),
    WRITE_READY   = (1U << 5U),
} app_flash_status_t;

typedef enum _asr_mics_state
{
    ASR_MICS_OFF = 0,
    ASR_MICS_ON,
} asr_mics_state_t;

typedef enum _asr_cmd_res
{
    ASR_CMD_RES_OFF = 0,
    ASR_CMD_RES_ON,
} asr_cmd_res_t;

typedef enum _asr_cfg_demo
{
    ASR_CFG_DEMO_NO_CHANGE               = (1U << 0U), // OOB demo type or languages unchanged
    ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED = (1U << 1U), // OOB demo type changed
    ASR_CFG_DEMO_LANGUAGE_CHANGED        = (1U << 2U), // OOB language type changed
    ASR_CFG_MODE_CHANGED                 = (1U << 3U), // OOB ASR mode changed
} asr_cfg_demo_t;

typedef enum _asr_mode
{
    ASR_MODE_WW_AND_CMD     = (1U << 0U), // ASR processes wake-word + command pairs
    ASR_MODE_WW_AND_MUL_CMD = (1U << 1U), // ASR processes wake-word + multiple commands
    ASR_MODE_WW_ONLY        = (1U << 2U), // ASR processes only wake-words
    ASR_MODE_CMD_ONLY       = (1U << 3U), // ASR processes only commands
    ASR_MODE_PTT            = (1U << 4U), // ASR processes push-to-talk mode
} asr_mode_t;

typedef struct _app_asr_shell_commands
{
    app_flash_status_t status;
    asr_cfg_demo_t asrCfg;
    uint8_t volume; // 0 ~ 100
    asr_mics_state_t micsState; // mics state: on or off
    uint32_t timeout; // in millisecond
    asr_mode_t asrMode;
    uint16_t demo;        // demo types: elevator, washing machine, smart home
    uint16_t activeLanguage; // runtime language types
    asr_cmd_res_t cmdresults;
    uint8_t vitActive;  // VIT ASR is used
    uint8_t version;
} app_asr_shell_commands_t;

#endif /* SLN_LOCAL_VOICE_COMMON_STRUCTURES_H_ */
