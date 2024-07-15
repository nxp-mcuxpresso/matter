/*
 *    Copyright 2024 Project CHIP Authors
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

#pragma once

/* ---- App Config ---- */
#define APP_DEVICE_TYPE_ENDPOINT 1
#define APP_CLUSTER_ATTRIBUTE chip::app::Clusters::OnOff::Attributes::OnOff

/* ---- Button Manager Config ---- */
#define BUTTON_MANAGER_FACTORY_RESET_TIMEOUT_MS 6000

/* ---- LED Manager Config ---- */
#define LED_MANAGER_STATUS_LED_INDEX 0
#define LED_MANAGER_LIGHT_LED_INDEX 1

/*
 * The status LED and the external flash CS pin are wired together.
 * The OTA image writing may fail if used together.
 */
#ifndef CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR
#define LED_MANAGER_ENABLE_STATUS_LED 1
#endif
