/*
 * Copyright 2019-2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "fsl_dmamux.h"
#endif /* SDK_SAI_BASED_COMPONENT_USED */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#if SLN_SVUI_RD
#define BOARD_NAME "SLN-SVUI-RD"
#else
#define BOARD_NAME "SLN-SVUI-IOT"
#endif /* SLN_SVUI_RD */

/* Frequency for CPU in boost mode */
#define BOARD_BOOSTCLOCK_CORE_CLOCK 600000000U /*!< Core clock frequency: 600000000Hz */

/* Macro used to lower CPU frequency, valid range for divider value: 1-8.
 * Divides CPU default frequency(528MHz) by a specific value.
 * Divider value:1   CPU freq:528MHz
 * Divider value:2   CPU freq:264MHz
 * Divider value:3   CPU freq:176MHz
 * Divider value:4   CPU freq:132MHz
 * Divider value:5   CPU freq:105MHz
 * Divider value:6   CPU freq: 88MHz
 * Divider value:7   CPU freq: 75MHz
 * Divider value:8   CPU freq: 66MHz */
#if (MICS_TYPE == MICS_PDM)
#define BOARD_CORE_CLOCK_DIVIDER 7 /*!< Core clock frequency: 75000000Hz */
#elif (MICS_TYPE == MICS_I2S)
#define BOARD_CORE_CLOCK_DIVIDER 8 /*!< Core clock frequency: 66000000Hz */
#endif /* MICS_TYPE */

/* Frequency for CPU when voice activity is not detected */
#define BOARD_REDUCEDCLOCK_CORE_CLOCK (DEFAULT_SYSTEM_CLOCK/BOARD_CORE_CLOCK_DIVIDER)

/* Macro for DMA MIC Capture */
#define BOARD_PDM_DMA DMA0
/* Macro for SAI MIC Capture */
#define BOARD_PDM_SAI_1 SAI1
#define BOARD_PDM_SAI_2 SAI2
/* Select Audio/Video PLL (786.48 MHz) as SAI clock source */
#define BOARD_PDM_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for SAI clock source */
#define BOARD_PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER (0U) //(1U)
/* Clock divider for SAI clock source */
#define BOARD_PDM_SAI_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of SAI clock */
#define BOARD_PDM_SAI_CLK_FREQ                                                       \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (BOARD_PDM_SAI_CLOCK_SOURCE_DIVIDER + 1U) / \
     (BOARD_PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

#if ENABLE_AMPLIFIER
/* Macro for AMP SAI */
#define BOARD_AMP_SAI SAI3
/* I2C instance and clock */
#define BOARD_AMP_I2C LPI2C1
/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define BOARD_AMP_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define BOARD_AMP_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define BOARD_AMP_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define BOARD_AMP_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define BOARD_AMP_SAI_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define BOARD_AMP_SAI_CLOCK_SOURCE_DIVIDER (3U)
/* Get frequency of sai1 clock */
#define BOARD_AMP_SAI_CLK_FREQ                                                       \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (BOARD_AMP_SAI_CLOCK_SOURCE_DIVIDER + 1U) / \
     (BOARD_AMP_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

#define BOARD_AMP_SAI_TX_IRQ         SAI3_TX_IRQn
#define BOARD_AMP_SAI_RX_IRQ         SAI3_RX_IRQn
#define BOARD_AMP_SAI_Tx_IRQ_Handler SAI3_TX_IRQHandler
#define BOARD_AMP_SAI_Rx_IRQ_Handler SAI3_RX_IRQHandler

#define BOARD_AMP_SAI_EDMA_TX_CH  2U
#define BOARD_AMP_SAI_EDMA_RX_CH  3U
#define BOARD_AMP_SAI_EDMA_TX_REQ kDmaRequestMuxSai3Tx
#define BOARD_AMP_SAI_EDMA_RX_REQ kDmaRequestMuxSai3Rx
#define BOARD_AMP_SAI_EDMA_TX_IRQ DMA2_DMA18_IRQn
#define BOARD_AMP_SAI_EDMA_RX_IRQ DMA3_DMA19_IRQn
#endif /* ENABLE_AMPLIFIER */

#define SAMPLE_RATE (kSAI_SampleRate48KHz)

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART6
#define BOARD_DEBUG_UART_INSTANCE 6U

#define BOARD_DEBUG_UART_CLK_FREQ BOARD_DebugConsoleSrcFreq()

#define BOARD_UART_IRQ         LPUART6_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART6_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE (115200U)
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The USER_LED used for board */
#define RGB_LED_RED_PIN_GPIO   GPIO1
#define RGB_LED_GREEN_PIN_GPIO GPIO1
#define RGB_LED_BLUE_PIN_GPIO  GPIO1
#define RGB_LED_RED_PIN        21
#define RGB_LED_GREEN_PIN      22
#define RGB_LED_BLUE_PIN       23

#ifndef BOARD_USER_LED_GPIO
#define BOARD_USER_LED_GPIO RGB_LED_RED_PIN_GPIO
#endif
#ifndef BOARD_USER_LED_GPIO_PIN
#define BOARD_USER_LED_GPIO_PIN RGB_LED_RED_PIN
#endif

#define LED_FLEXIO_BASEADDR BOARD_INITLED_RED_LED_PERIPHERAL

#define FLEXIO_RED_OUTPUTPIN   BOARD_INITLED_RED_LED_CHANNEL
#define FLEXIO_RED_TIMER_CH    (0U) /* Flexio timer0 used */
#define FLEXIO_GREEN_OUTPUTPIN BOARD_INITLED_GREEN_LED_CHANNEL
#define FLEXIO_GREEN_TIMER_CH  (1U) /* Flexio timer0 used */
#define FLEXIO_BLUE_OUTPUTPIN  BOARD_INITLED_BLUE_LED_CHANNEL
#define FLEXIO_BLUE_TIMER_CH   (2U) /* Flexio timer0 used */
#define FLEXIO_PWM_TIMER_CH    (3U)

#define LED_FLEXIO_CLOCK_FREQUENCY BOARD_BOOTCLOCKRUN_FLEXIO2_CLK_ROOT

/* FLEXIO output PWM frequency */
#define LED_FLEXIO_FREQUENCY (200000U)
#define FLEXIO_MAX_FREQUENCY (LED_FLEXIO_CLOCK_FREQUENCY / 2U)
#define FLEXIO_MIN_FREQUENCY (LED_FLEXIO_CLOCK_FREQUENCY / 256U)

/*! @brief Define the port interrupt number for the board switches */
#define SW1_GPIO     GPIO2
#define SW2_GPIO     GPIO2
#define SW1_GPIO_PIN 4
#define SW2_GPIO_PIN 9

#define BOARD_USER_BUTTON_GPIO        2
#define BOARD_USER_BUTTON_GPIO_PIN    SW1_GPIO_PIN      
#define BOARD_USER_BUTTON_IRQ         GPIO2_Combined_0_15_IRQn
#define BOARD_USER_BUTTON_IRQ_HANDLER GPIO2_Combined_0_15_IRQHandler

/*! @brief Macro for FlexSPI */
#define BOARD_FLEXSPI           FLEXSPI
#define BOARD_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define BOARD_FLEXSPI_CLOCK     kCLOCK_FlexSpi

/*! @brief The hyper flash size */
#if SLN_SVUI_RD
#define BOARD_FLASH_SIZE (0x1000000U)
#else
#define BOARD_FLASH_SIZE (0x2000000U)
#endif /* SLN_SVUI_RD */

/*! @brief The ENET PHY address. */
#define BOARD_ENET0_PHY_ADDRESS (0x01U) /* Phy address of enet port 0. */

/* USB PHY configuration */
#define BOARD_USB_PHY_D_CAL     (0x0CU)
#define BOARD_USB_PHY_TXCAL45DP (0x06U)
#define BOARD_USB_PHY_TXCAL45DM (0x06U)

#define BOARD_ARDUINO_INT_IRQ           (GPIO1_INT3_IRQn)
#define BOARD_ARDUINO_I2C_IRQ           (LPI2C1_IRQn)
#define BOARD_ARDUINO_I2C_INDEX         (1)
#define BOARD_USDHC1_BASEADDR           USDHC1
#define BOARD_USDHC2_BASEADDR           USDHC2
#define BOARD_USDHC_CD_GPIO_BASE        GPIO2
#define BOARD_USDHC_CD_GPIO_PIN         28
#define BOARD_USDHC_CD_PORT_IRQ         GPIO2_Combined_16_31_IRQn
#define BOARD_USDHC_CD_PORT_IRQ_HANDLER GPIO2_Combined_16_31_IRQHandler

#define BOARD_USDHC_CD_STATUS() (GPIO_PinRead(BOARD_USDHC_CD_GPIO_BASE, BOARD_USDHC_CD_GPIO_PIN))

#define BOARD_USDHC_CD_INTERRUPT_STATUS()    (GPIO_PortGetInterruptFlags(BOARD_USDHC_CD_GPIO_BASE))
#define BOARD_USDHC_CD_CLEAR_INTERRUPT(flag) (GPIO_PortClearInterruptFlags(BOARD_USDHC_CD_GPIO_BASE, flag))

#define BOARD_USDHC_CD_GPIO_INIT()                                                          \
    {                                                                                       \
        gpio_pin_config_t sw_config = {                                                     \
            kGPIO_DigitalInput,                                                             \
            0,                                                                              \
            kGPIO_IntRisingOrFallingEdge,                                                   \
        };                                                                                  \
        GPIO_PinInit(BOARD_USDHC_CD_GPIO_BASE, BOARD_USDHC_CD_GPIO_PIN, &sw_config);        \
        GPIO_PortEnableInterrupts(BOARD_USDHC_CD_GPIO_BASE, 1U << BOARD_USDHC_CD_GPIO_PIN); \
        GPIO_PortClearInterruptFlags(BOARD_USDHC_CD_GPIO_BASE, ~0);                         \
    }
#define BOARD_HAS_SDCARD              (1U)
#define BOARD_SD_POWER_RESET_GPIO     (GPIO5)
#define BOARD_SD_POWER_RESET_GPIO_PIN (1U)

/* @brief Wifi WL_REG_ON Pins*/
#define BOARD_INITPINS_WL_REG_ON_GPIO GPIO5 // former BOARD_WL_REG_ON_port
#define BOARD_INITPINS_WL_REG_ON_PIN  1U    // former BOARD_WL_REG_ON_pin

#define BOARD_WL_OOB_IRQ_port    GPIO5
#define BOARD_WL_OOB_IRQ_pin     0U
#define BOARD_WL_OOB_IRQ         GPIO5_Combined_0_15_IRQn
#define BOARD_WL_OOB_IRQ_Handler GPIO5_Combined_0_15_IRQHandler

/* @brief BLE PWR Pins*/
#define BOARD_INITPINS_BT_REG_ON_GPIO GPIO2 // former  BOARD_BT_REG_ON_port
#define BOARD_INITPINS_BT_REG_ON_PIN  30U   // former  BOARD_BT_REG_ON_pin

#define BOARD_BT_HOST_WAKE_port GPIO2
#define BOARD_BT_HOST_WAKE_pin  31U

#define BOARD_USDHC_CARD_INSERT_CD_LEVEL (0U)

#define BOARD_USDHC_MMCCARD_POWER_CONTROL(state)

#define BOARD_USDHC_MMCCARD_POWER_CONTROL_INIT()                                            \
    {                                                                                       \
        gpio_pin_config_t sw_config = {                                                     \
            kGPIO_DigitalOutput,                                                            \
            0,                                                                              \
            kGPIO_NoIntmode,                                                                \
        };                                                                                  \
        GPIO_PinInit(BOARD_SD_POWER_RESET_GPIO, BOARD_SD_POWER_RESET_GPIO_PIN, &sw_config); \
        GPIO_PinWrite(BOARD_SD_POWER_RESET_GPIO, BOARD_SD_POWER_RESET_GPIO_PIN, true);      \
    }

#define BOARD_USDHC_SDCARD_POWER_CONTROL_INIT()                                             \
    {                                                                                       \
        gpio_pin_config_t sw_config = {                                                     \
            kGPIO_DigitalOutput,                                                            \
            0,                                                                              \
            kGPIO_NoIntmode,                                                                \
        };                                                                                  \
        GPIO_PinInit(BOARD_SD_POWER_RESET_GPIO, BOARD_SD_POWER_RESET_GPIO_PIN, &sw_config); \
    }

#define BOARD_USDHC_SDCARD_POWER_CONTROL(state) \
    (GPIO_PinWrite(BOARD_SD_POWER_RESET_GPIO, BOARD_SD_POWER_RESET_GPIO_PIN, state))

#define BOARD_USDHC1_CLK_FREQ (CLOCK_GetSysPfdFreq(kCLOCK_Pfd0) / (CLOCK_GetDiv(kCLOCK_Usdhc1Div) + 1U))
#define BOARD_USDHC2_CLK_FREQ (CLOCK_GetSysPfdFreq(kCLOCK_Pfd0) / (CLOCK_GetDiv(kCLOCK_Usdhc2Div) + 1U))

#define BOARD_SD_HOST_BASEADDR BOARD_USDHC1_BASEADDR
#define BOARD_SD_HOST_CLK_FREQ BOARD_USDHC1_CLK_FREQ
#define BOARD_SD_HOST_IRQ      USDHC1_IRQn

#define BOARD_MMC_HOST_BASEADDR BOARD_USDHC2_BASEADDR
#define BOARD_MMC_HOST_CLK_FREQ BOARD_USDHC2_CLK_FREQ
#define BOARD_MMC_HOST_IRQ      USDHC2_IRQn
#define BOARD_MMC_VCCQ_SUPPLY   kMMC_VoltageWindow170to195
#define BOARD_MMC_VCC_SUPPLY    kMMC_VoltageWindows270to360
/* we are using the BB SD socket to DEMO the MMC example,but the
 * SD socket provide 4bit bus only, so we define this macro to avoid
 * 8bit data bus test
 */
#define BOARD_MMC_SUPPORT_8BIT_BUS (1U)

#define BOARD_SD_HOST_SUPPORT_SDR104_FREQ (200000000U)
#define BOARD_SD_HOST_SUPPORT_HS200_FREQ  (180000000U)
/* define for SD/MMC config IO driver strength dynamic */
#define BOARD_SD_Pin_Config(speed, strength)                                                      \
    {                                                                                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD,                                      \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK,                                      \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(0) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
    }

#define BOARD_MMC_PIN_CONFIG(speed, strength)                                                     \
    {                                                                                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_USDHC2_CMD,                                      \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_USDHC2_CLK,                                      \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(0) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_03_USDHC2_DATA0,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_02_USDHC2_DATA1,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_01_USDHC2_DATA2,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_00_USDHC2_DATA3,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_08_USDHC2_DATA4,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_09_USDHC2_DATA5,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_USDHC2_DATA6,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_USDHC2_DATA7,                                    \
                            IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | \
                                IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |   \
                                IOMUXC_SW_PAD_CTL_PAD_DSE(strength));                             \
    }

/*! @brief The WIFI-QCA shield pin. */
#define BOARD_INITGT202SHIELD_PWRON_GPIO      GPIO1               /*!< GPIO device name: GPIO */
#define BOARD_INITGT202SHIELD_PWRON_PORT      1U                  /*!< PORT device index: 1 */
#define BOARD_INITGT202SHIELD_PWRON_GPIO_PIN  3U                  /*!< PIO4 pin index: 3 */
#define BOARD_INITGT202SHIELD_PWRON_PIN_NAME  GPIO1_3             /*!< Pin name */
#define BOARD_INITGT202SHIELD_PWRON_LABEL     "PWRON"             /*!< Label */
#define BOARD_INITGT202SHIELD_PWRON_NAME      "PWRON"             /*!< Identifier name */
#define BOARD_INITGT202SHIELD_PWRON_DIRECTION kGPIO_DigitalOutput /*!< Direction */

#define BOARD_INITGT202SHIELD_IRQ_GPIO      GPIO1              /*!< GPIO device name: GPIO */
#define BOARD_INITGT202SHIELD_IRQ_PORT      1U                 /*!< PORT device index: 1 */
#define BOARD_INITGT202SHIELD_IRQ_GPIO_PIN  19U                /*!< PIO1 pin index: 19 */
#define BOARD_INITGT202SHIELD_IRQ_PIN_NAME  GPIO1_19           /*!< Pin name */
#define BOARD_INITGT202SHIELD_IRQ_LABEL     "IRQ"              /*!< Label */
#define BOARD_INITGT202SHIELD_IRQ_NAME      "IRQ"              /*!< Identifier name */
#define BOARD_INITGT202SHIELD_IRQ_DIRECTION kGPIO_DigitalInput /*!< Direction */

/* @Brief Board Bluetooth HCI UART configuration */
#define BOARD_BT_UART_BASEADDR    LPUART1
#define BOARD_BT_UART_BAUDRATE    3000000
#define BOARD_BT_UART_INSTANCE    1
#define BOARD_BT_UART_CLK_FREQ    BOARD_DebugConsoleSrcFreq()
#define BOARD_BT_UART_IRQ         LPUART1_IRQn
#define BOARD_BT_UART_IRQ_HANDLER LPUART1_IRQHandler

#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED

typedef struct
{
    edma_handle_t *amp_dma_tx_handle;
    edma_handle_t *amp_dma_rx_handle;
    sai_edma_handle_t *amp_sai_tx_handle;
    sai_edma_handle_t *amp_sai_rx_handle;
    sai_edma_callback_t sai_tx_callback;
    sai_edma_callback_t sai_rx_callback;
} sai_init_handle_t;

#endif /* SDK_SAI_BASED_COMPONENT_USED */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
uint32_t BOARD_DebugConsoleSrcFreq(void);

void BOARD_RelocateVectorTableToRam(void);

void BOARD_InitDebugConsole(void);

void *BOARD_GetBoardCodecConfig(void);

/*!
 * @brief Boost ARM core clock PLL1@600MHz
 */
void BOARD_BoostClock(void);
/*!
 * @brief Revert ARM core clock to PLL2@528MHz
 */
void BOARD_RevertClock(void);
/*!
 * @brief Reduce ARM core clock to a specific value depending on BOARD_CORE_CLOCK_DIVIDER
 */
void BOARD_ReduceClock(void);

void BOARD_ConfigMPU(void);

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz);
status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subaddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize);
status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subaddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize);
status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subaddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize);
status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subaddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize);

#endif /* SDK_I2C_BASED_COMPONENT_USED */

#if ENABLE_AMPLIFIER
#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED
void BOARD_SAI_Init(sai_init_handle_t saiInitHandle);
#endif
#endif /* ENABLE_AMPLIFIER */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
