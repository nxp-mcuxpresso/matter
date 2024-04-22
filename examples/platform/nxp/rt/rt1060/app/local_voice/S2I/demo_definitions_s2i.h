/*
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DEMO_DEFINITIONS_S2I_H_
#define DEMO_DEFINITIONS_S2I_H_

#if ENABLE_S2I_ASR

/* Enumeration of the languages supported by the project
 * Add, remove or replace languages here, but keep the one bit
 * per language allocation, as currently implemented.
 * The code assumes one bit allocation per definition */
typedef enum _asr_languages
{
    UNDEFINED_LANGUAGE = 0,
    ASR_FIRST_LANGUAGE = (1U << 0U),
    ASR_ENGLISH        = (1U << 0U),
} asr_language_t;

/* Enumeration of the command sets integrated into the project
 * Add, remove or replace the command sets here.
 * Keep the one bit per command set allocation, as currently implemented.
 * The code assumes one bit allocation per definition */
typedef enum _asr_inference
{
    UNDEFINED_INFERENCE     = 0,
    ASR_S2I_HOME            = (1U << 0U),
    ASR_CMD_INVALID_DEMO
} asr_inference_t;

/* listen for multiple wake words in parallel; not supported for VIT */
#define MULTILINGUAL               (0)

/* demo after first boot. can be used for selecting the demo at the very first board boot */
#define BOOT_ASR_CMD_DEMO          ASR_S2I_HOME

/* default demo. this can have the same value as BOOT_ASR_CMD_DEMO
 * in our POC we use BOOT_ASR_CMD_DEMO to give the possibility to the user to select the demo
 * via voice commands. If a selection is not made until the timeout is hit, then
 * DEFAULT_ASR_CMD_DEMO will be loaded */
#define DEFAULT_ASR_CMD_DEMO       ASR_S2I_HOME

/* default language */
#define DEFAULT_ASR_LANGUAGE       ASR_ENGLISH

/* max languages active at the same time */
#define MAX_CONCURRENT_LANGUAGES   (1)

#define DEMO_STR_HOME "home"

#define LANG_STR_EN   "en"

#define SHELL_SELECTABLE_DEMOS     DEMO_STR_HOME
#define SHELL_SELECTABLE_LANGUAGES LANG_STR_EN

#endif /* ENABLE_S2I_ASR */
#endif /* DEMO_DEFINITIONS_S2I_H_ */
