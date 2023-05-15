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

/**
 *    @file
 *          Utilities for accessing persisted device configuration on
 *          platforms based on the  NXP SDK.
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include "NXPConfig.h"

#include <lib/core/CHIPEncoding.h>
#include <platform/internal/testing/ConfigUnitTest.h>
#include "board.h"
#include "FunctionLib.h"
#include "FreeRTOS.h"

#include "fwk_key_storage.h"
#include "fwk_file_cache.h"
#include "fwk_lfs_mflash.h"

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include "ot_platform_common.h"
#endif

#if defined(DEBUG_NVM) && (DEBUG_NVM==2)
#include "fsl_debug_console.h"
#define DBG_PRINTF                      PRINTF
#define INFO_PRINTF                     PRINTF

#elif defined(DEBUG_NVM) && (DEBUG_NVM==1)
#include "fsl_debug_console.h"
#define DBG_PRINTF                      PRINTF
#define INFO_PRINTF(...)

#else
#define DBG_PRINTF(...)
#define INFO_PRINTF(...)
#endif

#define MAX_CONF_SIZE 2048

static ks_config_t ks_config = {
    .size    = MAX_CONF_SIZE,
    .KS_name = "KS_config",
};
static void *ks_handle_p;

static bool isInitialized = false;

namespace chip {
namespace DeviceLayer {
namespace Internal {

CHIP_ERROR NXPConfig::Init()
{
    if (!isInitialized)
    {
        ks_handle_p = KS_Init(&ks_config);
        isInitialized = true;
    }

    DBG_PRINTF("Init");
    return CHIP_NO_ERROR;
}

CHIP_ERROR NXPConfig::ReadConfigValue(Key key, bool & val)
{
    CHIP_ERROR err;
    ks_error_t status;
    int req_len;
    int outLen;

    VerifyOrExit(ValidConfigKey(key), err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    req_len = sizeof(bool);
    status = KS_GetKeyInt(ks_handle_p, (int)key, (void *)val, req_len, &outLen);
    SuccessOrExit(err = MapKeyStorageStatus(status));
    ChipLogProgress(DeviceLayer, "ReadConfigValue bool = %u", val);

exit:
    return err;
}

CHIP_ERROR NXPConfig::ReadConfigValue(Key key, uint32_t & val)
{
    CHIP_ERROR err;
    ks_error_t status;
    int req_len;
    int outLen;

    VerifyOrExit(ValidConfigKey(key), err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    req_len = sizeof(uint32_t);
    status = KS_GetKeyInt(ks_handle_p, (int)key, (void *)val, req_len, &outLen);
    SuccessOrExit(err = MapKeyStorageStatus(status));
    ChipLogProgress(DeviceLayer, "ReadConfigValue uint32_t = %lu", val);

exit:
    return err;
}

CHIP_ERROR NXPConfig::ReadConfigValue(Key key, uint64_t & val)
{
    CHIP_ERROR err;
    ks_error_t status;
    int req_len;
    int ret;
    int outLen;

    VerifyOrExit(ValidConfigKey(key), err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    req_len = sizeof(uint64_t);
    status = KS_GetKeyInt(ks_handle_p, (int)key, (void *)val, req_len, &outLen);
    SuccessOrExit(err = MapKeyStorageStatus(status));
    ChipLogProgress(DeviceLayer, "ReadConfigValue uint64_t = " ChipLogFormatX64, ChipLogValueX64(val));

exit:
    return err;
}

CHIP_ERROR NXPConfig::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    CHIP_ERROR err;
    ks_error_t status;

    VerifyOrExit(ValidConfigKey(key), err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    status = KS_GetKeyInt(ks_handle_p, (int)key, (void *)buf, (int)bufSize, (int *)outLen);
    SuccessOrExit(err = MapKeyStorageStatus(status));
    ChipLogProgress(DeviceLayer, "ReadConfigValueStr lenRead = %u", outLen);

exit:
    return err;
}

CHIP_ERROR NXPConfig::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    return ReadConfigValueStr(key, (char*) buf, bufSize, outLen);
}

CHIP_ERROR NXPConfig::ReadConfigValueBin(const char* keyString, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    CHIP_ERROR err;
    ks_error_t status;
    int keyLen;

    VerifyOrExit(keyString != NULL, err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    keyLen = (int)strlen(keyString);
    status = KS_GetKeyString(ks_handle_p, (char *)keyString, keyLen, (void *)buf, (int)bufSize, (int *)outLen);
    SuccessOrExit(err = MapKeyStorageStatus(status));
    ChipLogProgress(DeviceLayer, "ReadConfigValueStr lenRead = %u", outLen);

exit:
    return err;
}

CHIP_ERROR NXPConfig::ReadConfigValueCounter(uint8_t counterIdx, uint32_t & val)
{
    Key key = kMinConfigKey_ChipCounter + counterIdx;
    return ReadConfigValue(key, val);
}

CHIP_ERROR NXPConfig::WriteConfigValue(Key key, bool val)
{
    CHIP_ERROR err;
    ks_error_t status;
    int valSize;

    VerifyOrExit(ValidConfigKey(key), err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    valSize = sizeof(bool);
    status = KS_SetKeyInt(ks_handle_p, (int)key, (void *)val, valSize);
    SuccessOrExit(err = MapKeyStorageStatus(status));

    DBG_PRINTF("WriteConfigValue: MT write \r\n");

    ChipLogProgress(DeviceLayer, "WriteConfigValue done");

exit:
    return err;
}

CHIP_ERROR NXPConfig::WriteConfigValue(Key key, uint32_t val)
{
    CHIP_ERROR err;
    ks_error_t status;
    int valSize;

    VerifyOrExit(ValidConfigKey(key), err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    valSize = sizeof(uint32_t);
    status = KS_SetKeyInt(ks_handle_p, (int)key, (void *)val, valSize);
    SuccessOrExit(err = MapKeyStorageStatus(status));

    DBG_PRINTF("WriteConfigValue: MT write \r\n");

    ChipLogProgress(DeviceLayer, "WriteConfigValue done");

exit:
    return err;
}

CHIP_ERROR NXPConfig::WriteConfigValue(Key key, uint64_t val)
{
    CHIP_ERROR err;
    ks_error_t status;
    int valSize;

    VerifyOrExit(ValidConfigKey(key), err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    valSize = sizeof(uint64_t);
    status = KS_SetKeyInt(ks_handle_p, (int)key, (void *)val, valSize);
    SuccessOrExit(err = MapKeyStorageStatus(status));

    DBG_PRINTF("WriteConfigValue64: MT write \r\n");

    ChipLogProgress(DeviceLayer, "WriteConfigValue done");

exit:
    return err;
}

CHIP_ERROR NXPConfig::WriteConfigValueStr(Key key, const char * str)
{
    return WriteConfigValueStr(key, str, (str != NULL) ? strlen(str) : 0);
}

CHIP_ERROR NXPConfig::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    CHIP_ERROR err;
    ks_error_t status;

    VerifyOrExit(ValidConfigKey(key), err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    status = KS_SetKeyInt(ks_handle_p, (int)key, (void *)str, strLen);
    SuccessOrExit(err = MapKeyStorageStatus(status));

    DBG_PRINTF("WriteConfigValueStr: MT write \r\n");

    ChipLogProgress(DeviceLayer, "WriteConfigValue done");

exit:
    return err;
}

CHIP_ERROR NXPConfig::WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen)
{
    return WriteConfigValueStr(key, (char *) data, dataLen);
}

CHIP_ERROR NXPConfig::WriteConfigValueBin(const char* keyString, const uint8_t * data, size_t dataLen)
{
    CHIP_ERROR err;
    ks_error_t status;
    int keyLen;

    VerifyOrExit(keyString != NULL, err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    keyLen = (int)strlen(keyString);
    status = KS_SetKeyString(ks_handle_p, (char *)keyString, keyLen, (void *)data, (int)dataLen);
    SuccessOrExit(err = MapKeyStorageStatus(status));

    DBG_PRINTF("WriteConfigValueBin: MT write \r\n");

    ChipLogProgress(DeviceLayer, "WriteConfigValue done");

exit:
    return err;
}

CHIP_ERROR NXPConfig::WriteConfigValueCounter(uint8_t counterIdx, uint32_t val)
{
    Key key = kMinConfigKey_ChipCounter + counterIdx;
    return WriteConfigValue(key, val);
}

CHIP_ERROR NXPConfig::ClearConfigValue(Key key)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    ks_error_t status;

    VerifyOrExit(ValidConfigKey(key), err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    status = KS_DeleteKeyInt(ks_handle_p, (int)key);
    SuccessOrExit(err = MapKeyStorageStatus(status));

    DBG_PRINTF("ClearConfigValue: MT write \r\n");

exit:
    return err;
}

CHIP_ERROR NXPConfig::ClearConfigValue(const char * keyString)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    ks_error_t status;
    int keyLen;

    VerifyOrExit(keyString != NULL, err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND); // Verify key id.
    keyLen = strlen(keyString);
    status = KS_DeleteKeyString(ks_handle_p, (char *)keyString, keyLen);
    SuccessOrExit(err = MapKeyStorageStatus(status));

    DBG_PRINTF("WriteConfigValueBin: MT write \r\n");

exit:
    return err;
}

bool NXPConfig::ConfigValueExists(Key key)
{
    ks_error_t status;
    bool found = false;
    void *readValue_p;
    int outLen;
    int bufSize = 100;

    if (ValidConfigKey(key))
    {
        /* Get the first occurence */
        status = KS_GetKeyInt(ks_handle_p, (int)key, readValue_p, bufSize, &outLen);
        found = (status == KS_ERROR_NONE && outLen != 0);
    }
    return found;
}

CHIP_ERROR NXPConfig::FactoryResetConfig(void)
{
    /*for (Key key = kMinConfigKey_ChipConfig; key <= kMaxConfigKey_ChipConfig; key++)
    {
        ClearConfigValue(key);
    }*/

    KS_Reset(ks_handle_p);

    DBG_PRINTF("FactoryResetConfig done\r\n");

    return CHIP_NO_ERROR;
}

bool NXPConfig::ValidConfigKey(Key key)
{
    // Returns true if the key is in the valid CHIP Config PDM key range.

    if ((key >= kMinConfigKey_ChipFactory) && (key <= kMaxConfigKey_KVS))
    {
        return true;
    }

    return false;
}

CHIP_ERROR NXPConfig::MapKeyStorageStatus(ks_error_t ksStatus)
{
    CHIP_ERROR err;

    switch (ksStatus)
    {
        case KS_ERROR_NONE:
            err = CHIP_NO_ERROR;
            break;
        case KS_ERROR_BUF_TOO_SMALL:
            err = CHIP_ERROR_BUFFER_TOO_SMALL;
            break;
        default: /* KS_ERROR_KEY_NOT_FOUND */
            err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
            break;
    }

    return err;
}

void NXPConfig::RunConfigUnitTest(void)
{
}

void NXPConfig::RunSystemIdleTask(void)
{
    if (isInitialized)
    {
        FC_Process();
        INFO_PRINTF("str mt write  \r\n");
    }
}

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip
