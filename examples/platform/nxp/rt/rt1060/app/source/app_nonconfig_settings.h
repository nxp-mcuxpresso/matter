/*
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_WIFI
#define WIFI_IW416_BOARD_AW_AM510_USD

#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#endif /* ENABLE_WIFI */

#if ENABLE_USB_SHELL
#define BOARD_USE_VIRTUALCOM                   1
#define DEBUG_CONSOLE_TRANSFER_NON_BLOCKING    1
#define SERIAL_PORT_TYPE_USBCDC                1
#define USB_CDC_SERIAL_MANAGER_RUN_NO_HOST     1
#endif /* ENABLE_USB_SHELL */
