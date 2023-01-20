/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
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

#include "LEDWidget.h"
#include "fsl_adapter_gpio.h"

#include <system/SystemClock.h>

#ifndef BOARD_USER_LED_GPIO_INSTANCE
#define BOARD_USER_LED_GPIO_INSTANCE 1
#endif
#ifndef BOARD_USER_LED_GPIO_PIN
#define BOARD_USER_LED_GPIO_PIN (9U)
#endif

GPIO_HANDLE_DEFINE(ledGpioHanlde);

void LEDWidget::Init(void)
{
    hal_gpio_pin_config_t sw_config = {
        .direction = kHAL_GpioDirectionOut,
        .level     = 0U,
        .port      = BOARD_USER_LED_GPIO_INSTANCE,
        .pin       = BOARD_USER_LED_GPIO_PIN,
    };
    mLastChangeTimeUS = 0;
    mBlinkOnTimeMS    = 0;
    mBlinkOffTimeMS   = 0;
    mState            = false;
    HAL_GpioInit((hal_gpio_handle_t) ledGpioHanlde, &sw_config);
    Set(false);
}

void LEDWidget::Invert(void)
{
    Set(!mState);
}

void LEDWidget::Set(bool state)
{
    mLastChangeTimeUS = mBlinkOnTimeMS = mBlinkOffTimeMS = 0;
    DoSet(state);
}

void LEDWidget::Blink(uint32_t changeRateMS)
{
    Blink(changeRateMS, changeRateMS);
}

void LEDWidget::Blink(uint32_t onTimeMS, uint32_t offTimeMS)
{
    mBlinkOnTimeMS  = onTimeMS;
    mBlinkOffTimeMS = offTimeMS;
    Animate();
}

void LEDWidget::Animate()
{
    if (mBlinkOnTimeMS != 0 && mBlinkOffTimeMS != 0)
    {
        int64_t nowUS = chip::System::Platform::Layer::GetClock_Monotonic();
        ;
        int64_t stateDurUS       = ((mState) ? mBlinkOnTimeMS : mBlinkOffTimeMS) * 1000LL;
        int64_t nextChangeTimeUS = mLastChangeTimeUS + stateDurUS;

        if (nowUS > nextChangeTimeUS)
        {
            DoSet(!mState);
            mLastChangeTimeUS = nowUS;
        }
    }
}

void LEDWidget::DoSet(bool state)
{
    mState = state;

    if (state)
    {
        HAL_GpioSetOutput((hal_gpio_handle_t) ledGpioHanlde, 1);
    }
    else
    {
        HAL_GpioSetOutput((hal_gpio_handle_t) ledGpioHanlde, 0);
    }
}
