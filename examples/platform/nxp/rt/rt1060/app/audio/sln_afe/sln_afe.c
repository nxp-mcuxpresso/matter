/*
 * Copyright 2021-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "arm_math.h"
#include "libVoiceSeekerLight.h"
#include "math.h"
#include "sln_afe.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Size of input and output streams used for Process_Audio function:
 * microphone, speaker and processed stream. */
#define INPUT_CHUNK_SIZE (160)

/* VSL input and output frame sizes.
 * VoiceSeekerLight_Process takes as input chunks of 32 samples.
 * VoiceSeekerLight_Process gives as output a chunk of 160 samples. */
#define VSL_FRAME_SIZE_IN  32
#define VSL_FRAME_SIZE_OUT 160

#if INPUT_CHUNK_SIZE != VSL_FRAME_SIZE_OUT
#error "INPUT_CHUNK_SIZE must be equal to VSL_FRAME_SIZE_OUT"
#endif /* INPUT_CHUNK_SIZE != VSL_FRAME_SIZE_OUT */

/* Number of speakers */
#if ENABLE_AEC
#define VSL_NUM_SPK 1
#else
#define VSL_NUM_SPK 0
#endif /* ENABLE_AEC */

/* Amount of memory from VSL's memory pool's end which is used as scratch memory. */
#define SCRATCH_MEM_SIZE_WW_LEN_1500 (1024 * 4)
#define SCRATCH_MEM_SIZE_WW_LEN_3000 (1024 * 8)

typedef enum _reserved_indices
{
    kRes0,
    kRes1,
    kRes2,
    kRes3,
    kRes4,
    kResTotal
} reserved_indices_t;

typedef struct _afe_mem_pool
{
    uint8_t micCount;
    float **micBuffsAddr;
    float *micBuffs;
    uint8_t speakerCount;
    float **refBuffsAddr;
    float *refBuffs;
    float postGain;
    afe_data_type_t dataInType;
    afe_data_type_t dataOutType;
    RETUNE_VOICESEEKERLIGHT_plugin_t *vsl;
    rdsp_voiceseekerlight_ver_struct_t version;
    int32_t reserved[kResTotal];
} afe_mem_pool_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

static afe_mem_pool_t *s_afeMemPool = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/

