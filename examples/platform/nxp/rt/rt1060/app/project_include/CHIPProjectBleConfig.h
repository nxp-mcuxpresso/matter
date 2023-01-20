/*
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2020 Google LLC.
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
 * CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
 *
 * Enable support for CHIP-over-BLE (CHIPOBLE).
 */
#ifndef CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
#define CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE 0
#endif

/**
 * CHIP_DEVICE_CONFIG_BLE_FAST_ADVERTISING_TIMEOUT
 *
 * The amount of time in miliseconds after which BLE should change his advertisements
 * from fast interval to slow interval.
 *
 * 30000 (30 secondes).
 */
#define CHIP_DEVICE_CONFIG_BLE_FAST_ADVERTISING_TIMEOUT (30 * 1000)

/**
 * CHIP_DEVICE_CONFIG_BLE_ADVERTISING_TIMEOUT
 *
 * The amount of time in miliseconds after which BLE advertisement should be disabled, counting
 * from the moment of slow advertisement commencement.
 *
 * Defaults to 9000000 (15 minutes).
 */
#define CHIP_DEVICE_CONFIG_BLE_ADVERTISING_TIMEOUT (15 * 60 * 1000)

#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
/*
* Define ZEPHYR to 1 to have the function MapErrorZephyr linked
*/
#define __ZEPHYR__ 1
#endif