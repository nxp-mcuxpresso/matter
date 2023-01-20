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

/**
 *    @file
 *          Provides the implementation of the Device Layer ConfigurationManager object
 *          for NXP platforms using the NXP SDK.
 */

/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <platform/ConfigurationManager.h>
#include "NXPConfig.h"
#include <platform/internal/GenericConfigurationManagerImpl.ipp>

#include "fsl_device_registers.h"

#if CONFIG_CHIP_PLAT_LOAD_REAL_FACTORY_DATA
#include "FactoryDataProvider.h"
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_WPA
extern "C" {
#include "wifi.h"
}
#endif

namespace chip {
namespace DeviceLayer {

using namespace ::chip::DeviceLayer::Internal;

ConfigurationManagerImpl & ConfigurationManagerImpl::GetDefaultInstance()
{
    static ConfigurationManagerImpl sInstance;
    return sInstance;
}

CHIP_ERROR ConfigurationManagerImpl::Init()
{
    CHIP_ERROR err;
    bool failSafeArmed;

    // Initialize the generic implementation base class.
    err = Internal::GenericConfigurationManagerImpl<NXPConfig>::Init();
    SuccessOrExit(err);

    // TODO: Initialize the global GroupKeyStore object here

    // If the fail-safe was armed when the device last shutdown, initiate a factory reset.
    if (GetFailSafeArmed(failSafeArmed) == CHIP_NO_ERROR && failSafeArmed)
    {
        ChipLogProgress(DeviceLayer, "Detected fail-safe armed on reboot; initiating factory reset");
        InitiateFactoryReset();
    }
    err = CHIP_NO_ERROR;

exit:
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::GetPrimaryWiFiMACAddress(uint8_t * buf)
{
#if CHIP_DEVICE_CONFIG_ENABLE_WPA
    wifi_mac_addr_t mac_addr;
    int status;

    status = wifi_get_device_mac_addr(&mac_addr);
    if (status != WM_SUCCESS)
    {
        return CHIP_ERROR_INTERNAL;
    }

    (void)memcpy(&buf[0], mac_addr.mac, MLAN_MAC_ADDR_LENGTH);

    return CHIP_NO_ERROR;
#else
    (void)memset(&buf[0], 0, 6); // this is to avoid compilation error in GenericConfigurationManagerImpl.cpp
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
#endif
}

CHIP_ERROR ConfigurationManagerImpl::GetUniqueId(char * buf, size_t bufSize)
{
    CHIP_ERROR err;
    size_t uniqueIdLen = 0; // without counting null-terminator
    err                = ReadConfigValueStr(NXPConfig::kConfigKey_UniqueId, buf, bufSize, uniqueIdLen);

    ReturnErrorOnFailure(err);

    ReturnErrorCodeIf(uniqueIdLen >= bufSize, CHIP_ERROR_BUFFER_TOO_SMALL);
    ReturnErrorCodeIf(buf[uniqueIdLen] != 0, CHIP_ERROR_INVALID_STRING_LENGTH);

    return err;
}

CHIP_ERROR ConfigurationManagerImpl::StoreUniqueId(const char * uniqueId, size_t uniqueIdLen)
{
    return WriteConfigValueStr(NXPConfig::kConfigKey_UniqueId, uniqueId, uniqueIdLen);
}

CHIP_ERROR ConfigurationManagerImpl::GenerateUniqueId(char * buf, size_t bufSize)
{
    uint64_t randomUniqueId = Crypto::GetRandU64();
    return Encoding::BytesToUppercaseHexString(reinterpret_cast<uint8_t *>(&randomUniqueId), sizeof(uint64_t), buf, bufSize);
}

bool ConfigurationManagerImpl::CanFactoryReset()
{
    // TODO: query the application to determine if factory reset is allowed.
    return true;
}

void ConfigurationManagerImpl::InitiateFactoryReset()
{
    PlatformMgr().ScheduleWork(DoFactoryReset);
}

CHIP_ERROR ConfigurationManagerImpl::ReadPersistedStorageValue(::chip::Platform::PersistedStorage::Key persistedStorageKey,
                                                               uint32_t & value)
{
    CHIP_ERROR err;

    err = NXPConfig::ReadConfigValueCounter(persistedStorageKey, value);
    if (err == CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }
    SuccessOrExit(err);

exit:
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::WritePersistedStorageValue(::chip::Platform::PersistedStorage::Key persistedStorageKey,
                                                                uint32_t value)
{
    // This method reads Chip Persisted Counter type nvm3 objects.
    // (where persistedStorageKey represents an index to the counter).
    CHIP_ERROR err;

    err = NXPConfig::WriteConfigValueCounter(persistedStorageKey, value);
    if (err == CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }
    SuccessOrExit(err);

exit:
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, bool & val)
{
    return NXPConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, uint32_t & val)
{
    return NXPConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, uint64_t & val)
{
    return NXPConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    return NXPConfig::ReadConfigValueStr(key, buf, bufSize, outLen);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    return NXPConfig::ReadConfigValueBin(key, buf, bufSize, outLen);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, bool val)
{
    return NXPConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, uint32_t val)
{
    return NXPConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, uint64_t val)
{
    return NXPConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueStr(Key key, const char * str)
{
    return NXPConfig::WriteConfigValueStr(key, str);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    return NXPConfig::WriteConfigValueStr(key, str, strLen);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen)
{
    return NXPConfig::WriteConfigValueBin(key, data, dataLen);
}

void ConfigurationManagerImpl::RunConfigUnitTest(void)
{
    NXPConfig::RunConfigUnitTest();
}

void ConfigurationManagerImpl::DoFactoryReset(intptr_t arg)
{
    CHIP_ERROR err;

    ChipLogProgress(DeviceLayer, "Performing factory reset");

    err = NXPConfig::FactoryResetConfig();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "FactoryResetConfig() failed: %s", ErrorStr(err));
    }

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD

    ThreadStackMgr().ErasePersistentInfo();

#endif // CHIP_DEVICE_CONFIG_ENABLE_THREAD

    /* Schedule a reset in the next idle call */
    PlatformMgrImpl().ScheduleResetInIdle();
}

ConfigurationManager & ConfigurationMgrImpl()
{
    return ConfigurationManagerImpl::GetDefaultInstance();
}

} // namespace DeviceLayer
} // namespace chip
