/*
 *  Copyright 2023-2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __WIFICONNECT_H__
#define __WIFICONNECT_H__
namespace chip {
namespace NXP {
namespace App {
/* Creates a dedicated Task responsible for connecting to a WiFi network */
void WifiConnectTaskCreate(void);
} // namespace App
} // namespace NXP
} // namespace chip
#endif /* __WIFICONNECT_H__ */