sln_afe_status_t SLN_AFE_Init(sln_afe_config_t *afeConfig)
{
    sln_afe_status_t status                    = kAfeSuccess;
    RdspStatus vslStatus                       = OK;
    afe_mem_pool_t *afeMemPool                 = NULL;
    RETUNE_VOICESEEKERLIGHT_plugin_t *vsl      = NULL;
    rdsp_voiceseekerlight_config_t vslConfig   = {0};
    uint32_t requiredHeapSize                  = 0;
    rdsp_voiceseekerlight_ver_struct_t vsl_ver = {0};

    float **micBuffsAddr = NULL;
    float *micBuffs      = NULL;
    float **refBuffsAddr = NULL;
    float *refBuffs      = NULL;

    if ((afeConfig == NULL) || (afeConfig->mallocFunc == NULL) || (afeConfig->freeFunc == NULL))
    {
        status = kAfeNullPointer;
    }

    if (status == kAfeSuccess)
    {
        afeMemPool = afeConfig->mallocFunc(sizeof(afe_mem_pool_t));
        if (afeMemPool == NULL)
        {
            status = kAfeOutOfMemory;
        }
    }

    if (status == kAfeSuccess)
    {
        vsl = afeConfig->mallocFunc(sizeof(RETUNE_VOICESEEKERLIGHT_plugin_t));
        if (vsl == NULL)
        {
            status = kAfeOutOfMemory;
        }
    }

    if (status == kAfeSuccess)
    {
        micBuffs = afeConfig->mallocFunc(sizeof(float) * VSL_FRAME_SIZE_IN * afeConfig->numberOfMics);
        if (micBuffs == NULL)
        {
            status = kAfeOutOfMemory;
        }
    }

    if (status == kAfeSuccess)
    {
        micBuffsAddr = afeConfig->mallocFunc(sizeof(float *) * afeConfig->numberOfMics);
        if (micBuffsAddr != NULL)
        {
            for (uint8_t i = 0; i < afeConfig->numberOfMics; i++)
            {
                micBuffsAddr[i] = &micBuffs[i * VSL_FRAME_SIZE_IN];
            }
        }
        else
        {
            status = kAfeOutOfMemory;
        }
    }

    if (status == kAfeSuccess)
    {
        vslConfig.mic_xyz_mm = afeConfig->mallocFunc(sizeof(rdsp_xyz_t) * afeConfig->numberOfMics);
        if (vslConfig.mic_xyz_mm == NULL)
        {
            status = kAfeOutOfMemory;
        }
    }

#if VSL_NUM_SPK
    if (status == kAfeSuccess)
    {
        refBuffs = afeConfig->mallocFunc(sizeof(float) * VSL_FRAME_SIZE_IN * VSL_NUM_SPK);
        if (refBuffs == NULL)
        {
            status = kAfeOutOfMemory;
        }
    }

    if (status == kAfeSuccess)
    {
        refBuffsAddr = afeConfig->mallocFunc(sizeof(float *) * VSL_NUM_SPK);
        if (refBuffsAddr != NULL)
        {
            for (uint8_t i = 0; i < VSL_NUM_SPK; i++)
            {
                refBuffsAddr[i] = &refBuffs[i * VSL_FRAME_SIZE_IN];
            }
        }
        else
        {
            status = kAfeOutOfMemory;
        }
    }
#endif /* VSL_NUM_SPK */

    if (status == kAfeSuccess)
    {
        memset(vsl, 0, sizeof(RETUNE_VOICESEEKERLIGHT_plugin_t));

        vsl->mem.pPrivateDataBase    = afeConfig->afeMemBlock;
        vsl->mem.pPrivateDataNext    = afeConfig->afeMemBlock;
        vsl->mem.FreePrivateDataSize = afeConfig->afeMemBlockSize;

        vslConfig.num_mics             = afeConfig->numberOfMics;
        vslConfig.num_spks             = VSL_NUM_SPK;
        vslConfig.framesize_out        = VSL_FRAME_SIZE_OUT;
        vslConfig.create_aec           = afeConfig->aecEnabled;
        vslConfig.create_doa           = afeConfig->doaEnabled;
        vslConfig.buffer_length_sec    = ((float)afeConfig->wakeWordMaxLength) / 1000;
        vslConfig.aec_filter_length_ms = afeConfig->aecFilterLength;

        if (afeConfig->mcuType == kAfeMcuMIMXRT1060)
        {
            vslConfig.device_id = Device_IMXRT1060_CM7;
        }
        else if (afeConfig->mcuType == kAfeMcuMIMXRT1170)
        {
            vslConfig.device_id = Device_IMXRT1170_CM7;
        }
        else
        {
            /* Not critical, works without specifying the MCU. */
        }

        for (uint8_t i = 0; i < afeConfig->numberOfMics; i++)
        {
            vslConfig.mic_xyz_mm[i][0] = afeConfig->micsPosition[i][0];
            vslConfig.mic_xyz_mm[i][1] = afeConfig->micsPosition[i][1];
            vslConfig.mic_xyz_mm[i][2] = afeConfig->micsPosition[i][2];
        }

        requiredHeapSize = VoiceSeekerLight_GetRequiredHeapMemoryBytes(vsl, &vslConfig);
        if (requiredHeapSize > afeConfig->afeMemBlockSize)
        {
            status = kAfeOutOfMemory;
        }
    }

    if (status == kAfeSuccess)
    {
        vslStatus = VoiceSeekerLight_Create(vsl, &vslConfig);

        if (vslStatus != OK)
        {
            status = kAfeFail;
        }
    }

#if ENABLE_VAD
    if (status == kAfeSuccess)
    {
        vslStatus = VoiceSeekerLight_Vad_Create(vsl);

        if (vslStatus != OK)
        {
            status = kAfeFail;
        }
    }
#endif /* ENABLE_VAD */

    if (status == kAfeSuccess)
    {
        VoiceSeekerLight_Init(vsl);

        VoiceSeekerLight_GetLibVersion(vsl, &vsl_ver);

        afeMemPool->version.major = vsl_ver.major;
        afeMemPool->version.minor = vsl_ver.minor;
        afeMemPool->version.patch = vsl_ver.patch;

#if ENABLE_VAD
        VoiceSeekerLight_Vad_Init(vsl);
#endif /* ENABLE_VAD */

        afeMemPool->micCount     = afeConfig->numberOfMics;
        afeMemPool->micBuffsAddr = micBuffsAddr;
        afeMemPool->micBuffs     = micBuffs;
        afeMemPool->speakerCount = VSL_NUM_SPK;
        afeMemPool->refBuffsAddr = refBuffsAddr;
        afeMemPool->refBuffs     = refBuffs;
        afeMemPool->postGain     = afeConfig->postProcessedGain;
        afeMemPool->dataInType   = afeConfig->dataInType;
        afeMemPool->dataOutType  = afeConfig->dataOutType;
        afeMemPool->vsl          = vsl;

        s_afeMemPool = afeMemPool;
    }
    else
    {
        if ((afeConfig != NULL) && (afeConfig->freeFunc != NULL))
        {
            afeConfig->freeFunc(vsl);
            afeConfig->freeFunc(micBuffs);
            afeConfig->freeFunc(micBuffsAddr);
            afeConfig->freeFunc(vslConfig.mic_xyz_mm);
            afeConfig->freeFunc(refBuffs);
            afeConfig->freeFunc(refBuffsAddr);
            afeConfig->freeFunc(afeMemPool);
        }

        s_afeMemPool = NULL;
    }

    return status;
}

