/*
 * Copyright 2019-2024 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */

#include "fsl_iomuxc.h"

#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED
#include "sln_mic_config.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#endif /* SDK_SAI_BASED_COMPONENT_USED */

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern void __base_SRAM_DTC(void);
extern void __top_SRAM_DTC(void);
extern void __base_SRAM_ITC(void);
extern void __top_SRAM_ITC(void);
extern void __base_SRAM_OC_NON_CACHEABLE(void);
extern void __top_SRAM_OC_NON_CACHEABLE(void);
extern void __base_SRAM_OC_CACHEABLE(void);
extern void __top_SRAM_OC_CACHEABLE(void);

__attribute__((section(".vectorTableRam"), aligned(0x400))) uint32_t g_vectorTable[256] = {0};

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t __Vectors[];
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
#define __VECTOR_TABLE __Vectors
#define __StackTop     Image$$ARM_LIB_STACK$$ZI$$Limit
#elif defined(__MCUXPRESSO)
extern uint32_t __Vectors[];
extern void _vStackTop(void);
#define __VECTOR_TABLE __Vectors
#define __StackTop     _vStackTop
#elif defined(__ICCARM__)
extern uint32_t __vector_table[];
extern uint32_t CSTACK$$Limit;
#define __VECTOR_TABLE __vector_table
#define __StackTop     CSTACK$$Limit
#elif defined(__GNUC__)
extern uint32_t __StackTop;
extern uint32_t __Vectors[];
#define __VECTOR_TABLE __Vectors
#endif

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t g_i2cHandle = {0};
#endif
#endif

#if ENABLE_AMPLIFIER
#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t dummy_txbuffer[32], 32);

mqs_config_t mqsConfig = {
    .s_AmpRxDataRingBuffer = NULL, .ringbuf_delay = 0, .ringbuf_size = AMP_LOOPBACK_RINGBUF_SIZE, .volume = 0};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_MQS, .codecDevConfig = &mqsConfig};
#endif /* SDK_SAI_BASED_COMPONENT_USED */
#endif /* ENABLE_AMPLIFIER */

/* Needed for overwriting the default vPortSetupTimerInterrupt of FreeRTOS,
 * which doesn't allow dynamic MCU frequency */
#define portNVIC_SYSTICK_CTRL_REG             ( *( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG             ( *( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG    ( *( ( volatile uint32_t * ) 0xe000e018 ) )

#define portNVIC_SYSTICK_CLK_BIT              ( 1UL << 2UL )
#define portNVIC_SYSTICK_INT_BIT              ( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT           ( 1UL << 0UL )

void vPortSetupTimerInterrupt(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint8_t get_mpu_size(uint32_t ramSize)
{
    uint8_t mpuRegSize = 0U;

    if (ramSize == 0U)
    {
        mpuRegSize = 0U;
    }
    else if (ramSize <= (32 * 1024))
    {
        mpuRegSize = ARM_MPU_REGION_SIZE_32KB;
    }
    else if (ramSize <= (64 * 1024))
    {
        mpuRegSize = ARM_MPU_REGION_SIZE_64KB;
    }
    else if (ramSize <= (128 * 1024))
    {
        mpuRegSize = ARM_MPU_REGION_SIZE_128KB;
    }
    else if (ramSize <= (256 * 1024))
    {
        mpuRegSize = ARM_MPU_REGION_SIZE_256KB;
    }
    else
    {
        mpuRegSize = ARM_MPU_REGION_SIZE_512KB;
    }

    return mpuRegSize;
}

/* Get debug console frequency. */
uint32_t BOARD_DebugConsoleSrcFreq(void)
{
    uint32_t freq;

    /* To make it simple, we assume default PLL and divider settings, and the only variable
       from application is use PLL3 source or OSC source */
    if (CLOCK_GetMux(kCLOCK_UartMux) == 0) /* PLL3 div6 80M */
    {
        freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }
    else
    {
        freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }

    return freq;
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq = BOARD_DebugConsoleSrcFreq();

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    lpi2c_master_config_t lpi2cConfig = {0};

    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&lpi2cConfig);
    LPI2C_MasterInit(base, &lpi2cConfig, clkSrc_Hz);
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize)
{
    status_t reVal;

    /* Send master blocking data to slave */
    reVal = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Write);
    if (kStatus_Success == reVal)
    {
        while (LPI2C_MasterGetStatusFlags(base) & kLPI2C_MasterNackDetectFlag)
        {
        }

        reVal = LPI2C_MasterSend(base, &subAddress, subAddressSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterSend(base, txBuff, txBuffSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterStop(base);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }
    }

    return reVal;
}

