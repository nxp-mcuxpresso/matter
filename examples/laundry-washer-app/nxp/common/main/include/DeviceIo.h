/*
 *
 *    Copyright (c) 2024 Project CHIP Authors
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
 * @file DeviceIo.h
 *
 * Implementations for the DeviceIo functions
 *
 **/

#pragma once

#include <app/util/af-types.h>
#include <platform/CHIPDeviceLayer.h>


namespace LaundryWasherApp {
class DeviceIo
{
public:
    virtual ~DeviceIo() = default;

    /* Initialize the IO */
    virtual void init(void) = 0;

    /* Flash the light on the board */
    virtual void FlashLight(uint8_t times) = 0;

    /* This returns an instance of this class. */
    static DeviceIo & GetDefaultInstance(void);
};
} // namespace LaundryWasherApp