sln_afe_status_t SLN_AFE_Process_Audio(void *audioBuff, int16_t *refSignal, void **processedAudio)
{
    sln_afe_status_t status               = kAfeSuccess;
    RdspStatus vslStatus                  = OK;
    RETUNE_VOICESEEKERLIGHT_plugin_t *vsl = NULL;
    float *vslProcessedAudio              = NULL;

    if ((audioBuff == NULL) || (processedAudio == NULL) || (s_afeMemPool == NULL))
    {
        status = kAfeNullPointer;
    }

    if (status == kAfeSuccess)
    {
        vsl = s_afeMemPool->vsl;
        if (vsl == NULL)
        {
            status = kAfeNullPointer;
        }
    }

    if (status == kAfeSuccess)
    {
        for (uint8_t i = 0; i < (INPUT_CHUNK_SIZE / VSL_FRAME_SIZE_IN); i++)
        {
            for (uint8_t micID = 0; micID < s_afeMemPool->micCount; micID++)
            {
                if (s_afeMemPool->dataInType == kAfeTypeFloat)
                {
                    memcpy(s_afeMemPool->micBuffsAddr[micID],
                           &(((float *)audioBuff)[micID * INPUT_CHUNK_SIZE + i * VSL_FRAME_SIZE_IN]),
                           VSL_FRAME_SIZE_IN * sizeof(float));
                }
                else if (s_afeMemPool->dataInType == kAfeTypeInt16)
                {
                    arm_q15_to_float(&(((int16_t *)audioBuff)[micID * INPUT_CHUNK_SIZE + i * VSL_FRAME_SIZE_IN]),
                                     s_afeMemPool->micBuffsAddr[micID], VSL_FRAME_SIZE_IN);
                }
                else if (s_afeMemPool->dataInType == kAfeTypeInt32)
                {
                    arm_q31_to_float(&(((int32_t *)audioBuff)[micID * INPUT_CHUNK_SIZE + i * VSL_FRAME_SIZE_IN]),
                                     s_afeMemPool->micBuffsAddr[micID], VSL_FRAME_SIZE_IN);
                }
            }

            if (refSignal != NULL)
            {
                for (uint8_t refID = 0; refID < s_afeMemPool->speakerCount; refID++)
                {
                    arm_q15_to_float(&refSignal[refID * INPUT_CHUNK_SIZE + i * VSL_FRAME_SIZE_IN],
                                     s_afeMemPool->refBuffsAddr[refID], VSL_FRAME_SIZE_IN);
                }
            }

            /* refSignal == NULL means that there is no need to perform AEC.
             * AEC disabled means less CPU usage.
             * (example: device not streaming -> no need for AEC) */
            vslProcessedAudio = NULL;
            if (refSignal != NULL)
            {
                vslStatus = VoiceSeekerLight_Process(vsl, s_afeMemPool->micBuffsAddr, s_afeMemPool->refBuffsAddr,
                                                     &vslProcessedAudio);
            }
            else
            {
                vslStatus = VoiceSeekerLight_Process(vsl, s_afeMemPool->micBuffsAddr, NULL, &vslProcessedAudio);
            }

            if (vslStatus == OK)
            {
                if (vslProcessedAudio != NULL)
                {
                    if (i != ((INPUT_CHUNK_SIZE / VSL_FRAME_SIZE_IN) - 1))
                    {
                        status = kAfeFail;
                        break;
                    }

                    if (s_afeMemPool->postGain != 1)
                    {
                        arm_scale_f32(vslProcessedAudio, s_afeMemPool->postGain, vslProcessedAudio, VSL_FRAME_SIZE_OUT);
                    }
                }
            }
            else
            {
                status = kAfeFail;
                break;
            }
        }

        if (vslProcessedAudio != NULL)
        {
            /* Convert the result to the desired output type */
            if (s_afeMemPool->dataOutType == kAfeTypeFloat)
            {
                /* If configured for float output array, simply reuse the result from VSL */
            }
            else if (s_afeMemPool->dataOutType == kAfeTypeInt16)
            {
                arm_float_to_q15(vslProcessedAudio, (q15_t *)vslProcessedAudio, VSL_FRAME_SIZE_OUT);
            }
            else if (s_afeMemPool->dataOutType == kAfeTypeInt32)
            {
                arm_float_to_q31(vslProcessedAudio, (q31_t *)vslProcessedAudio, VSL_FRAME_SIZE_OUT);
            }

            *processedAudio = vslProcessedAudio;
        }
        else
        {
            status = kAfeFail;
        }
    }

    if ((status != kAfeSuccess) && (processedAudio != NULL))
    {
        *processedAudio = NULL;
    }

    return status;
}