status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subAddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize)
{
    status_t reVal;

    reVal = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Write);
    if (kStatus_Success == reVal)
    {
        while (LPI2C_MasterGetStatusFlags(base) & kLPI2C_MasterNackDetectFlag)
        {
        }

        reVal = LPI2C_MasterSend(base, &subAddress, subAddressSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterRepeatedStart(base, deviceAddress, kLPI2C_Read);

        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterReceive(base, rxBuff, rxBuffSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterStop(base);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }
    }
    return reVal;
}

status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subAddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(base, deviceAddress, subAddress, subAddressSize, txBuff, txBuffSize);
}

status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize)
{
    status_t reVal;

    reVal = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Write);
    if (kStatus_Success == reVal)
    {
        while (LPI2C_MasterGetStatusFlags(base) & kLPI2C_MasterNackDetectFlag)
        {
        }

        reVal = LPI2C_MasterSend(base, &subAddress, subAddressSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        /* SCCB does not support LPI2C repeat start, must stop then start. */
        reVal = LPI2C_MasterStop(base);

        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Read);

        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterReceive(base, rxBuff, rxBuffSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterStop(base);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }
    }
    return reVal;
}

#endif /* SDK_I2C_BASED_COMPONENT_USED */

#if ENABLE_AMPLIFIER
#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED
void BOARD_SAI_Enable_Mclk_Output(I2S_Type *base, bool enable)
{
    uint32_t mclk_dir_mask = 0;

    if (SAI1 == base)
        mclk_dir_mask = IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
    else if (SAI2 == base)
        mclk_dir_mask = IOMUXC_GPR_GPR1_SAI2_MCLK_DIR_MASK;
    else if (SAI3 == base)
        mclk_dir_mask = IOMUXC_GPR_GPR1_SAI3_MCLK_DIR_MASK;
    else
        return;

    if (enable)
    {
        IOMUXC_GPR->GPR1 |= mclk_dir_mask;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~mclk_dir_mask);
    }
}

void configMQS(void)
{
    CCM->CCGR0 = ((CCM->CCGR0 & (~CCM_CCGR0_CG2_MASK)) | CCM_CCGR0_CG2(3)); /* Enable MQS hmclk. */

    IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, true);  /* Reset MQS. */
    IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, false); /* Release reset MQS. */
    IOMUXC_MQSEnable(IOMUXC_GPR, true);              /* Enable MQS. */
    IOMUXC_MQSConfig(IOMUXC_GPR, kIOMUXC_MqsPwmOverSampleRate64,
                     4u); /* 786.43MHz/1/4/64/(4+1) = 0.614MHz
                          Higher frequency PWM involves less low frequency harmonic.*/
}

