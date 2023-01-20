/*
 *
 *    Copyright (c) 2020-2022 Project CHIP Authors
 *    Copyright (c) 2020 Nest Labs, Inc.
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
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <platform/ConnectivityManager.h>
#include <platform/internal/BLEManager.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>
#include "NetworkCommissioningDriver.h"

#include <platform/internal/GenericConnectivityManagerImpl_UDP.ipp>

#if INET_CONFIG_ENABLE_TCP_ENDPOINT
#include <platform/internal/GenericConnectivityManagerImpl_TCP.ipp>
#endif

#include <lwip/dns.h>
#include <lwip/ip_addr.h>
#include <lwip/nd6.h>
#include <lwip/netif.h>

#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
#include <platform/internal/GenericConnectivityManagerImpl_BLE.ipp>
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_WPA

extern "C" {
#include "wlan.h"
#include "wifi.h"
#include "wm_net.h"
#include <wm_os.h>
}

#include <platform/internal/GenericConnectivityManagerImpl_WiFi.ipp>

#endif /* CHIP_DEVICE_CONFIG_ENABLE_WPA */

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <platform/internal/GenericConnectivityManagerImpl_Thread.ipp>
#endif

using namespace ::chip;
using namespace ::chip::Inet;
using namespace ::chip::System;
using namespace ::chip::TLV;
using namespace ::chip::DeviceLayer::Internal;

namespace chip {
namespace DeviceLayer {

ConnectivityManagerImpl ConnectivityManagerImpl::sInstance;
#if CHIP_DEVICE_CONFIG_ENABLE_WPA
netif_ext_callback_t ConnectivityManagerImpl::sNetifCallback;
#endif /* CHIP_DEVICE_CONFIG_ENABLE_WPA */

CHIP_ERROR ConnectivityManagerImpl::_Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    mWiFiStationMode                = kWiFiStationMode_Disabled;
    mWiFiStationState               = kWiFiStationState_NotConnected;
    mWiFiStationReconnectIntervalMS = CHIP_DEVICE_CONFIG_WIFI_STATION_RECONNECT_INTERVAL;

    // Initialize the generic base classes that require it.
#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
    GenericConnectivityManagerImpl_Thread<ConnectivityManagerImpl>::_Init();
#endif

