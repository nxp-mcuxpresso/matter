/*
 * Copyright 2022-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SLN_LOCAL_VOICE_COMMON_H_
#define SLN_LOCAL_VOICE_COMMON_H_

#include "stdint.h"
#include "string.h"

#if ENABLE_DSMT_ASR
#include "sln_local_voice_dsmt.h"
#elif ENABLE_VIT_ASR
#include "sln_local_voice_vit.h"
#elif ENABLE_S2I_ASR
#include "sln_local_voice_s2i.h"
#else
#error "Must use either ENABLE_DSMT_ASR, ENABLE_VIT_ASR or ENABLE_S2I_ASR"
#endif

// Shell Commands Related
#define ASR_SHELL_COMMANDS_FILE_NAME "asr_shell_commands.dat"

/*!
 * @brief Print current ASR engine and its version
 */
void print_asr_version(void);

/*!
 * @brief Task responsible for ASR initialization and ASR processing.
 *
 * The ASR engine is specified in app.h
 */
void local_voice_task(void *arg);

#endif /* SLN_LOCAL_VOICE_COMMON_H_ */
