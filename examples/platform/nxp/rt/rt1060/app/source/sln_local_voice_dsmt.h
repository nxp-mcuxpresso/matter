/*
 * Copyright 2021-2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SLN_LOCAL_VOICE_DSMT_H_
#define SLN_LOCAL_VOICE_DSMT_H_

#if ENABLE_DSMT_ASR

#include "sln_asr.h"
#include "sln_local_voice_common_structures.h"
#include "demo_definitions_dsmt.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define NUM_GROUPS        (NUM_CMD_GROUPS + 3)      // groups: base, ww, cmd_iot, cmd_elevator, and so on
#define NUM_INFERENCES_WW (NUM_LANGUAGES)           // WW in multiple languages

// ASR events
typedef enum _asr_events
{
    ASR_SESSION_STARTED,
    ASR_SESSION_ENDED,
    ASR_SESSION_TIMEOUT,
} asr_events_t;

struct asr_language_model;   // will be used to install the selected languages.
struct asr_inference_engine; // will be used to install and set the selected WW/CMD inference engines.

struct asr_language_model
{
    asr_language_t iWhoAmI; // language types for language model. A model is language specific.
    uint8_t nGroups;                               // base, group1 (ww), group2 (commands set 1), group3 (commands set 2), ..., groupN (commands set N)
    unsigned char *addrBin;                        // model binary address
    unsigned char *addrGroup[NUM_GROUPS];          // addresses for base, group1, group2, ...
    unsigned char *addrGroupMapID[NUM_GROUPS - 1]; // addresses for mapIDs for group1, group2, ...
    struct asr_language_model *next;               // pointer to next language model in this linked list
};

struct asr_inference_engine
{
    asr_inference_t iWhoAmI_inf;   // inference types for WW engine or CMD engine
    asr_language_t iWhoAmI_lang;   // language for inference engine
    void *handler;                 // model handler
    uint8_t nGroups;               // the number of groups for an inference engine. Default is 2 and it's enough.
    unsigned char *addrGroup[2];   // base + keyword group. default nGroups is 2
    unsigned char *addrGroupMapID; // mapID group. default nGroups is 1
    char **idToKeyword;            // the string list
    unsigned char *memPool;        // memory pool in ram for inference engine
    uint32_t memPoolSize;          // memory pool size
    struct asr_inference_engine
        *next; // pointer to next inference engine, if this is linked list. The end of "next" should be NULL.
};

typedef struct _asr_control
{
    struct asr_language_model *langModel;      // linked list
    struct asr_inference_engine *infEngineWW;  // linked list
    struct asr_inference_engine *infEngineCMD; // not linked list
    uint32_t sampleCount;                      // to measure the waiting response time
    asr_result_t result;                       // results of the command processing
} asr_control_t;

#if defined(__cplusplus)
}
#endif

#endif /* ENABLE_DSMT_ASR */

#endif /* SLN_LOCAL_VOICE_DSMT_H_ */
