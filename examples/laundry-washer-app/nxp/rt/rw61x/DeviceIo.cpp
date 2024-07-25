/*
 *
 *    Copyright (c) 2024 Project CHIP Authors
 *    Copyright 2024 NXP
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 * @file DeviceIo.cpp
 *
 * Implements the DeviceIo functions
 *
 **/
#include "DeviceIo.h"

extern "C" {
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_io_mux.h"
#include "fsl_power.h"
#include "fsl_clock.h"
}

static void _BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-M33[cm33] */
   IO_MUX_SetPinMux(IO_MUX_FC3_USART_DATA);
   IO_MUX_SetPinMux(IO_MUX_GPIO1);
   IO_MUX_SetPinMux(IO_MUX_GPIO25);
}

#define BOARD_LED_BLUE_GPIO_PORT 0U
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN 1U
#endif

#define APP_BOARD_TEST_LED_PORT BOARD_LED_BLUE_GPIO_PORT
#define APP_BOARD_TEST_LED_PIN  BOARD_LED_BLUE_GPIO_PIN

#ifndef os_thread_sleep
#define os_thread_sleep(ticks) vTaskDelay(ticks)
#endif
#ifndef os_msec_to_ticks
#define os_msec_to_ticks(msecs) ((msecs) / (portTICK_PERIOD_MS))
#endif

using namespace LaundryWasherApp;

class RW61xDeviceIo : public DeviceIo
{
public:
    /* Initialize the IO */
    void init(void);

    /* Flash the light on the board */
    void FlashLight(uint8_t times);
};

void RW61xDeviceIo::init(void)
{
    /* Initialize the GPIO pins */
    IO_MUX_SetPinMux(IO_MUX_FC3_USART_DATA);
    IO_MUX_SetPinMux(IO_MUX_GPIO1);
    IO_MUX_SetPinMux(IO_MUX_GPIO25);

    uint32_t port_state = 0;
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    GPIO_PortInit(GPIO, APP_BOARD_TEST_LED_PORT);
    GPIO_PinInit(GPIO, APP_BOARD_TEST_LED_PORT, APP_BOARD_TEST_LED_PIN, &led_config);
    GPIO_PinWrite(GPIO, APP_BOARD_TEST_LED_PORT, APP_BOARD_TEST_LED_PIN, 1);
    return;
}

void RW61xDeviceIo::FlashLight(uint8_t times)
{
    for (uint8_t i=0 ; i<times ; i++) {
        GPIO_PortToggle(GPIO, APP_BOARD_TEST_LED_PORT, 1u << APP_BOARD_TEST_LED_PIN);
        os_thread_sleep(os_msec_to_ticks(1000));
        GPIO_PortToggle(GPIO, APP_BOARD_TEST_LED_PORT, 1u << APP_BOARD_TEST_LED_PIN);
	os_thread_sleep(os_msec_to_ticks(1000));
    }
    return;
}

DeviceIo & DeviceIo::GetDefaultInstance()
{
    static RW61xDeviceIo sDeviceIoInstance;
    return sDeviceIoInstance;
}

