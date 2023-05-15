/*
 *
 *    Copyright (c) 2020-2022 Project CHIP Authors
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
 *    @file
 *          Platform-specific configuration overrides for the Chip
 *          Addressing and Routing Module (WARM) on nxp platforms
 *          using the NXP SDK.
 *
 */

#pragma once

// ==================== Platform Adaptations ====================

#ifndef WARM_CONFIG_SUPPORT_THREAD
#define WARM_CONFIG_SUPPORT_THREAD 1
#endif // WARM_CONFIG_SUPPORT_THREAD

#ifndef WARM_CONFIG_SUPPORT_THREAD_ROUTING
#define WARM_CONFIG_SUPPORT_THREAD_ROUTING 0
#endif // WARM_CONFIG_SUPPORT_THREAD_ROUTING

#ifndef WARM_CONFIG_SUPPORT_LEGACY6LOWPAN_NETWORK
#define WARM_CONFIG_SUPPORT_LEGACY6LOWPAN_NETWORK 0
#endif // WARM_CONFIG_SUPPORT_LEGACY6LOWPAN_NETWORK

#ifndef WARM_CONFIG_SUPPORT_WIFI
#define WARM_CONFIG_SUPPORT_WIFI 0
#endif // WARM_CONFIG_SUPPORT_WIFI

#ifndef WARM_CONFIG_SUPPORT_CELLULAR
#define WARM_CONFIG_SUPPORT_CELLULAR 0
#endif // WARM_CONFIG_SUPPORT_CELLULAR

// ========== Platform-specific Configuration Overrides =========

/* none so far */