void BOARD_SAI_Init(sai_init_handle_t saiInitHandle)
{
    sai_config_t saiConfig = {0};
    sai_transfer_t txfer;

    sai_transfer_format_t saiAmpFormat = {0};

#if USE_MQS
    saiAmpFormat.bitWidth      = kSAI_WordWidth16bits;
    saiAmpFormat.sampleRate_Hz = kSAI_SampleRate48KHz;
#elif USE_16BIT_PCM
    saiAmpFormat.bitWidth      = kSAI_WordWidth16bits;
    saiAmpFormat.sampleRate_Hz = kSAI_SampleRate16KHz;
#elif USE_32BIT_PCM
    saiAmpFormat.bitWidth      = kSAI_WordWidth32bits;
    saiAmpFormat.sampleRate_Hz = kSAI_SampleRate16KHz;
#endif
    saiAmpFormat.channel            = 0U;
    saiAmpFormat.protocol           = kSAI_BusLeftJustified;
    saiAmpFormat.isFrameSyncCompact = true;
    saiAmpFormat.stereo             = kSAI_Stereo;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    saiAmpFormat.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif

    /* Clock setting for SAI3 */
    CLOCK_SetMux(kCLOCK_Sai3Mux, BOARD_AMP_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai3PreDiv, BOARD_AMP_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai3Div, BOARD_AMP_SAI_CLOCK_SOURCE_DIVIDER);

    BOARD_SAI_Enable_Mclk_Output(BOARD_AMP_SAI, true);

    EDMA_CreateHandle(saiInitHandle.amp_dma_tx_handle, DMA0, BOARD_AMP_SAI_EDMA_TX_CH);
    DMAMUX_SetSource(DMAMUX, BOARD_AMP_SAI_EDMA_TX_CH, (uint8_t)BOARD_AMP_SAI_EDMA_TX_REQ);
    DMAMUX_EnableChannel(DMAMUX, BOARD_AMP_SAI_EDMA_TX_CH);

    /* Initialize SAI Tx */
    SAI_TxGetDefaultConfig(&saiConfig);
    saiConfig.protocol = kSAI_BusLeftJustified;
    SAI_TxInit(BOARD_AMP_SAI, &saiConfig);

    SAI_TransferTxCreateHandleEDMA(BOARD_AMP_SAI, saiInitHandle.amp_sai_tx_handle, saiInitHandle.sai_tx_callback, NULL,
                                   saiInitHandle.amp_dma_tx_handle);

    SAI_TransferTxSetFormatEDMA(BOARD_AMP_SAI, saiInitHandle.amp_sai_tx_handle, &saiAmpFormat, BOARD_AMP_SAI_CLK_FREQ,
                                BOARD_AMP_SAI_CLK_FREQ);

    saiInitHandle.amp_sai_tx_handle->channelMask = (1 << saiAmpFormat.channel);
    saiInitHandle.amp_sai_tx_handle->channelNums = 1;

    configMQS();

    /* Force bit clock to override standard enablement */
    SAI_TxSetBitClockRate(BOARD_AMP_SAI, BOARD_AMP_SAI_CLK_FREQ, saiAmpFormat.sampleRate_Hz, saiAmpFormat.bitWidth, 1U);

    /* Enable interrupt to handle FIFO error */
    SAI_TxEnableInterrupts(BOARD_AMP_SAI, kSAI_FIFOErrorInterruptEnable);

    NVIC_SetPriority(BOARD_AMP_SAI_EDMA_TX_IRQ, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);

    EnableIRQ(BOARD_AMP_SAI_TX_IRQ);

    memset(dummy_txbuffer, 0, 32);
    txfer.dataSize = 32;
    txfer.data     = dummy_txbuffer;
    SAI_TransferSendEDMA(BOARD_AMP_SAI, saiInitHandle.amp_sai_tx_handle, &txfer);
}

extern void SAI_UserTxIRQHandler(void);
extern void SAI_UserRxIRQHandler(void);
void BOARD_AMP_SAI_Tx_IRQ_Handler(void)
{
    if (BOARD_AMP_SAI->TCSR & kSAI_FIFOErrorFlag)
    {
        SAI_UserTxIRQHandler();
    }
}

void BOARD_AMP_SAI_Rx_IRQ_Handler(void)
{
    if (BOARD_AMP_SAI->RCSR & kSAI_FIFOErrorFlag)
    {
        SAI_UserRxIRQHandler();
    }
}
#endif /* SDK_SAI_BASED_COMPONENT_USED */
#endif /* ENABLE_AMPLIFIER */

