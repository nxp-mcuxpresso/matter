/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MATTER_MBEDTLS_CONFIG_H
#define MATTER_MBEDTLS_CONFIG_H

/* SDK mbetdls config include */
#include "ksdk_mbedtls_config.h"

#ifdef USE_SDK_2_13
#undef MBEDTLS_FREESCALE_FREERTOS_CALLOC_ALT
#endif

#endif // MATTER_MBEDTLS_CONFIG_H
