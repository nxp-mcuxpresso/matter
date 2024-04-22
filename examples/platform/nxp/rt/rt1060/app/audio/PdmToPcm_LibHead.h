/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PdmToPcmLibHead_h__
#define __PdmToPcmLibHead_h__



#ifdef __cplusplus
extern "C"{
#endif

typedef enum {
    Status_SUCCESS = 0,
    Status_FAIL = 1,
} PdmConvertingLibStatus;

typedef enum {
    PdmConvertingCfg1 = 1,
    PdmConvertingCfg2 = 2,
    PdmConvertingCfg3 = 3,
    PdmConvertingCfg4 = 4,
    PdmConvertingCfg5 = 5,
    PdmConvertingCfg6 = 6,
} PdmConvertingCfg;


extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg1_NoHpf(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg2_NoHpf(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg3_NoHpf(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg4_NoHpf(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg5_NoHpf(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg6_NoHpf(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);

extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg1_WithHpf1(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg2_WithHpf1(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg3_WithHpf1(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg4_WithHpf1(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg5_WithHpf1(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg6_WithHpf1(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);

extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg1_WithHpf2(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg2_WithHpf2(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg3_WithHpf2(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg4_WithHpf2(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg5_WithHpf2(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);
extern PdmConvertingLibStatus PdmToPcm_ConvertOneFrame_Cfg6_WithHpf2(uint32_t *PdmSrcPtr_U32, float *PcmDstPtr_Float, int8_t ChIdx);


extern PdmConvertingLibStatus PdmToPcm_Init(PdmConvertingCfg CfgIdx, uint32_t ChNum, uint16_t FrmSize);
extern uint16_t PdmToPcm_GetHeapSizeNeededForPdmConverting(void);
extern void PdmToPcm_Create(void *MemStrtPtr);







extern void MoveFloatSamplesToS32Buf_8SampleGrouped(int32_t *DstPtr, float *SrcPtr, uint32_t LengthIn8SamplesBlock);
extern void MoveS32SamplesToFloatBuf_8SampleGrouped(float *DstPtr, int32_t *SrcPtr, uint32_t LengthIn8SamplesBlock);

#ifdef __cplusplus
}
#endif

#endif

