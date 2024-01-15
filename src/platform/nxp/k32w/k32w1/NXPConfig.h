/*
 *
 *    Copyright (c) 2020-2022 Project CHIP Authors
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
 *          Utilities for accessing persisted device configuration on
 *          platforms based on the  K32W1 SDK.
 */

#pragma once

#include <functional>

#include <platform/internal/CHIPDeviceLayerInternal.h>

#include "FreeRTOS.h"
#include "NVM_Interface.h"

#include "ram_storage.h"

namespace chip {
namespace DeviceLayer {
namespace Internal {

constexpr inline uint16_t NXPConfigKey(uint8_t chipId, uint8_t nvmId)
{
    return static_cast<uint16_t>(chipId) << 8 | nvmId;
}

/**
 * This implementation uses the  NVM component as the underlying storage layer.
 *
 * NOTE: This class is designed to be mixed-in to the concrete subclass of the
 * GenericConfigurationManagerImpl<> template.  When used this way, the class
 * naturally provides implementations for the delegated members referenced by
 * the template class (e.g. the ReadConfigValue() method).
 */
class NXPConfig
{
public:
    // Category ids used by the CHIP Device Layer
    static constexpr uint8_t kFileId_ChipFactory = 0x01; /**< Category containing persistent config values set at
                                                          * manufacturing time. Retained during factory reset. */
    static constexpr uint8_t kFileId_ChipConfig = 0x02;  /**< Catyegory containing dynamic config values set at runtime.
                                                          *   Cleared during factory reset. */
    static constexpr uint8_t kFileId_ChipCounter = 0x03; /**< Category containing dynamic counter values set at runtime.
                                                          * Retained during factory reset. */
    static constexpr uint8_t kFileId_KVSKey = 0x04;      /**< Category containing KVS set at runtime.
                                                          *  Cleared during factory reset. */
    static constexpr uint8_t kFileId_KVSValue = 0x05;    /**< Category containing KVS values set at runtime.
                                                          *   Cleared during factory reset. */

    using Key = uint16_t;

    // Key definitions for well-known configuration values.
    // Factory config keys
    static constexpr Key kConfigKey_SerialNum             = NXPConfigKey(kFileId_ChipFactory, 0x00);
    static constexpr Key kConfigKey_MfrDeviceId           = NXPConfigKey(kFileId_ChipFactory, 0x01);
    static constexpr Key kConfigKey_MfrDeviceCert         = NXPConfigKey(kFileId_ChipFactory, 0x02);
    static constexpr Key kConfigKey_MfrDevicePrivateKey   = NXPConfigKey(kFileId_ChipFactory, 0x03);
    static constexpr Key kConfigKey_ManufacturingDate     = NXPConfigKey(kFileId_ChipFactory, 0x04);
    static constexpr Key kConfigKey_SetupPinCode          = NXPConfigKey(kFileId_ChipFactory, 0x05);
    static constexpr Key kConfigKey_MfrDeviceICACerts     = NXPConfigKey(kFileId_ChipFactory, 0x06);
    static constexpr Key kConfigKey_HardwareVersion       = NXPConfigKey(kFileId_ChipFactory, 0x07);
    static constexpr Key kConfigKey_SetupDiscriminator    = NXPConfigKey(kFileId_ChipFactory, 0x08);
    static constexpr Key kConfigKey_Spake2pIterationCount = NXPConfigKey(kFileId_ChipFactory, 0x09);
    static constexpr Key kConfigKey_Spake2pSalt           = NXPConfigKey(kFileId_ChipFactory, 0x0A);
    static constexpr Key kConfigKey_Spake2pVerifier       = NXPConfigKey(kFileId_ChipFactory, 0x0B);

