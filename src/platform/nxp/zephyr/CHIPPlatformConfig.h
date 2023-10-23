/*
 *
 *    Copyright (c) 2023-2023 Project CHIP Authors
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
 *    @file
 *          Platform-specific configuration overrides for CHIP on
 *          Zephyr platform.
 */

#pragma once

// Set MRP retry intervals for Thread and Wi-Fi to test-proven values.
#ifndef CHIP_CONFIG_MRP_LOCAL_ACTIVE_RETRY_INTERVAL
#if CHIP_ENABLE_OPENTHREAD
#define CHIP_CONFIG_MRP_LOCAL_ACTIVE_RETRY_INTERVAL (800_ms32)
#else
#define CHIP_CONFIG_MRP_LOCAL_ACTIVE_RETRY_INTERVAL (1000_ms32)
#endif // CHIP_ENABLE_OPENTHREAD
#endif // CHIP_CONFIG_MRP_LOCAL_ACTIVE_RETRY_INTERVAL

#ifndef CHIP_CONFIG_MRP_LOCAL_IDLE_RETRY_INTERVAL
#if CHIP_ENABLE_OPENTHREAD
#define CHIP_CONFIG_MRP_LOCAL_IDLE_RETRY_INTERVAL (800_ms32)
#else
#define CHIP_CONFIG_MRP_LOCAL_IDLE_RETRY_INTERVAL (1000_ms32)
#endif // CHIP_ENABLE_OPENTHREAD
#endif // CHIP_CONFIG_MRP_LOCAL_IDLE_RETRY_INTERVAL

#include <platform/Zephyr/CHIPPlatformConfig.h>
#include <platform/nxp/common/CHIPNXPPlatformDefaultConfig.h>
