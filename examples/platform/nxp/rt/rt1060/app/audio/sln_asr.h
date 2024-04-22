/*
 * Copyright 2020-2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SLN_ASR_H__
#define __SLN_ASR_H__

#include <stddef.h>
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HANDLE void*

#define CmdMapID_Garbage  0
#define CmdMapID_WW       1
#define CmdMapID_IOT      2
#define CmdMapID_Elevator 3
#define CmdMapID_Audio    4
#define CmdMapID_Wash     5
#define CmdMapID_LED      6
#define CmdMapID_Dialog   7
#define CmdMapID_Temp     8
#define CmdMapID_Timer    9

typedef struct _asr_result
{
    int32_t keywordID[2];   // typically, we need two phases - WW and CMD with ID of each
    int32_t cmdMapID;       // command map ID set in Models Tool
    int32_t trustScore;     // The confidence score of the detection
    int32_t SGDiffScore;    // The Silence/Garbage Difference score of the detection
    int32_t filterScore;    // Higher Fil score means voice is more different from Filter model
    int32_t energy;         // Cmd energy(RMS)
    int32_t wordDura;       // Duration of the result in number of samples
    int32_t endSil;         // Ending silence length in number of sample
    int32_t networkLatency; // Model delay length in number of samples
} asr_result_t;

typedef enum _sln_asr_local_states
{
    kAsrLocalSuccess = 0,
    kAsrLocalDetected,
    kAsrLocalUnsupported,
    kAsrLocalInstallFailed,
    kAsrLocalOutOfMemory,
    kAsrLocalNullPointer,
    kAsrLocalEvaluation,
} sln_asr_local_states_t;
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/**
 * @brief Initialize the detection library
 * 
 * @param addrBase          The base model
 * @param addrGroup         The keyword group, a group of either wake word(s) or commands
 * @param numModel          The number of models
 * @param maxTime           The maximum buffer length in number of frames for keeping the status of commands
 * @param memPool           Memory buffer for the recognizer
 * @param memSize           Size in bytes of the memory buffer memPool
 * @param status            0 indicates success, else error code. It can be NULL
 * @return HANDLE           A recognizer handle or NULL
 */
HANDLE SLN_ASR_LOCAL_Init(uint8_t *addrBase,
                        uint8_t **addrGroup,
                        int32_t numModel,
                        int32_t maxTime,
                        uint8_t *memPool,
                        int32_t memSize,
                        int32_t *status);

/**
 * @brief Sets the maps IDs for the commands
 *
 * @param handler    A handle of the recognizer
 * @param addrGroup  The keyword group, a group of either wake word(s) or commands
 * @param nGroups    The number of groups

 * @return int32_t  The status of the execution (sln_asr_local_states_t)
 */
int32_t SLN_ASR_LOCAL_Set_CmdMapID(HANDLE handler,
                                unsigned char **addrGroup,
                                signed int nGroups);

/**
 * @brief Transfer voice samples to the recognizer for recognizing and if recognized it gives the id of the command
 * 
 * @param handler   A handle of the recognizer
 * @param audioBuff The pointer of voice data buffer
 * @param bufSize   The number of voice data (a unit is a short, we prefer to add 480 samples per call)
 * @param kwdID     The recognized command ID
 * @param mapID     The recognized command map ID
 * @param result    Pointer to structure containing the resulting parameters of a detection
 * @return int32_t  The status of the execution (sln_asr_local_states_t)
 */
int32_t SLN_ASR_LOCAL_Process(HANDLE handler,
                        int16_t *audioBuff,
                        uint16_t bufSize,
                        asr_result_t *result);

/**
 * @brief Get the memory usage of the model
 * 
 * @param addrBase  The base model
 * @param addrGroup The keyword group, a group of either wake word(s) or commands
 * @param numModel  The number of models
 * @param maxTime   The maximum buffer length in number of frames for keeping the status of commands
 * @return int32_t  The memory usage of the model
 */
int32_t SLN_ASR_LOCAL_Verify(unsigned char *addrBase,
                        unsigned char **addrGroup,
                        signed int numModel,
                        signed int maxTime);

/**
 * @brief Get the duration in samples of the last detected command
 *
 * @param handler   A handle of the recognizer
 * @return uint32_t Number of samples from the beginning of the last detected commands until this call.
 */
uint32_t SLN_ASR_LOCAL_GetDetectedCommandDuration(HANDLE handler);

/**
 * @brief Reset recognizer
 * 
 * @param handler A handle of the recognizer
 */
void SLN_ASR_LOCAL_Reset(HANDLE handler);

/**
 * @brief Destroy a recognizer (free resources)
 * 
 * @param handler A handle of the recognizer
 */
void SLN_ASR_LOCAL_Release(HANDLE handler);

/**
 * @brief Get DSMT lib version
 *
 * @return Pointer to string representing the version
 */
const char *SLN_ASR_LOCAL_Ver(void);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif // __SLN_ASR_H__