sln_afe_status_t SLN_AFE_Trigger_Found(uint32_t wakeWordLength)
{
    sln_afe_status_t status               = kAfeSuccess;
    RETUNE_VOICESEEKERLIGHT_plugin_t *vsl = NULL;

    if (s_afeMemPool == NULL)
    {
        status = kAfeNullPointer;
    }

    if (status == kAfeSuccess)
    {
        vsl = s_afeMemPool->vsl;
        if (vsl == NULL)
        {
            status = kAfeNullPointer;
        }
    }

    if (status == kAfeSuccess)
    {
        VoiceSeekerLight_TriggerFound(vsl, wakeWordLength);
    }

    return status;
}

sln_afe_status_t SLN_AFE_Voice_Detected(int16_t *audio, bool *voiceDetected)
{
    sln_afe_status_t status               = kAfeSuccess;
    RETUNE_VOICESEEKERLIGHT_plugin_t *vsl = NULL;
    int32_t vad_ret                       = 0;

    if (s_afeMemPool == NULL)
    {
        status = kAfeNullPointer;
    }

    if (status == kAfeSuccess)
    {
        vsl = s_afeMemPool->vsl;
        if (vsl == NULL)
        {
            status = kAfeNullPointer;
        }
    }

    if (status == kAfeSuccess)
    {
        /* Use microphone's buffer to check VAD.
         * SLN_AFE_Process_Audio and SLN_AFE_Voice_Detected must not overlap. */
        for (uint8_t i = 0; i < (INPUT_CHUNK_SIZE / VSL_FRAME_SIZE_IN); i++)
        {
            arm_q15_to_float(&audio[i * VSL_FRAME_SIZE_IN], s_afeMemPool->micBuffsAddr[0], VSL_FRAME_SIZE_IN);

            vad_ret = VoiceSeekerLight_Vad_Process(vsl, s_afeMemPool->micBuffsAddr[0]);
            if (vad_ret == 1)
            {
                break;
            }
        }

        if (vad_ret == 1)
        {
            *voiceDetected = true;
        }
        else
        {
            *voiceDetected = false;
        }
    }

    return status;
}

void SLN_AFE_Get_Version(uint32_t *major, uint32_t *minor, uint32_t *patch)
{
    rdsp_voiceseekerlight_ver_struct_t vsl_ver = {0};

    VoiceSeekerLight_GetLibVersion(s_afeMemPool->vsl, &vsl_ver);

    *major = vsl_ver.major;
    *minor = vsl_ver.minor;
    *patch = vsl_ver.patch;
}

/* Below functions need to be defined because they are required by VoiceSeekerLight.
 * Attribute "weak" offers the possibility to redefine these functions on application side. */

__attribute__((weak)) float log2f(float x)
{
    return (log(x) / log(2));
}

__attribute__((weak)) void init_ccount(void)
{
    ;
}

__attribute__((weak)) uint32_t read_ccount(void)
{
    return 0;
}
