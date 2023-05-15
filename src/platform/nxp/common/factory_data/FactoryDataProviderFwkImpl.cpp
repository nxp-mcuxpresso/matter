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

#include "FactoryDataProviderFwkImpl.h"
#include "fwk_factory_data_provider.h"

namespace chip {
namespace DeviceLayer {

FactoryDataProviderImpl FactoryDataProviderImpl::sInstance;

CHIP_ERROR FactoryDataProviderImpl::SearchForId(uint8_t searchedType, uint8_t *pBuf, size_t bufLength, uint16_t &length)
{
    return SearchForId(searchedType, pBuf, bufLength, length, NULL);
}

CHIP_ERROR FactoryDataProviderImpl::SearchForId(uint8_t searchedType, uint8_t *pBuf, size_t bufLength, uint16_t &length, uint32_t *contentAddr)
{
    CHIP_ERROR err = CHIP_ERROR_NOT_FOUND;
    uint32_t readLen = 0;

    uint8_t *ramBufferAddr = FDP_SearchForId(searchedType, pBuf, bufLength, &readLen);

    if (ramBufferAddr != NULL)
    {
        if (contentAddr != NULL)
            *contentAddr = (uint32_t) ramBufferAddr;
        err = CHIP_NO_ERROR;
    }
    length = readLen;

    return err;
}

CHIP_ERROR FactoryDataProviderImpl::Init(void)
{
    /*
    * Currently the fwk_factory_data_provider module supports only ecb mode.
    * Therefore return an error if encrypt mode is not ecb
    */
    if (pAesKey == NULL || encryptMode != encrypt_ecb)
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    if (FDP_Init(pAesKey) < 0)
    {
        return CHIP_ERROR_INTERNAL;
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProviderImpl::SetAes128Key(const uint8_t *keyAes128)
{
    CHIP_ERROR error = CHIP_ERROR_INVALID_ARGUMENT;
    if (keyAes128 != nullptr)
    {
        pAesKey = keyAes128;
        error = CHIP_NO_ERROR;
    }
    return error;
}

CHIP_ERROR FactoryDataProviderImpl::SetEncryptionMode(EncryptionMode mode)
{
    CHIP_ERROR error = CHIP_ERROR_INVALID_ARGUMENT;

    /*
    * Currently the fwk_factory_data_provider module supports only ecb mode.
    * Therefore return an error if encrypt mode is not ecb
    */
    if (mode == encrypt_ecb)
    {
        encryptMode = mode;
        error = CHIP_NO_ERROR;
    }
    return error;
}

} // namespace DeviceLayer
} // namespace chip
