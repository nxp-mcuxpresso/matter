/*
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SLN_PWM_DRIVER_FLEXIO_H_
#define SLN_PWM_DRIVER_FLEXIO_H_

#include "fsl_common.h"

#define TIME_TO_RAISE 82U
#define TIME_TO_LOWER 63U

extern uint8_t current_percentage_open;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Initializes FLEXIO3 with the default configuration
 */
status_t FlexPWM_Init(void);

/*!
 * @brief Sets the given duty cycle on the C13 pin configured for PWM
 *
 * @param dutyCycle Target duty cycle to set on PWM
 */
void PWM_SetDutyCycle(uint32_t duty_cycle);

/*!
 * @brief Function to update currentPercentage
 *
 * @param new_percentage_open The new open percentage
 */
void PWM_UpdatePercentageOpen(uint8_t new_percentage_open);

/*!
 * @brief Wrapper function to raise or lower a set of blinds to a certain percentage
 *
 * @param percentage_open Percentage which represents how raised the set of blinds is
 */
void PWM_DriveBlinds(uint8_t percentage_open);

/**
 * @brief Set the frequency and duty cycle for a specific channel
 *
 * @param freq_Hz frequency in Hz
 * @param duty    Duty cycle from 1 to 99
 * @param channel PWM channel
 * @param pwm_pin PWM pin
 */
void _flexio_pwm_init(uint32_t freq_Hz, uint32_t duty, uint8_t channel, uint8_t pwm_pin);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* SLN_PWM_DRIVER_FLEXIO_H_ */
