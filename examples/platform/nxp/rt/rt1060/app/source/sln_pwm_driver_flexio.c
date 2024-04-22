/*
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "fsl_flexio.h"
#include "sln_pwm_driver_flexio.h"

uint8_t current_percentage_open = 100U;

status_t FlexPWM_Init(void)
{
    flexio_config_t fxioUserConfig;

    FLEXIO_GetDefaultConfig(&fxioUserConfig);
    FLEXIO_Init(FLEXIO3, &fxioUserConfig);

    return kStatus_Success;
}

void PWM_SetDutyCycle(uint32_t duty_cycle)
{
    _flexio_pwm_init(LED_FLEXIO_FREQUENCY, duty_cycle, FLEXIO_PWM_TIMER_CH, 27U);
    if (duty_cycle != 0)
    {
        LED_FLEXIO_BASEADDR->TIMCTL[FLEXIO_PWM_TIMER_CH] |= FLEXIO_TIMCTL_TIMOD(kFLEXIO_TimerModeDual8BitPWM);
    }
}

void PWM_UpdatePercentageOpen(uint8_t new_percentage_open)
{
    current_percentage_open = new_percentage_open;
}

void PWM_DriveBlinds(uint8_t percentage_open)
{
    int8_t percentage_to_open;
    percentage_to_open = (int8_t)current_percentage_open - (int8_t)percentage_open;

    if (percentage_to_open == 0)
    {
        return;
    }
    else if (percentage_to_open < 0)
    {
        /*  IN1 for H-Bridge  */
        GPIO_PinWrite(GPIO1, 3U, 1U);
        /*  IN2 for H-Bridge  */
        GPIO_PinWrite(GPIO2, 19U, 0U);
        PWM_SetDutyCycle((uint32_t)99);
        vTaskDelay(TIME_TO_RAISE * percentage_to_open * (-1));
        GPIO_PinWrite(GPIO1, 3U, 0U);
        PWM_SetDutyCycle((uint32_t)0);
    }
    else if (percentage_to_open > 0)
    {
        /*  IN1 for H-Bridge  */
        GPIO_PinWrite(GPIO1, 3U, 0U);
        /*  IN2 for H-Bridge  */
        GPIO_PinWrite(GPIO2, 19U, 1U);
        PWM_SetDutyCycle((uint32_t)99);
        vTaskDelay(TIME_TO_LOWER * percentage_to_open);
        GPIO_PinWrite(GPIO2, 19U, 0U);
        PWM_SetDutyCycle((uint32_t)0);
    }
    current_percentage_open = percentage_open;
    return;
}
