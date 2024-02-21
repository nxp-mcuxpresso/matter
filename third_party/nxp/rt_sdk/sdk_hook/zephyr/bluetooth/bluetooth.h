/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

/*
 * Hook file that is used to allow to build Zephyr/BLEManagerImpl.cpp with NXP SDK on a freeRTOS OS
 */

/*
 * __ZEPHYR__ flag is mandatory to build Zephyr/BLEManagerImpl.cpp file to enable MapErrorZephyr function
 * Nevertheless this flag should not be used in NXP SDK ".h" file when building a freeRTOS OS, therefore undef it when a NXP SDK
 * file is included.
 */
#ifdef __ZEPHYR__
#define ZEPHYR_FLAG_DEFINED 1
#undef __ZEPHYR__
#else
#define ZEPHYR_FLAG_DEFINED 0
#endif

#include <bluetooth/bluetooth.h>

#if ZEPHYR_FLAG_DEFINED
#define __ZEPHYR__
#endif

#if defined(__cplusplus)
}
#endif