    SuccessOrExit(err);

exit:
    return err;
}

void ConnectivityManagerImpl::_OnPlatformEvent(const ChipDeviceEvent * event)
{
    // Forward the event to the generic base classes as needed.
#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
    GenericConnectivityManagerImpl_Thread<ConnectivityManagerImpl>::_OnPlatformEvent(event);
#endif
}

#if CHIP_DEVICE_CONFIG_ENABLE_WPA

CHIP_ERROR ConnectivityManagerImpl::_ConnectWiFiNetwork(void)
{
    struct wlan_network network;
    int result;

    // Check not connected
    if (mWiFiStationState != kWiFiStationState_NotConnected)
    {
        ChipLogError(DeviceLayer, "WLAN already connected or connecting");
        return CHIP_ERROR_INCORRECT_STATE;
    }

    // Get previously added network
    result = wlan_get_network(0, &network);
    if (result != WM_SUCCESS)
    {
        ChipLogError(DeviceLayer, "wlan_get_network failed: %d", result);
        return CHIP_ERROR_INCORRECT_STATE;
    }

    // Start connecting
    result = wlan_connect(network.name);
    if (result != WM_SUCCESS)
    {
        ChipLogError(DeviceLayer, "wlan_connect failed: %d", result);
        return CHIP_ERROR_INCORRECT_STATE;
    }

    _SetWiFiStationState(kWiFiStationState_Connecting);

    return CHIP_NO_ERROR;
}

ConnectivityManager::WiFiStationMode ConnectivityManagerImpl::_GetWiFiStationMode()
{
    return mWiFiStationMode;
}

CHIP_ERROR ConnectivityManagerImpl::_SetWiFiStationMode(ConnectivityManager::WiFiStationMode val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    VerifyOrExit(val != ConnectivityManager::kWiFiStationMode_NotSupported, err = CHIP_ERROR_INVALID_ARGUMENT);

    if (mWiFiStationMode != val)
    {
        ChipLogProgress(DeviceLayer, "WiFi station mode change: %s -> %s", WiFiStationModeToStr(mWiFiStationMode),
                        WiFiStationModeToStr(val));
    }

    mWiFiStationMode = val;
    xEventGroupSetBits(mWiFiEventGroup, kWiFiEventGroup_WiFiStationModeBit);
exit:
    return err;
}

void ConnectivityManagerImpl::_SetWiFiStationState(ConnectivityManager::WiFiStationState val)
{
    if (mWiFiStationState != val)
    {
        ChipLogProgress(DeviceLayer, "WiFi station state change: %s -> %s", WiFiStationStateToStr(mWiFiStationState),
                        WiFiStationStateToStr(val));
    }

    mWiFiStationState = val;
}

CHIP_ERROR ConnectivityManagerImpl::_SetWiFiAPMode(WiFiAPMode val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    VerifyOrExit(val != kWiFiAPMode_NotSupported, err = CHIP_ERROR_INVALID_ARGUMENT);

    if (mWiFiAPMode != val)
    {
        ChipLogProgress(DeviceLayer, "WiFi AP mode change: %s -> %s", WiFiAPModeToStr(mWiFiAPMode), WiFiAPModeToStr(val));
    }

    mWiFiAPMode = val;
exit:
    return err;
}

bool ConnectivityManagerImpl::_IsWiFiStationEnabled()
{
    return GetWiFiStationMode() == kWiFiStationMode_Enabled;
}

bool ConnectivityManagerImpl::_IsWiFiStationConnected()
{
    return (mWiFiStationState == kWiFiStationState_Connected);
}

bool ConnectivityManagerImpl::_IsWiFiStationApplicationControlled()
{
    return mWiFiStationMode == ConnectivityManager::kWiFiStationMode_ApplicationControlled;
}

int ConnectivityManagerImpl::_WlanEventCallback(enum wlan_event_reason event, void *data)
{
#if CHIP_DETAIL_LOGGING
    enum wlan_connection_state state;
    int result;

    result = wlan_get_connection_state(&state);
    if (result == WM_SUCCESS)
    {
        ChipLogDetail(DeviceLayer, "WLAN event: %d, WLAN connection state: %d", event, state);
    }
    else
    {
        ChipLogDetail(DeviceLayer, "WLAN event: %d, WLAN connection state: unknown", event);
    }
#endif /* CHIP_DETAIL_LOGGING */

    switch (event)
    {
        case WLAN_REASON_SUCCESS:
            ChipLogProgress(DeviceLayer, "Connected to WLAN network");
            if (sInstance._GetWiFiStationState() == kWiFiStationState_Connecting)
            {
                sInstance._SetWiFiStationState(kWiFiStationState_Connecting_Succeeded);
                sInstance._SetWiFiStationState(kWiFiStationState_Connected);
                NetworkCommissioning::NXPWiFiDriver::GetInstance().OnConnectWiFiNetwork(NetworkCommissioning::Status::kSuccess, CharSpan(), event);
                sInstance.OnStationConnected();
            }
            break;

        case WLAN_REASON_AUTH_SUCCESS:
            ChipLogProgress(DeviceLayer, "Associated to WLAN network");
            break;

        case WLAN_REASON_CONNECT_FAILED:
            ChipLogError(DeviceLayer, "WLAN (re)connect failed");
            sInstance._SetWiFiStationState(kWiFiStationState_Connecting_Failed);
            sInstance._SetWiFiStationState(kWiFiStationState_NotConnected);
            (void)sInstance._ConnectWiFiNetwork();
            break;

        case WLAN_REASON_NETWORK_NOT_FOUND:
            ChipLogError(DeviceLayer, "WLAN network not found");
            NetworkCommissioning::NXPWiFiDriver::GetInstance().OnConnectWiFiNetwork(NetworkCommissioning::Status::kNetworkNotFound, CharSpan(), event);
            break;

        case WLAN_REASON_NETWORK_AUTH_FAILED:
            ChipLogError(DeviceLayer, "Authentication to WLAN network failed");
            NetworkCommissioning::NXPWiFiDriver::GetInstance().OnConnectWiFiNetwork(NetworkCommissioning::Status::kAuthFailure, CharSpan(), event);
            break;

        case WLAN_REASON_LINK_LOST:
            ChipLogError(DeviceLayer, "WLAN link lost");
            if (sInstance._GetWiFiStationState() == kWiFiStationState_Connected)
            {
                sInstance._SetWiFiStationState(kWiFiStationState_NotConnected);
                sInstance.OnStationDisconnected();
                sInstance._SetWiFiStationState(kWiFiStationState_Connecting);
            }
            break;

        case WLAN_REASON_USER_DISCONNECT:
            ChipLogProgress(DeviceLayer, "Disconnected from WLAN network");
            sInstance._SetWiFiStationState(kWiFiStationState_NotConnected);
            sInstance.OnStationDisconnected();
            break;

        case WLAN_REASON_INITIALIZED:
            sInstance._SetWiFiStationState(kWiFiStationState_NotConnected);
            sInstance._SetWiFiStationMode(kWiFiStationMode_Enabled);
            break;

        default:
            break;
    }

    return 0;
}

void ConnectivityManagerImpl::OnStationConnected()
{
    CHIP_ERROR err;
    int result;
    char ip[16];
    struct wlan_ip_config addr;
    struct wlan_network network;

    // Print addresses
    result = wlan_get_address(&addr);
    if (result == WM_SUCCESS)
    {
        net_inet_ntoa(addr.ipv4.address, ip);
        result = wlan_get_current_network(&network);
        if (result == WM_SUCCESS)
        {
            ChipLogProgress(DeviceLayer, "Connected to \"%s\" with IP = [%s]", network.ssid, ip);
#if LWIP_IPV6
            ChipLogProgress(DeviceLayer, " IPv6 Addresses:");
            for (int i = 0; i < CONFIG_MAX_IPV6_ADDRESSES; i++)
            {
                if (network.ip.ipv6[i].addr_state != IP6_ADDR_INVALID)
                {
                    ChipLogProgress(DeviceLayer, "\t%-13s:\t%s (%s)", ipv6_addr_type_to_desc(&network.ip.ipv6[i]),
                        inet6_ntoa(network.ip.ipv6[i].address), ipv6_addr_state_to_desc(network.ip.ipv6[i].addr_state));
                }
            }
#endif /* LWIP_IPV6 */
        }
    }

    ChipDeviceEvent event;
    event.Type                          = DeviceEventType::kWiFiConnectivityChange;
    event.WiFiConnectivityChange.Result = kConnectivity_Established;
    err = PlatformMgr().PostEvent(&event);
    VerifyOrDie(err == CHIP_NO_ERROR);

    UpdateInternetConnectivityState();
}

void ConnectivityManagerImpl::OnStationDisconnected()
{
    CHIP_ERROR err;
    ChipDeviceEvent event;
    event.Type                          = DeviceEventType::kWiFiConnectivityChange;
    event.WiFiConnectivityChange.Result = kConnectivity_Lost;
    err = PlatformMgr().PostEvent(&event);
    VerifyOrDie(err == CHIP_NO_ERROR);

    UpdateInternetConnectivityState();
}

void ConnectivityManagerImpl::UpdateInternetConnectivityState()
{
    bool haveIPv4Conn      = false;
    bool haveIPv6Conn      = false;
    const bool hadIPv4Conn = mFlags.Has(ConnectivityFlags::kHaveIPv4InternetConnectivity);
    const bool hadIPv6Conn = mFlags.Has(ConnectivityFlags::kHaveIPv6InternetConnectivity);
    const ip_addr_t *addr4;
    const ip6_addr_t *addr6;

    // If the WiFi station is currently in the connected state...
    if (mWiFiStationState == kWiFiStationState_Connected)
    {
        // Get the LwIP netif for the WiFi station interface.
        struct netif *netif = static_cast<struct netif *>(net_get_mlan_handle());

        // If the WiFi station interface is up...
        if ((netif != nullptr) && netif_is_up(netif) && netif_is_link_up(netif))
        {
            // Check if a DNS server is currently configured.  If so...
            ip_addr_t dnsServerAddr = *dns_getserver(0);
            if (!ip_addr_isany_val(dnsServerAddr))
            {
                // If the station interface has been assigned an IPv4 address, and has
                // an IPv4 gateway, then presume that the device has IPv4 Internet
                // connectivity.
                if (!ip4_addr_isany_val(*netif_ip4_addr(netif)) && !ip4_addr_isany_val(*netif_ip4_gw(netif)))
                {
                    haveIPv4Conn = true;
                    addr4 = &netif->ip_addr;
                }
            }

            // Search among the IPv6 addresses assigned to the interface for an
            // address that is in the valid state. Search goes backwards because
            // the link-local address is in the first slot and we prefer to report
            // other than the link-local address value if there are multiple addresses.
            for (int i = (LWIP_IPV6_NUM_ADDRESSES - 1); i >= 0; i--)
            {
                if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)))
                {
                    haveIPv6Conn = true;
                    addr6 = netif_ip6_addr(netif, i);
                    break;
                }
            }
        }
    }

    // If the internet connectivity state has changed...
    if (haveIPv4Conn != hadIPv4Conn || haveIPv6Conn != hadIPv6Conn)
    {
        // Update the current state.
        mFlags.Set(ConnectivityFlags::kHaveIPv4InternetConnectivity, haveIPv4Conn)
            .Set(ConnectivityFlags::kHaveIPv6InternetConnectivity, haveIPv6Conn);
    }

    if (haveIPv4Conn != hadIPv4Conn)
    {
        CHIP_ERROR err;
        ChipDeviceEvent event;
        event.Type                            = DeviceEventType::kInternetConnectivityChange;
        event.InternetConnectivityChange.IPv4 = GetConnectivityChange(hadIPv4Conn, haveIPv4Conn);
        event.InternetConnectivityChange.IPv6 = kConnectivity_NoChange;
        if (haveIPv4Conn)
        {
            event.InternetConnectivityChange.ipAddress = IPAddress(*addr4);
        }
        err = PlatformMgr().PostEvent(&event);
        VerifyOrDie(err == CHIP_NO_ERROR);

        ChipLogProgress(DeviceLayer, "%s Internet connectivity %s", "IPv4", (haveIPv4Conn) ? "ESTABLISHED" : "LOST");
    }

    if (haveIPv6Conn != hadIPv6Conn)
    {
        CHIP_ERROR err;
        ChipDeviceEvent event;
        event.Type                            = DeviceEventType::kInternetConnectivityChange;
        event.InternetConnectivityChange.IPv4 = kConnectivity_NoChange;
        event.InternetConnectivityChange.IPv6 = GetConnectivityChange(hadIPv6Conn, haveIPv6Conn);
        if (haveIPv6Conn)
        {
            event.InternetConnectivityChange.ipAddress = IPAddress(*addr6);
        }
        err = PlatformMgr().PostEvent(&event);
        VerifyOrDie(err == CHIP_NO_ERROR);

        ChipLogProgress(DeviceLayer, "%s Internet connectivity %s", "IPv6", (haveIPv6Conn) ? "ESTABLISHED" : "LOST");
    }
}