void BOARD_ConfigMPU(void)
{
    unsigned int dtcSize = (unsigned int)__top_SRAM_DTC - (unsigned int)__base_SRAM_DTC;
    unsigned int itcSize = (unsigned int)__top_SRAM_ITC - (unsigned int)__base_SRAM_ITC;
    unsigned int ocSize_non_cacheable = 
        (unsigned int)__top_SRAM_OC_NON_CACHEABLE - (unsigned int)__base_SRAM_OC_NON_CACHEABLE;
    unsigned int ocSize_cacheable = (unsigned int)__top_SRAM_OC_CACHEABLE - (unsigned int)__base_SRAM_OC_CACHEABLE;


#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t) Image$$RW_m_ncache$$Base;
    uint32_t size          = ((uint32_t) Image$$RW_m_ncache_unused$$Base == nonCacheStart)
                 ? 0
                 : ((uint32_t) Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_SRAM_OC_NON_CACHEABLE;
    extern uint32_t __top_SRAM_OC_NON_CACHEABLE;
    uint32_t nonCacheStart = (uint32_t) (&__base_SRAM_OC_NON_CACHEABLE);
    uint32_t size          = (uint32_t) (&__top_SRAM_OC_NON_CACHEABLE) - nonCacheStart;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t RAM_NC_START[];
    extern uint32_t RAM_NC_SIZE[];
    uint32_t nonCacheStart = (uint32_t) RAM_NC_START;
    uint32_t size          = (uint32_t) RAM_NC_SIZE;
#endif
    volatile uint32_t i = 0;

    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
     * SubRegionDisable, Size)
     * API in mpu_armv7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
     * disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
     * Privileged mode.
     *      Use MACROS defined in mpu_armv7.h:
     * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribute    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     * through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write
     * back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write
     * through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write
     * back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner
     * noncache
     *     1             1           0           0              Normal             shareable       outer and inner
     * noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write
     * back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write
     * back write/read acllocate
     *     2             x           0           0              Device              not shareable
     *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache
     * policy.
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
     * mpu_armv7.h.
     */

    /*
     * Add default region to deny access to whole address space to workaround speculative prefetch.
     * Refer to Arm errata 1013783-B for more details.
     *
     */
    /* Region 0 setting: Instruction access disabled, No data access permission. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(1, ARM_MPU_AP_NONE, 0, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4GB);

    /* Region 1 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 2 setting: Memory with Device type, not shareable,  non-cacheable. */

    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Region 3 setting: Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_8MB);
#endif

    /* Region 4 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 5 setting: ITC */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);
    
    /* Region 6 setting: DTC */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);

    /* Region 7&8 setting: OCRAM */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);
    MPU->RBAR = ARM_MPU_RBAR(8, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 9 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x40000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4MB);

    /* Region 10 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(10, 0x42000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}

static const clock_arm_pll_config_t armPllConfig_BOARD_BoostClock = {
    .loopDivider = 100, /* PLL loop divider, Fout = Fin * 50 */
    .src         = 0,   /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};


static bool clockReduced = false;
static bool clockBoost   = false;

void BOARD_BoostClock(void)
{
    if (!clockBoost)
    {
        clockBoost = true;
        /* Switch AHB_CLK_ROOT from pre_periph_clk to periph_clk2 */
        CLOCK_SetMux(kCLOCK_PeriphMux, 1);

        /* Setting the VDD_SOC to 1.275V. It is necessary to config AHB to 600Mhz. */
        DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0x13);
        /* Waiting for DCDC_STS_DC_OK bit is asserted */
        while (DCDC_REG0_STS_DC_OK_MASK != (DCDC_REG0_STS_DC_OK_MASK & DCDC->REG0))
        {
        }
        CLOCK_InitArmPll(&armPllConfig_BOARD_BoostClock);
        CLOCK_SetMux(kCLOCK_PeriphMux, 0);

        SystemCoreClock = BOARD_BOOSTCLOCK_CORE_CLOCK;

        vPortSetupTimerInterrupt();
    }
}

