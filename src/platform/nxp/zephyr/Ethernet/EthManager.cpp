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

#include "EthManager.h"

#include <inet/InetInterface.h>
#include <inet/UDPEndPointImplSockets.h>

#include <lib/support/logging/CHIPLogging.h>

#include <platform/CHIPDeviceLayer.h>
#include <platform/Zephyr/InetUtils.h>

namespace chip {
namespace DeviceLayer {

CHIP_ERROR EthManager::Init()
{
    // TODO: consider moving these to ConnectivityManagerImpl to be prepared for handling multiple interfaces on a single device.
    Inet::UDPEndPointImplSockets::SetMulticastGroupHandler([](Inet::InterfaceId interfaceId, const Inet::IPAddress & address,
                                                              Inet::UDPEndPointImplSockets::MulticastOperation operation) {
        const in6_addr addr = InetUtils::ToZephyrAddr(address);
        net_if * iface      = InetUtils::GetInterface(interfaceId);
        VerifyOrReturnError(iface != nullptr, INET_ERROR_UNKNOWN_INTERFACE);

        if (operation == Inet::UDPEndPointImplSockets::MulticastOperation::kJoin)
        {
            net_if_mcast_addr * maddr = net_if_ipv6_maddr_add(iface, &addr);

            if (maddr && !net_if_ipv6_maddr_is_joined(maddr) && !net_ipv6_is_addr_mcast_link_all_nodes(&addr))
            {
                net_if_ipv6_maddr_join(iface, maddr);
            }
        }
        else if (operation == Inet::UDPEndPointImplSockets::MulticastOperation::kLeave)
        {
            VerifyOrReturnError(net_ipv6_is_addr_mcast_link_all_nodes(&addr) || net_if_ipv6_maddr_rm(iface, &addr),
                                CHIP_ERROR_INVALID_ADDRESS);
        }
        else
        {
            return CHIP_ERROR_INCORRECT_STATE;
        }

        return CHIP_NO_ERROR;
    });

    ChipLogDetail(DeviceLayer, "EthManager has been initialized");
    return CHIP_NO_ERROR;
}
} // namespace DeviceLayer
} // namespace chip