void ConnectivityManagerImpl::OnStationIPv4AddressAvailable()
{
    CHIP_ERROR err;

    UpdateInternetConnectivityState();

    ChipDeviceEvent event;
    event.Type                           = DeviceEventType::kInterfaceIpAddressChanged;
    event.InterfaceIpAddressChanged.Type = InterfaceIpChangeType::kIpV4_Assigned;
    err = PlatformMgr().PostEvent(&event);
    VerifyOrDie(err == CHIP_NO_ERROR);
}

void ConnectivityManagerImpl::OnStationIPv4AddressLost()
{
    CHIP_ERROR err;

    UpdateInternetConnectivityState();

    ChipDeviceEvent event;
    event.Type                           = DeviceEventType::kInterfaceIpAddressChanged;
    event.InterfaceIpAddressChanged.Type = InterfaceIpChangeType::kIpV4_Lost;
    err = PlatformMgr().PostEvent(&event);
    VerifyOrDie(err == CHIP_NO_ERROR);
}

void ConnectivityManagerImpl::OnIPv6AddressAvailable()
{
    CHIP_ERROR err;

    UpdateInternetConnectivityState();

    ChipDeviceEvent event;
    event.Type                           = DeviceEventType::kInterfaceIpAddressChanged;
    event.InterfaceIpAddressChanged.Type = InterfaceIpChangeType::kIpV6_Assigned;
    err = PlatformMgr().PostEvent(&event);
    VerifyOrDie(err == CHIP_NO_ERROR);
}

void ConnectivityManagerImpl::_NetifExtCallback(struct netif* netif, netif_nsc_reason_t reason, const netif_ext_callback_args_t* args)
{
    struct netif *station_netif;

    ChipLogDetail(DeviceLayer, "_NetifExtCallback: netif=%p, reason=0x%04x", netif, reason);

    station_netif = static_cast<struct netif *>(net_get_mlan_handle());
    if (netif != station_netif)
    {
        return;
    }

    if ((reason & LWIP_NSC_IPV6_SET) == LWIP_NSC_IPV6_SET)
    {
        sInstance.OnIPv6AddressAvailable();
    }
    else if ((reason & LWIP_NSC_IPV6_ADDR_STATE_CHANGED) == LWIP_NSC_IPV6_ADDR_STATE_CHANGED)
    {
        sInstance.UpdateInternetConnectivityState();
    }

    if (((reason & LWIP_NSC_IPV4_SETTINGS_CHANGED) == LWIP_NSC_IPV4_SETTINGS_CHANGED)
        || ((reason & LWIP_NSC_IPV4_ADDRESS_CHANGED) == LWIP_NSC_IPV4_ADDRESS_CHANGED))
    {
        if ((reason & LWIP_NSC_IPV4_ADDR_VALID) == LWIP_NSC_IPV4_ADDR_VALID)
        {
            sInstance.OnStationIPv4AddressAvailable();
        }
        else
        {
            sInstance.OnStationIPv4AddressLost();
        }
    }
}