void BOARD_RevertClock(void)
{
    if (clockReduced || clockBoost)
    {
        clockReduced = false;
        clockBoost = false;

        /* Switch AHB_CLK_ROOT from pre_periph_clk to periph_clk2 */
        CLOCK_SetMux(kCLOCK_PeriphMux, 1);
        CLOCK_SetDiv(kCLOCK_AhbDiv, 0);

        /* Setting the VDD_SOC to 1.15V. It is necessary to config AHB to 528Mhz. */
        DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0xE);
        /* Waiting for DCDC_STS_DC_OK bit is asserted */
        while (DCDC_REG0_STS_DC_OK_MASK != (DCDC_REG0_STS_DC_OK_MASK & DCDC->REG0))
        {
        }
        CLOCK_InitArmPll(&armPllConfig_BOARD_BootClockRUN);
        CLOCK_SetMux(kCLOCK_PeriphMux, 0);

        SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

        vPortSetupTimerInterrupt();
    }
}

void BOARD_ReduceClock(void)
{
    if (!clockReduced)
    {
        clockReduced = true;

        /* Switch AHB_CLK_ROOT from pre_periph_clk to periph_clk2 */
        CLOCK_SetMux(kCLOCK_PeriphMux, 1);
        CLOCK_SetDiv(kCLOCK_AhbDiv, (BOARD_CORE_CLOCK_DIVIDER - 1));

        /* Setting the VDD_SOC to 1.15V. It is necessary to config AHB to 528Mhz. */
        DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0xE);
        /* Waiting for DCDC_STS_DC_OK bit is asserted */
        while (DCDC_REG0_STS_DC_OK_MASK != (DCDC_REG0_STS_DC_OK_MASK & DCDC->REG0))
        {
        }
        CLOCK_InitArmPll(&armPllConfig_BOARD_BootClockRUN);
        CLOCK_SetMux(kCLOCK_PeriphMux, 0);

        SystemCoreClock = BOARD_REDUCEDCLOCK_CORE_CLOCK;

        vPortSetupTimerInterrupt();
    }
}

void BOARD_RelocateVectorTableToRam(void)
{
    uint32_t n;
    uint32_t irqMaskValue;

    irqMaskValue = DisableGlobalIRQ();

    SCB_DisableDCache();
    SCB_DisableICache();

    /* Copy the vector table from ROM to RAM */
    for (n = 0; n < ((uint32_t)0x400) / sizeof(uint32_t); n++)
    {
        g_vectorTable[n] = __VECTOR_TABLE[n];
    }

    /* Set application defined stack pointer */
    volatile unsigned int vStackTop = (unsigned int)&__StackTop;
    g_vectorTable[0]                = vStackTop;

    /* Point the VTOR to the position of vector table */
    SCB->VTOR = (uint32_t)g_vectorTable;
    __DSB();

    SCB_EnableICache();
    SCB_EnableDCache();

    EnableGlobalIRQ(irqMaskValue);
}

uint8_t BUTTON_MSDPressed(void)
{
    /* Check if USB MSD Mode button (SW2) is pushed */
    /* SW2 is connected to GND and uses external pull-up resistor */
    if (0 == GPIO_PinRead(SW2_GPIO, SW2_GPIO_PIN))
        return 1;

    return 0;
}

uint8_t BUTTON_OTWPressed(void)
{
    /* Check if OTW Mode button (SW1) is pushed */
    /* SW1 is connected to GND and uses external pull-up resistor */
    if (0 == GPIO_PinRead(SW1_GPIO, SW1_GPIO_PIN))
        return 1;

    return 0;
}

#if ENABLE_AMPLIFIER
#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED
void *BOARD_GetBoardCodecConfig(void)
{
    return (void *)&boardCodecConfig;
}
#endif /* SDK_SAI_BASED_COMPONENT_USED */
#endif /* ENABLE_AMPLIFIER */

void vPortSetupTimerInterrupt( void )
{
    /* Stop the SysTick. */
    portNVIC_SYSTICK_CTRL_REG = 0UL;

    /* Configure SysTick to interrupt at the requested rate. */
    portNVIC_SYSTICK_LOAD_REG = ( SystemCoreClock / configTICK_RATE_HZ ) - 1UL;
    portNVIC_SYSTICK_CTRL_REG = ( portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT );
}
