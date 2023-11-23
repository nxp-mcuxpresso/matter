/*
 *
 *    Copyright (c) 2023 Project CHIP Authors
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
#include <lib/support/CodeUtils.h>
#include <lib/support/FixedBufferAllocator.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/OpenThread/GenericThreadStackManagerImpl_OpenThread.h>
#include <platform/OpenThread/OpenThreadUtils.h>

#include <openthread/mdns_server.h>

using namespace ::chip::DeviceLayer;
using namespace chip::DeviceLayer::Internal;

namespace chip {
namespace Dnssd {

static void ChipDnssdOtBrowseCallback(otError aError, const otDnsBrowseResponse * aResponse, void * aContext);
static void ChipDnssdOtServiceCallback(otError aError, const otDnsServiceResponse * aResponse, void * aContext);

static void DispatchBrowseEmpty(intptr_t context);
static void DispatchBrowse(intptr_t context);
static void DispatchBrowseNoMemory(intptr_t context);

void DispatchAddressResolve(intptr_t context);
void DispatchResolve(intptr_t context);
void DispatchResolveNoMemory(intptr_t context);

static DnsBrowseCallback mDnsBrowseCallback;
static DnsResolveCallback mDnsResolveCallback;

#define LOCAL_DOMAIN_STRING_SIZE 7

struct DnsServiceTxtEntries
{
    uint8_t mBuffer[64];
    Dnssd::TextEntry mTxtEntries[10];
};

struct DnsResult
{
    void * context;
    chip::Dnssd::DnssdService mMdnsService;
    DnsServiceTxtEntries mServiceTxtEntry;
    char mServiceType[chip::Dnssd::kDnssdTypeAndProtocolMaxSize + LOCAL_DOMAIN_STRING_SIZE + 1];
    CHIP_ERROR error;

    DnsResult(void * cbContext, CHIP_ERROR aError)
    {
        context = cbContext;
        error   = aError;
    }
};

CHIP_ERROR ChipDnssdInit(DnssdAsyncReturnCallback initCallback, DnssdAsyncReturnCallback errorCallback, void * context)
{
    CHIP_ERROR error            = CHIP_NO_ERROR;
    otInstance * thrInstancePtr = ThreadStackMgrImpl().OTInstance();

    uint8_t macBuffer[ConfigurationManager::kPrimaryMACAddressLength];
    MutableByteSpan mac(macBuffer);
    char hostname[kHostNameMaxLength + LOCAL_DOMAIN_STRING_SIZE + 1] = "";
    ReturnErrorOnFailure(DeviceLayer::ConfigurationMgr().GetPrimaryMACAddress(mac));
    MakeHostName(hostname, sizeof(hostname), mac);
    snprintf(hostname + strlen(hostname), sizeof(hostname), ".local.");

    error = MapOpenThreadError(otMdnsServerSetHostName(thrInstancePtr, hostname));
    if (error == CHIP_NO_ERROR)
    {
        initCallback(context, error);
    }
    else
    {
        errorCallback(context, error);
    }
    return error;
}

void ChipDnssdShutdown()
{
    otMdnsServerStop(ThreadStackMgrImpl().OTInstance());
}

const char * GetProtocolString(DnssdServiceProtocol protocol)
{
    return protocol == DnssdServiceProtocol::kDnssdProtocolUdp ? "_udp" : "_tcp";
}

CHIP_ERROR ChipDnssdPublishService(const DnssdService * service, DnssdPublishCallback callback, void * context)
{
    ReturnErrorCodeIf(service == nullptr, CHIP_ERROR_INVALID_ARGUMENT);
    otInstance * thrInstancePtr = ThreadStackMgrImpl().OTInstance();
    otDnsTxtEntry aTxtEntry;

    if (strcmp(service->mHostName, "") != 0)
    {
        // ReturnErrorOnFailure(ThreadStackMgr().SetupSrpHost(service->mHostName));
    }

    char serviceType[chip::Dnssd::kDnssdTypeAndProtocolMaxSize + LOCAL_DOMAIN_STRING_SIZE + 1] = "";
    snprintf(serviceType, sizeof(serviceType), "%s.%s.local.", service->mType, GetProtocolString(service->mProtocol));

    char fullInstName[Common::kInstanceNameMaxLength + chip::Dnssd::kDnssdTypeAndProtocolMaxSize + LOCAL_DOMAIN_STRING_SIZE + 1] =
        "";
    snprintf(fullInstName, sizeof(fullInstName), "%s.%s", service->mName, serviceType);

    Span<const char * const> subTypes(service->mSubTypes, service->mSubTypeSize);
    Span<const TextEntry> textEntries(service->mTextEntries, service->mTextEntrySize);

    uint8_t txtBuffer[chip::Dnssd::kDnssdTextMaxSize] = { 0 };
    uint32_t txtBufferOffset                          = 0;
    for (uint32_t i = 0; i < service->mTextEntrySize; i++)
    {
        uint32_t keySize = strlen(service->mTextEntries[i].mKey);
        // add TXT entry len + 1 is for '='
        *(txtBuffer + txtBufferOffset++) = keySize + service->mTextEntries[i].mDataSize + 1;

        // add TXT entry key
        memcpy(txtBuffer + txtBufferOffset, service->mTextEntries[i].mKey, keySize);
        txtBufferOffset += keySize;

        // add TXT entry value if pointer is not null, if pointer is null it means we have bool value
        if (service->mTextEntries[i].mData)
        {
            *(txtBuffer + txtBufferOffset++) = '=';
            memcpy(txtBuffer + txtBufferOffset, service->mTextEntries[i].mData, service->mTextEntries[i].mDataSize);
            txtBufferOffset += service->mTextEntries[i].mDataSize;
        }
    }
    aTxtEntry.mKey         = nullptr;
    aTxtEntry.mValue       = txtBuffer;
    aTxtEntry.mValueLength = txtBufferOffset;
    otMdnsServerAddService(thrInstancePtr, fullInstName, serviceType, service->mPort, &aTxtEntry, 1);

    return CHIP_NO_ERROR;
}

CHIP_ERROR ChipDnssdRemoveServices()
{
    // #if CHIP_DEVICE_CONFIG_ENABLE_THREAD_SRP_CLIENT
    //     ThreadStackMgr().InvalidateAllSrpServices();
    //     return CHIP_NO_ERROR;
    // #else
    return CHIP_ERROR_NOT_IMPLEMENTED;
    // #endif // CHIP_DEVICE_CONFIG_ENABLE_THREAD_SRP_CLIENT
}

CHIP_ERROR ChipDnssdFinalizeServiceUpdate()
{
    // #if CHIP_DEVICE_CONFIG_ENABLE_THREAD_SRP_CLIENT
    //     return ThreadStackMgr().RemoveInvalidSrpServices();
    // #else
    return CHIP_ERROR_NOT_IMPLEMENTED;
    // #endif // CHIP_DEVICE_CONFIG_ENABLE_THREAD_SRP_CLIENT
}

CHIP_ERROR ChipDnssdBrowse(const char * type, DnssdServiceProtocol protocol, Inet::IPAddressType addressType,
                           Inet::InterfaceId interface, DnssdBrowseCallback callback, void * context, intptr_t * browseIdentifier)
{
    *browseIdentifier = reinterpret_cast<intptr_t>(nullptr);

    if (type == nullptr || callback == nullptr)
        return CHIP_ERROR_INVALID_ARGUMENT;

    otInstance * thrInstancePtr = ThreadStackMgrImpl().OTInstance();
    mDnsBrowseCallback          = callback;

    // +1 for null-terminator
    // uint32_t serviceTypeSize = chip::Dnssd::kDnssdTypeAndProtocolMaxSize + LOCAL_DOMAIN_STRING_SIZE + 1;
    // char * serviceType = static_cast<char *>(Platform::MemoryAlloc (serviceTypeSize));

    DnsResult * dnsResult = Platform::New<DnsResult>(context, CHIP_NO_ERROR);
    VerifyOrReturnError(dnsResult != nullptr, CHIP_ERROR_NO_MEMORY);

    snprintf(dnsResult->mServiceType, sizeof(dnsResult->mServiceType), "%s.%s.local.", type, GetProtocolString(protocol));

    *browseIdentifier = reinterpret_cast<intptr_t>(dnsResult);
    otMdnsServerBrowse(thrInstancePtr, dnsResult->mServiceType, ChipDnssdOtBrowseCallback, dnsResult);

    return CHIP_NO_ERROR;
}

CHIP_ERROR ChipDnssdStopBrowse(intptr_t browseIdentifier)
{
#if 0
    auto * dnsResult = reinterpret_cast<DnsResult *>(browseIdentifier);

    otInstance * thrInstancePtr = ThreadStackMgrImpl().OTInstance();
    otError error = OT_ERROR_INVALID_ARGS;

    if (dnsResult)
    {
        error = otMdnsServerStopQuery(thrInstancePtr, dnsResult->mServiceType);
        dnsResult->error = CHIP_NO_ERROR;

        DeviceLayer::PlatformMgr().ScheduleWork(DispatchBrowseEmpty, reinterpret_cast<intptr_t>(dnsResult));
    }

    return MapOpenThreadError(error);
#else
    return CHIP_ERROR_NOT_IMPLEMENTED;
#endif
}

CHIP_ERROR ChipDnssdResolve(DnssdService * browseResult, Inet::InterfaceId interface, DnssdResolveCallback callback, void * context)
{
    if (browseResult == nullptr || callback == nullptr)
        return CHIP_ERROR_INVALID_ARGUMENT;

    otInstance * thrInstancePtr = ThreadStackMgrImpl().OTInstance();
    mDnsResolveCallback         = callback;

    char serviceType[chip::Dnssd::kDnssdTypeAndProtocolMaxSize + LOCAL_DOMAIN_STRING_SIZE + 1] = ""; // +1 for null-terminator
    char fullInstName[Common::kInstanceNameMaxLength + chip::Dnssd::kDnssdTypeAndProtocolMaxSize + LOCAL_DOMAIN_STRING_SIZE + 1] =
        "";
    snprintf(serviceType, sizeof(serviceType), "%s.%s.local.", browseResult->mType, GetProtocolString(browseResult->mProtocol));
    snprintf(fullInstName, sizeof(fullInstName), "%s.%s", browseResult->mName, serviceType);

    otMdnsServerResolveService(thrInstancePtr, fullInstName, ChipDnssdOtServiceCallback, context);

    return CHIP_NO_ERROR;
}

void ChipDnssdResolveNoLongerNeeded(const char * instanceName) {}

CHIP_ERROR ChipDnssdReconfirmRecord(const char * hostname, chip::Inet::IPAddress address, chip::Inet::InterfaceId interface)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR FromOtDnsResponseToMdnsData(otDnsServiceInfo & serviceInfo, const char * serviceType,
                                       chip::Dnssd::DnssdService & mdnsService, DnsServiceTxtEntries & serviceTxtEntries,
                                       otError error)
{
    char protocol[chip::Dnssd::kDnssdProtocolTextMaxSize + 1];

    if (strchr(serviceType, '.') == nullptr)
        return CHIP_ERROR_INVALID_ARGUMENT;

    // Extract from the <type>.<protocol>.<domain-name>. the <type> part.
    size_t substringSize = strchr(serviceType, '.') - serviceType;
    if (substringSize >= ArraySize(mdnsService.mType))
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }
    Platform::CopyString(mdnsService.mType, substringSize + 1, serviceType);

    // Extract from the <type>.<protocol>.<domain-name>. the <protocol> part.
    const char * protocolSubstringStart = serviceType + substringSize + 1;

    if (strchr(protocolSubstringStart, '.') == nullptr)
        return CHIP_ERROR_INVALID_ARGUMENT;

    substringSize = strchr(protocolSubstringStart, '.') - protocolSubstringStart;
    if (substringSize >= ArraySize(protocol))
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }
    Platform::CopyString(protocol, substringSize + 1, protocolSubstringStart);

    if (strncmp(protocol, "_udp", chip::Dnssd::kDnssdProtocolTextMaxSize) == 0)
    {
        mdnsService.mProtocol = chip::Dnssd::DnssdServiceProtocol::kDnssdProtocolUdp;
    }
    else if (strncmp(protocol, "_tcp", chip::Dnssd::kDnssdProtocolTextMaxSize) == 0)
    {
        mdnsService.mProtocol = chip::Dnssd::DnssdServiceProtocol::kDnssdProtocolTcp;
    }
    else
    {
        mdnsService.mProtocol = chip::Dnssd::DnssdServiceProtocol::kDnssdProtocolUnknown;
    }

    // Check if SRV record was included in DNS response.
    if (error != OT_ERROR_NOT_FOUND)
    {
        if (strchr(serviceInfo.mHostNameBuffer, '.') == nullptr)
            return CHIP_ERROR_INVALID_ARGUMENT;

        // Extract from the <hostname>.<domain-name>. the <hostname> part.
        substringSize = strchr(serviceInfo.mHostNameBuffer, '.') - serviceInfo.mHostNameBuffer;
        if (substringSize >= ArraySize(mdnsService.mHostName))
        {
            return CHIP_ERROR_INVALID_ARGUMENT;
        }
        Platform::CopyString(mdnsService.mHostName, substringSize + 1, serviceInfo.mHostNameBuffer);

        mdnsService.mPort = serviceInfo.mPort;
    }

    mdnsService.mInterface = Inet::InterfaceId::Null();

    // Check if AAAA record was included in DNS response.

    if (!otIp6IsAddressUnspecified(&serviceInfo.mHostAddress))
    {
        mdnsService.mAddressType = Inet::IPAddressType::kIPv6;
        mdnsService.mAddress     = MakeOptional(ToIPAddress(serviceInfo.mHostAddress));
    }

    // Check if TXT record was included in DNS response.
    if (serviceInfo.mTxtDataSize != 0)
    {
        otDnsTxtEntryIterator iterator;
        otDnsInitTxtEntryIterator(&iterator, serviceInfo.mTxtData, serviceInfo.mTxtDataSize);

        otDnsTxtEntry txtEntry;
        chip::FixedBufferAllocator alloc(serviceTxtEntries.mBuffer);

        uint8_t entryIndex = 0;
        while ((otDnsGetNextTxtEntry(&iterator, &txtEntry) == OT_ERROR_NONE) && entryIndex < 64)
        {
            if (txtEntry.mKey == nullptr || txtEntry.mValue == nullptr)
                continue;

            serviceTxtEntries.mTxtEntries[entryIndex].mKey      = alloc.Clone(txtEntry.mKey);
            serviceTxtEntries.mTxtEntries[entryIndex].mData     = alloc.Clone(txtEntry.mValue, txtEntry.mValueLength);
            serviceTxtEntries.mTxtEntries[entryIndex].mDataSize = txtEntry.mValueLength;
            entryIndex++;
        }

        ReturnErrorCodeIf(alloc.AnyAllocFailed(), CHIP_ERROR_BUFFER_TOO_SMALL);

        mdnsService.mTextEntries   = serviceTxtEntries.mTxtEntries;
        mdnsService.mTextEntrySize = entryIndex;
    }
    else
    {
        mdnsService.mTextEntrySize = 0;
    }

    return CHIP_NO_ERROR;
}

static void ChipDnssdOtBrowseCallback(otError aError, const otDnsBrowseResponse * aResponse, void * aContext)
{
    CHIP_ERROR error;
    // type buffer size is kDnssdTypeAndProtocolMaxSize + . + kMaxDomainNameSize + . + termination character
    char type[Dnssd::kDnssdTypeAndProtocolMaxSize + LOCAL_DOMAIN_STRING_SIZE + 3];
    // hostname buffer size is kHostNameMaxLength + . + kMaxDomainNameSize + . + termination character
    char hostname[Dnssd::kHostNameMaxLength + LOCAL_DOMAIN_STRING_SIZE + 3];
    // secure space for the raw TXT data in the worst-case scenario relevant for Matter:
    // each entry consists of txt_entry_size (1B) + txt_entry_key + "=" + txt_entry_data
    // uint8_t txtBuffer[kMaxDnsServiceTxtEntriesNumber + kTotalDnsServiceTxtBufferSize];
    uint8_t txtBuffer[128];

    DnsResult * browseContext = reinterpret_cast<DnsResult *>(aContext);
    otDnsServiceInfo serviceInfo;
    uint16_t index = 0;

    /// TODO: check this code, might be remvoed, or if not free browseContext
    if (mDnsBrowseCallback == nullptr)
    {
        ChipLogError(DeviceLayer, "Invalid dns browse callback");
        return;
    }

    VerifyOrExit(aError == OT_ERROR_NONE, error = MapOpenThreadError(aError));

    error = MapOpenThreadError(otDnsBrowseResponseGetServiceName(aResponse, type, sizeof(type)));

    VerifyOrExit(error == CHIP_NO_ERROR, );

    char serviceName[Dnssd::Common::kInstanceNameMaxLength + 1];
    while (otDnsBrowseResponseGetServiceInstance(aResponse, index, serviceName, sizeof(serviceName)) == OT_ERROR_NONE)
    {
        serviceInfo.mHostNameBuffer     = hostname;
        serviceInfo.mHostNameBufferSize = sizeof(hostname);
        serviceInfo.mTxtData            = txtBuffer;
        serviceInfo.mTxtDataSize        = sizeof(txtBuffer);

        otError err = otDnsBrowseResponseGetServiceInfo(aResponse, serviceName, &serviceInfo);
        error       = MapOpenThreadError(err);

        VerifyOrExit(err == OT_ERROR_NOT_FOUND || err == OT_ERROR_NONE, );

        DnsResult * dnsResult = Platform::New<DnsResult>(browseContext->context, CHIP_NO_ERROR);

        VerifyOrExit(dnsResult != nullptr, error = CHIP_ERROR_NO_MEMORY);

        error = FromOtDnsResponseToMdnsData(serviceInfo, type, dnsResult->mMdnsService, dnsResult->mServiceTxtEntry, err);
        if (CHIP_NO_ERROR == error)
        {
            // Invoke callback for every service one by one instead of for the whole
            // list due to large memory size needed to allocate on stack.
            static_assert(ArraySize(dnsResult->mMdnsService.mName) >= ArraySize(serviceName),
                          "The target buffer must be big enough");
            Platform::CopyString(dnsResult->mMdnsService.mName, serviceName);
            DeviceLayer::PlatformMgr().ScheduleWork(DispatchBrowse, reinterpret_cast<intptr_t>(dnsResult));
        }
        else
        {
            Platform::Delete<DnsResult>(dnsResult);
        }
        index++;
    }

exit:
    // Invoke callback to notify about end-of-browse when OT_ERROR_RESPONSE_TIMEOUT is received, otherwise ignore errors
    if (aError == OT_ERROR_RESPONSE_TIMEOUT)
    {
        DeviceLayer::PlatformMgr().ScheduleWork(DispatchBrowseEmpty, reinterpret_cast<intptr_t>(browseContext));
    }
    else if (aError == OT_ERROR_NONE)
    {
        otInstance * thrInstancePtr = ThreadStackMgrImpl().OTInstance();
        otMdnsServerStopQuery(thrInstancePtr, type);
        DeviceLayer::PlatformMgr().ScheduleWork(DispatchBrowseEmpty, reinterpret_cast<intptr_t>(browseContext));
    }
}
static void ChipDnssdOtServiceCallback(otError aError, const otDnsServiceResponse * aResponse, void * aContext)
{
    CHIP_ERROR error;
    otError otErr;
    otDnsServiceInfo serviceInfo;
    DnsResult * dnsResult = Platform::New<DnsResult>(aContext, MapOpenThreadError(aError));
    bool bStopQuery       = false;

    if (aError != OT_ERROR_RESPONSE_TIMEOUT)
    {
        bStopQuery = true;
    }

    VerifyOrExit(dnsResult != nullptr, error = CHIP_ERROR_NO_MEMORY);

    // type buffer size is kDnssdTypeAndProtocolMaxSize + . + kMaxDomainNameSize + . + termination character
    char type[Dnssd::kDnssdTypeAndProtocolMaxSize + LOCAL_DOMAIN_STRING_SIZE + 3];
    // hostname buffer size is kHostNameMaxLength + . + kMaxDomainNameSize + . + termination character
    char hostname[Dnssd::kHostNameMaxLength + LOCAL_DOMAIN_STRING_SIZE + 3];
    // secure space for the raw TXT data in the worst-case scenario relevant for Matter:
    // each entry consists of txt_entry_size (1B) + txt_entry_key + "=" + txt_entry_data
    // uint8_t txtBuffer[kMaxDnsServiceTxtEntriesNumber + kTotalDnsServiceTxtBufferSize];
    uint8_t txtBuffer[128];

    if (mDnsResolveCallback == nullptr)
    {
        ChipLogError(DeviceLayer, "Invalid dns resolve callback");
        return;
    }

    VerifyOrExit(aError == OT_ERROR_NONE, error = MapOpenThreadError(aError));

    error = MapOpenThreadError(otDnsServiceResponseGetServiceName(aResponse, dnsResult->mMdnsService.mName,
                                                                  sizeof(dnsResult->mMdnsService.mName), type, sizeof(type)));

    VerifyOrExit(error == CHIP_NO_ERROR, );

    serviceInfo.mHostNameBuffer     = hostname;
    serviceInfo.mHostNameBufferSize = sizeof(hostname);
    serviceInfo.mTxtData            = txtBuffer;
    serviceInfo.mTxtDataSize        = sizeof(txtBuffer);

    otErr = otDnsServiceResponseGetServiceInfo(aResponse, &serviceInfo);
    error = MapOpenThreadError(otErr);

    VerifyOrExit(error == CHIP_NO_ERROR, );

    error = FromOtDnsResponseToMdnsData(serviceInfo, type, dnsResult->mMdnsService, dnsResult->mServiceTxtEntry, otErr);

exit:
    if (dnsResult == nullptr)
    {
        DeviceLayer::PlatformMgr().ScheduleWork(DispatchResolveNoMemory, reinterpret_cast<intptr_t>(aContext));
        return;
    }

    dnsResult->error = error;

    // If IPv6 address in unspecified (AAAA record not present), send additional DNS query to obtain IPv6 address.
    if (otIp6IsAddressUnspecified(&serviceInfo.mHostAddress))
    {
        DeviceLayer::PlatformMgr().ScheduleWork(DispatchAddressResolve, reinterpret_cast<intptr_t>(dnsResult));
    }
    else
    {
        DeviceLayer::PlatformMgr().ScheduleWork(DispatchResolve, reinterpret_cast<intptr_t>(dnsResult));
    }

    if (bStopQuery)
    {
        char fullInstName[Common::kInstanceNameMaxLength + chip::Dnssd::kDnssdTypeAndProtocolMaxSize + LOCAL_DOMAIN_STRING_SIZE +
                          1] = "";
        snprintf(fullInstName, sizeof(fullInstName), "%s.%s", dnsResult->mMdnsService.mName, type);

        otInstance * thrInstancePtr = ThreadStackMgrImpl().OTInstance();
        otMdnsServerStopQuery(thrInstancePtr, fullInstName);
    }
}

void DispatchBrowseEmpty(intptr_t context)
{
    auto * dnsResult = reinterpret_cast<DnsResult *>(context);
    mDnsBrowseCallback(dnsResult->context, nullptr, 0, true, dnsResult->error);
    Platform::Delete<DnsResult>(dnsResult);
}

void DispatchBrowse(intptr_t context)
{
    auto * dnsResult = reinterpret_cast<DnsResult *>(context);
    mDnsBrowseCallback(dnsResult->context, &dnsResult->mMdnsService, 1, false, dnsResult->error);
    Platform::Delete<DnsResult>(dnsResult);
}

void DispatchBrowseNoMemory(intptr_t context)
{
    mDnsBrowseCallback(reinterpret_cast<void *>(context), nullptr, 0, true, CHIP_ERROR_NO_MEMORY);
}

void DispatchAddressResolve(intptr_t context)
{
    CHIP_ERROR error = CHIP_ERROR_NO_MEMORY; // ResolveAddress(context, OnDnsAddressResolveResult);

    // In case of address resolve failure, fill the error code field and dispatch method to end resolve process.
    if (error != CHIP_NO_ERROR)
    {
        DnsResult * dnsResult = reinterpret_cast<DnsResult *>(context);
        dnsResult->error      = error;

        DeviceLayer::PlatformMgr().ScheduleWork(DispatchResolve, reinterpret_cast<intptr_t>(dnsResult));
    }
}

void DispatchResolve(intptr_t context)
{
    DnsResult * dnsResult         = reinterpret_cast<DnsResult *>(context);
    Dnssd::DnssdService & service = dnsResult->mMdnsService;
    Span<Inet::IPAddress> ipAddrs;

    if (service.mAddress.HasValue())
    {
        ipAddrs = Span<Inet::IPAddress>(&service.mAddress.Value(), 1);
    }

    mDnsResolveCallback(dnsResult->context, &service, ipAddrs, dnsResult->error);
    Platform::Delete<DnsResult>(dnsResult);
}

void DispatchResolveNoMemory(intptr_t context)
{
    Span<Inet::IPAddress> ipAddrs;
    mDnsResolveCallback(reinterpret_cast<void *>(context), nullptr, ipAddrs, CHIP_ERROR_NO_MEMORY);
}

} // namespace Dnssd
} // namespace chip