void ConnectivityManagerImpl::StartWiFiManagement()
{
    struct netif *netif = nullptr;
    EventBits_t bits;
    int32_t result;

    mWiFiEventGroup = xEventGroupCreate();
    if (mWiFiEventGroup == nullptr)
    {
        ChipLogError(DeviceLayer, "Failed to create WLAN event group");
        chipDie();
    }

    xEventGroupClearBits(mWiFiEventGroup, kWiFiEventGroup_WiFiStationModeBit);

    netif = static_cast<struct netif *>(net_get_mlan_handle());
    if (netif != nullptr)
    {
        memset(&ConnectivityManagerImpl::sNetifCallback, 0, sizeof(ConnectivityManagerImpl::sNetifCallback));
        netif_add_ext_callback(&ConnectivityManagerImpl::sNetifCallback, &_NetifExtCallback);
    }

    result = wlan_start(_WlanEventCallback);
    if (result != WM_SUCCESS)
    {
        ChipLogError(DeviceLayer, "Failed to start WLAN Connection Manager");
        chipDie();
    }

    while (true)
    {
        bits = xEventGroupWaitBits(mWiFiEventGroup, kWiFiEventGroup_WiFiStationModeBit, pdTRUE, pdFALSE, portMAX_DELAY);
        if (bits & kWiFiEventGroup_WiFiStationModeBit)
        {
            if (_GetWiFiStationMode() == kWiFiStationMode_Enabled)
            {
                wifi_fw_version_ext_t ver;
                wifi_get_device_firmware_version_ext(&ver);
                ChipLogProgress(DeviceLayer, "WLAN layer initialized, FW version: %s", ver.version_str);
                break;
            }
        }
    }
}
#endif // CHIP_DEVICE_CONFIG_ENABLE_WPA

CHIP_ERROR ConnectivityManagerImpl::ProvisionWiFiNetwork(const char * ssid, const char * key)
{
#if CHIP_DEVICE_CONFIG_ENABLE_WPA
    CHIP_ERROR ret = CHIP_NO_ERROR;
    struct wlan_network network;
    size_t key_len;
    int result;

    memset(&network, 0, sizeof(struct wlan_network));

    strncpy(network.name, ssid, WLAN_NETWORK_NAME_MAX_LENGTH);
    network.name[WLAN_NETWORK_NAME_MAX_LENGTH] = '\0';

    VerifyOrExit(strlen(ssid) <= IEEEtypes_SSID_SIZE, ret = CHIP_ERROR_INVALID_ARGUMENT);

    memcpy(network.ssid, ssid, strlen(ssid));
    network.ip.ipv4.addr_type = ADDR_TYPE_DHCP;
    network.ssid_specific     = 1;

    network.security.type = WLAN_SECURITY_NONE;

    key_len = strlen(key);
    if (key_len > 0)
    {
        network.security.type = WLAN_SECURITY_WILDCARD;
        strncpy(network.security.psk, key, key_len);
        network.security.psk_len = key_len;
    }

    result = wlan_add_network(&network);
    VerifyOrExit(result == WM_SUCCESS, ret = CHIP_ERROR_INTERNAL);

    _SetWiFiStationState(kWiFiStationState_Connecting);

    ChipLogProgress(DeviceLayer, "WLAN connecting to \"%s\"", ssid);
    result = wlan_connect(network.name);
    VerifyOrExit(result == WM_SUCCESS, ret = CHIP_ERROR_INTERNAL);

exit:
    return ret;
#else
    return CHIP_ERROR_NOT_IMPLEMENTED;
#endif
}

} // namespace DeviceLayer
} // namespace chip
