/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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

#include "lib/dnssd/platform/Dnssd.h"
#include "lwip/apps/mdns.h"
#include "DnssdImpl.h"

#if CHIP_DEVICE_CONFIG_ENABLE_WPA
extern "C" {
#include <wm_net.h>
}
#endif /* CHIP_DEVICE_CONFIG_ENABLE_WPA */

using namespace chip::Dnssd;

namespace chip {
namespace Dnssd {

struct text_entry
{
    text_entry() :
        data(nullptr),
        next(nullptr)
    {}

    ~text_entry()
    {
        if (data != nullptr)
        {
            delete data;
        }

        if (next != nullptr)
        {
            delete next;
        }
    }

    char *data;
    size_t size;
    struct text_entry *next;
};

/* Callback function to generate TXT Dnssd record for service. */
static void ChipDnssdGetText(struct mdns_service *service, void *txt_userdata)
{
    struct text_entry *text_entry;
    err_t err;

    ChipLogDetail(Discovery, "%s", __func__);

    text_entry = (struct text_entry *)txt_userdata;

    while (text_entry != nullptr)
    {
        err = mdns_resp_add_service_txtitem(service, text_entry->data, text_entry->size);
        if (err != ERR_OK)
        {
            ChipLogError(Discovery, "mdns_resp_add_service_txtitem failed: %d", err);
        }
        text_entry = text_entry->next;
    }
}

static struct netif *ChipDnssdGetValidNetif()
{
    struct netif *netif = nullptr;

#if CHIP_DEVICE_CONFIG_ENABLE_WPA
    netif = static_cast<struct netif *>(net_get_mlan_handle());
#else
    netif = netif_default;
#endif /* CHIP_DEVICE_CONFIG_ENABLE_WPA */

    if ((netif != nullptr) && !netif_is_link_up(netif))
    {
        netif = nullptr;
    }

    return netif;
}

CHIP_ERROR ChipDnssdInit(DnssdAsyncReturnCallback successCallback, DnssdAsyncReturnCallback errorCallback, void * context)
{
    ChipLogDetail(Discovery, "%s", __func__);

    mdns_resp_init();
    successCallback(context, CHIP_NO_ERROR);

    return CHIP_NO_ERROR;
}

CHIP_ERROR ChipDnssdPublishService(const DnssdService * service, DnssdPublishCallback callback, void * context)
{
    struct netif *netif = ChipDnssdGetValidNetif();
    enum mdns_sd_proto proto;
    struct text_entry *text_entries = nullptr;
    struct text_entry *text_entry_last = nullptr;
    struct text_entry *text_entry_current = nullptr;
    char *data;
    err_t err;
    s8_t slot;
    int len;
    int idx;

/* TODO:
 * Calling the function again with the same service name, type, protocol,
 * interface and port but different text should update the text published. */

    ChipLogDetail(Discovery, "%s(hostname=\"%s\", name=\"%s\", type=\"%s\")", __func__, service->mHostName, service->mName, service->mType);
    if (service->mSubTypeSize > 0U)
    {
        /* Subtypes are not implemented in lwIP */
        ChipLogError(Discovery, "%s: registering service subtypes is not supported (%u requested)", __func__, service->mSubTypeSize);
    }

    if (netif == nullptr)
    {
        ChipLogError(Discovery, "%s: no valid network interface", __func__);
        return CHIP_NO_ERROR;
    }

    if (!mdns_resp_netif_active(netif))
    {
        err = mdns_resp_add_netif(netif, service->mHostName);
        if (err != ERR_OK)
        {
            ChipLogError(Discovery, "mdns_resp_add_netif failed, error=%d", err);
            return CHIP_ERROR_INTERNAL;
        }
    }

    if (service->mProtocol == chip::Dnssd::DnssdServiceProtocol::kDnssdProtocolUdp)
    {
        proto = DNSSD_PROTO_UDP;
    }
    else if (service->mProtocol == chip::Dnssd::DnssdServiceProtocol::kDnssdProtocolTcp)
    {
        proto = DNSSD_PROTO_TCP;
    }
    else
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    /* Prepare TXT entries for later usage by ChipDnssdGetText callback */
    for (int i = 0; i < service->mTextEntrySize; i++)
    {
        text_entry_current = new text_entry;
        if (text_entry_current == nullptr)
        {
            ChipLogError(Discovery, "text_entry allocation failed");
            if (text_entries != nullptr)
            {
                delete text_entries;
            }

            return CHIP_ERROR_NO_MEMORY;
        }

        if (i == 0)
        {
            text_entries = text_entry_current;
        }
        else
        {
            text_entry_last->next = text_entry_current;
        }

        text_entry_last = text_entry_current;

        len = strlen(service->mTextEntries[i].mKey) + service->mTextEntries[i].mDataSize + 2; /* "mKey=mData\0" */
        data = new char[len];
        if (data == nullptr)
        {
            ChipLogError(Discovery, "text_entry data allocation failed");
            delete text_entries;

            return CHIP_ERROR_NO_MEMORY;
        }
        text_entry_current->data = data;

        idx = snprintf(data, len, "%s=", service->mTextEntries[i].mKey);
        if ((idx < 0) || (idx >= len))
        {
            ChipLogError(Discovery, "snprintf failed, error=%d", idx);
            delete text_entries;

            return CHIP_ERROR_INTERNAL;
        }

        memcpy(&data[idx], &service->mTextEntries[i].mData[0], service->mTextEntries[i].mDataSize);
        idx += service->mTextEntries[i].mDataSize;
        text_entry_current->size = idx;
        data[idx] = '\0';        

        ChipLogDetail(Discovery, "%s: TXT entry \"%s\"", __func__, data);
    }

    slot = mdns_resp_add_service(netif, service->mName, service->mType, proto, service->mPort, ChipDnssdGetText, (void *)text_entries);
    if (slot < 0)
    {
        ChipLogError(Discovery, "mdns_resp_add_service failed, error=%d", slot);
        delete text_entries;

        return CHIP_ERROR_INTERNAL;
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR ChipDnssdBrowse(const char * type, DnssdServiceProtocol protocol, chip::Inet::IPAddressType addressType,
                           chip::Inet::InterfaceId interface, DnssdBrowseCallback callback, void * context,
                           intptr_t * browseIdentifier)
{
    ChipLogDetail(Discovery, "%s", __func__);
    //return CHIP_ERROR_NOT_IMPLEMENTED;
    return CHIP_NO_ERROR;
}

CHIP_ERROR ChipDnssdStopBrowse(intptr_t browseIdentifier)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR ChipDnssdResolve(DnssdService * service, chip::Inet::InterfaceId interface, DnssdResolveCallback callback, void * context)
{
    ChipLogDetail(Discovery, "%s", __func__);
    //return CHIP_ERROR_NOT_IMPLEMENTED;
    return CHIP_NO_ERROR;
}

CHIP_ERROR ChipDnssdFinalizeServiceUpdate()
{
    ChipLogDetail(Discovery, "%s", __func__);
    return CHIP_NO_ERROR;
}

CHIP_ERROR ChipDnssdRemoveServices()
{
    struct netif *netif = ChipDnssdGetValidNetif();
    struct text_entry *entry;
    err_t err;

    ChipLogDetail(Discovery, "%s", __func__);

    if (netif == nullptr)
    {
        ChipLogError(Discovery, "%s: no valid network interface", __func__);
        return CHIP_NO_ERROR;
    }

    if (!mdns_resp_netif_active(netif))
    {
        ChipLogProgress(Discovery, "%s: responder not active", __func__);
        return CHIP_NO_ERROR;
    }

    for (int slot = 0; slot < MDNS_MAX_SERVICES; slot++)
    {
        entry = (struct text_entry *)mdns_get_service_txt_userdata(netif, slot);
        if (entry != nullptr)
        {
            mdns_resp_del_service(netif, slot);
            delete entry;
        }
    }

    return CHIP_NO_ERROR;
}

void ChipDnssdShutdown()
{
    struct netif *netif = ChipDnssdGetValidNetif();
    struct text_entry *entry;
    err_t err;

    ChipLogDetail(Discovery, "%s", __func__);

    do
    {
        if (netif == nullptr)
        {
            ChipLogError(Discovery, "%s: no valid network interface", __func__);
            break;
        }
        if (!mdns_resp_netif_active(netif))
        {
            ChipLogProgress(Discovery, "%s: responder not active", __func__);
            break;
        }
        for (int slot = 0; slot < MDNS_MAX_SERVICES; slot++)
        {
            entry = (struct text_entry *)mdns_get_service_txt_userdata(netif, slot);
            if (entry != nullptr)
            {
                mdns_resp_del_service(netif, slot);
                delete entry;
            }
        }
        err = mdns_resp_remove_netif(netif);
        if (err != ERR_OK)
        {
            ChipLogError(Discovery, "mdns_resp_remove_netif failed, error=%d", err);
            break;
        }
    } while (0);
}

void ChipDnssdResolveNoLongerNeeded(const char * instanceName) {}

CHIP_ERROR ChipDnssdReconfirmRecord(const char * hostname, chip::Inet::IPAddress address, chip::Inet::InterfaceId interface)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

} // namespace Dnssd
} // namespace chip