    // CHIP Config Keys
    static constexpr Key kConfigKey_ServiceConfig      = NXPConfigKey(kFileId_ChipConfig, 0x01);
    static constexpr Key kConfigKey_PairedAccountId    = NXPConfigKey(kFileId_ChipConfig, 0x02);
    static constexpr Key kConfigKey_ServiceId          = NXPConfigKey(kFileId_ChipConfig, 0x03);
    static constexpr Key kConfigKey_LastUsedEpochKeyId = NXPConfigKey(kFileId_ChipConfig, 0x05);
    static constexpr Key kConfigKey_FailSafeArmed      = NXPConfigKey(kFileId_ChipConfig, 0x06);
    static constexpr Key kConfigKey_RegulatoryLocation = NXPConfigKey(kFileId_ChipConfig, 0x07);
    static constexpr Key kConfigKey_CountryCode        = NXPConfigKey(kFileId_ChipConfig, 0x08);
    static constexpr Key kConfigKey_UniqueId           = NXPConfigKey(kFileId_ChipConfig, 0x0A);
    static constexpr Key kConfigKey_SoftwareVersion    = NXPConfigKey(kFileId_ChipConfig, 0x0B);

    // CHIP Counter Keys
    static constexpr Key kCounterKey_RebootCount           = NXPConfigKey(kFileId_ChipCounter, 0x00);
    static constexpr Key kCounterKey_UpTime                = NXPConfigKey(kFileId_ChipCounter, 0x01);
    static constexpr Key kCounterKey_TotalOperationalHours = NXPConfigKey(kFileId_ChipCounter, 0x02);
    static constexpr Key kCounterKey_BootReason            = NXPConfigKey(kFileId_ChipCounter, 0x03);

    // Set key id limits for each group.
    static constexpr Key kMinConfigKey_ChipFactory = NXPConfigKey(kFileId_ChipFactory, 0x00);
    static constexpr Key kMaxConfigKey_ChipFactory = NXPConfigKey(kFileId_ChipFactory, 0xFF);
    static constexpr Key kMinConfigKey_ChipConfig  = NXPConfigKey(kFileId_ChipConfig, 0x00);
    static constexpr Key kMaxConfigKey_ChipConfig  = NXPConfigKey(kFileId_ChipConfig, 0xFF);
    static constexpr Key kMinConfigKey_ChipCounter = NXPConfigKey(kFileId_ChipCounter, 0x00);
    static constexpr Key kMaxConfigKey_ChipCounter = NXPConfigKey(kFileId_ChipCounter, 0xFF); // Allows 32 Counters to be created.
    static constexpr Key kMinConfigKey_KVSKey      = NXPConfigKey(kFileId_KVSKey, 0x00);
    static constexpr Key kMaxConfigKey_KVSKey      = NXPConfigKey(kFileId_KVSKey, 0xFF);
    static constexpr Key kMinConfigKey_KVSValue    = NXPConfigKey(kFileId_KVSValue, 0x00);
    static constexpr Key kMaxConfigKey_KVSValue    = NXPConfigKey(kFileId_KVSValue, 0xFF);

    static CHIP_ERROR Init(void);

    // Configuration methods used by the GenericConfigurationManagerImpl<> template.
    static CHIP_ERROR ReadConfigValue(Key key, bool & val);
    static CHIP_ERROR ReadConfigValue(Key key, uint32_t & val);
    static CHIP_ERROR ReadConfigValue(Key key, uint64_t & val);
    static CHIP_ERROR ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen);
    static CHIP_ERROR ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen);
    static CHIP_ERROR ReadConfigValueCounter(uint8_t counterIdx, uint32_t & val);
    static CHIP_ERROR WriteConfigValue(Key key, bool val);
    static CHIP_ERROR WriteConfigValueSync(Key key, bool val);
    static CHIP_ERROR WriteConfigValue(Key key, uint32_t val);
    static CHIP_ERROR WriteConfigValue(Key key, uint64_t val);
    static CHIP_ERROR WriteConfigValueStr(Key key, const char * str);
    static CHIP_ERROR WriteConfigValueStr(Key key, const char * str, size_t strLen);
    static CHIP_ERROR WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen);
    static CHIP_ERROR WriteConfigValueCounter(uint8_t counterIdx, uint32_t val);
    static CHIP_ERROR ClearConfigValue(Key key);
    static bool ConfigValueExists(Key key);
    static CHIP_ERROR FactoryResetConfig(void);
    static bool ValidConfigKey(Key key);

    static void RunConfigUnitTest(void);

private:
    static CHIP_ERROR MapRamStorageStatus(rsError rsStatus);
    static void FactoryResetConfigInternal(Key firstKey, Key lastKey);
};

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip
