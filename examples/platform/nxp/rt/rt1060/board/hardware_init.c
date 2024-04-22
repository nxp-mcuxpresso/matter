/*
 * Copyright 2022, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "clock_config.h"
#include "fsl_dmamux.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart_edma.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "sln_rgb_led_driver.h"
#include "sln_main.h"

#if (defined(K32W061_TRANSCEIVER) && CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE) ||                                                        \
    (defined(WIFI_IW416_BOARD_AW_AM510_USD) && CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE) ||                                              \
    (defined(WIFI_IW416_BOARD_AW_AM457_USD) && CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE) ||                                              \
    (defined(WIFI_IW612_BOARD_MURATA_2EL_M2) && CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE)
#include "controller_hci_uart.h"
#endif

/*${header:end}*/

/*${variable:start}*/
static bool isInitialize = false;
/*${variable:end}*/

/*${function:start}*/

static void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

static void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = { .enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1 };
    CLOCK_InitEnetPll(&config);
}

void BOARD_InitHardware(void)
{
    if (!isInitialize)
    {
        BOARD_ConfigMPU();
        BOARD_InitBootPins();
        BOARD_InitBootClocks();
        BOARD_InitBootPeripherals();
        SCB_DisableDCache();

#if !CHIP_DEVICE_CONFIG_ENABLE_THREAD
        gpio_pin_config_t gpio_config = { kGPIO_DigitalOutput, 0, kGPIO_NoIntmode };
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
        DMAMUX_Type * dmaMuxBases[] = DMAMUX_BASE_PTRS;
        edma_config_t config;
        DMA_Type * dmaBases[] = DMA_BASE_PTRS;
        DMAMUX_Init(dmaMuxBases[0]);
        EDMA_GetDefaultConfig(&config);
        EDMA_Init(dmaBases[0], &config);
#endif

#if !CHIP_DEVICE_CONFIG_ENABLE_WPA
        BOARD_InitModuleClock();
#endif

        delay();
        GPIO_WritePinOutput(GPIO5, 1, 1);

        sln_main();
#endif
        isInitialize = true;
    }
}

#if ((defined(WIFI_IW416_BOARD_AW_AM510_USD) || defined(WIFI_IW416_BOARD_AW_AM457_USD)) && CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE)
int controller_hci_uart_get_configuration(controller_hci_uart_config_t * config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->clockSrc        = BOARD_BT_UART_CLK_FREQ;
    config->defaultBaudrate = BOARD_BT_UART_BAUDRATE;
    config->runningBaudrate = BOARD_BT_UART_BAUDRATE;
    config->instance        = BOARD_BT_UART_INSTANCE;
    config->enableRxRTS     = 1u;
    config->enableTxCTS     = 1u;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    config->dma_instance     = 0U;
    config->rx_channel       = 4U;
    config->tx_channel       = 5U;
    config->dma_mux_instance = 0U;
    config->rx_request       = kDmaRequestMuxLPUART1Rx;
    config->tx_request       = kDmaRequestMuxLPUART1Tx;
#endif
    return 0;
}
#elif (defined(K32W061_TRANSCEIVER) && CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE)
int controller_hci_uart_get_configuration(controller_hci_uart_config_t * config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->clockSrc        = BOARD_BT_UART_CLK_FREQ;
    config->defaultBaudrate = 115200u;
    config->runningBaudrate = 115200u;
    config->instance        = BOARD_BT_UART_INSTANCE;
    config->enableRxRTS     = 1u;
    config->enableTxCTS     = 1u;
    return 0;
}
#elif (defined(WIFI_IW612_BOARD_MURATA_2EL_M2) && CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE)
int controller_hci_uart_get_configuration(controller_hci_uart_config_t * config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->clockSrc        = BOARD_BT_UART_CLK_FREQ;
    config->defaultBaudrate = 115200u;
    config->runningBaudrate = BOARD_BT_UART_BAUDRATE;
    config->instance        = BOARD_BT_UART_INSTANCE;
    config->enableRxRTS     = 1u;
    config->enableTxCTS     = 1u;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    config->dma_instance     = 0U;
    config->rx_channel       = 4U;
    config->tx_channel       = 5U;
    config->dma_mux_instance = 0U;
    config->rx_request       = kDmaRequestMuxLPUART3Rx;
    config->tx_request       = kDmaRequestMuxLPUART3Tx;
#endif
    return 0;
}
#endif

/*${function:end}*/
