/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
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

#include "DataReaderEncryptedDCP.h"

#include "fsl_dcp.h"

#define CBC_INITIAL_VECTOR_SIZE 16

namespace chip {
namespace DeviceLayer {

DataReaderEncryptedDCP & DataReaderEncryptedDCP::GetDefaultInstance()
{
    static DataReaderEncryptedDCP sInstance;
    return sInstance;
}

CHIP_ERROR DataReaderEncryptedDCP::SetAes128Key(const uint8_t *keyAes128)
{
    CHIP_ERROR error = CHIP_ERROR_INVALID_ARGUMENT;
    if (keyAes128 != nullptr)
    {
        pKey = keyAes128;
        error = CHIP_NO_ERROR;
    }
    return error;
}

CHIP_ERROR DataReaderEncryptedDCP::SetKeySelected(KeySelect key)
{
    CHIP_ERROR error = CHIP_ERROR_INVALID_ARGUMENT;
    if (key <= kDCP_OCOTPKeyHigh)
    {
        selectedKey = key;
        error = CHIP_NO_ERROR;
    }
    return error;
}

void DataReaderEncryptedDCP::SetDCP_OTPKeySelect(void)
{
    switch (selectedKey)
    {
    case kDCP_OTPMKKeyLow:
        IOMUXC_GPR->GPR3 &= ~(1 << IOMUXC_GPR_GPR3_DCP_KEY_SEL_SHIFT);
        IOMUXC_GPR->GPR10 &= ~(1 << IOMUXC_GPR_GPR10_DCPKEY_OCOTP_OR_KEYMUX_SHIFT);
        break;
    case kDCP_OTPMKKeyHigh:
        IOMUXC_GPR->GPR3 |= (1 << IOMUXC_GPR_GPR3_DCP_KEY_SEL_SHIFT);
        IOMUXC_GPR->GPR10 &= ~(1 << IOMUXC_GPR_GPR10_DCPKEY_OCOTP_OR_KEYMUX_SHIFT);
        break;
    case kDCP_OCOTPKeyLow:
        IOMUXC_GPR->GPR3 &= ~(1 << IOMUXC_GPR_GPR3_DCP_KEY_SEL_SHIFT);
        IOMUXC_GPR->GPR10 |= (1 << IOMUXC_GPR_GPR10_DCPKEY_OCOTP_OR_KEYMUX_SHIFT);
        break;
    case kDCP_OCOTPKeyHigh:
        IOMUXC_GPR->GPR3 |= (1 << IOMUXC_GPR_GPR3_DCP_KEY_SEL_SHIFT);
        IOMUXC_GPR->GPR10 |= (1 << IOMUXC_GPR_GPR10_DCPKEY_OCOTP_OR_KEYMUX_SHIFT);
        break;
    default:
        break;
    }
}

CHIP_ERROR DataReaderEncryptedDCP::SetCbcInitialVector(const uint8_t *iv, uint16_t ivSize)
{
    CHIP_ERROR error = CHIP_ERROR_INVALID_ARGUMENT;
    if (ivSize == CBC_INITIAL_VECTOR_SIZE)
    {
        cbcInitialVector = iv;
        error = CHIP_NO_ERROR;
    }
    return error;
}

CHIP_ERROR DataReaderEncryptedDCP::SetEncryptionMode(EncryptionMode mode)
{
    CHIP_ERROR error = CHIP_ERROR_INVALID_ARGUMENT;
    if (mode <= encrypt_cbc)
    {
        encryptMode = mode;
        error = CHIP_NO_ERROR;
    }
    return error;
}

CHIP_ERROR DataReaderEncryptedDCP::ReadData(uint8_t * desBuff, uint8_t * sourceAddr, uint16_t sizeToRead)
{
    status_t status;
    dcp_handle_t m_handle;
    dcp_config_t dcpConfig;

    /* Check that the length is aligned on 16 bytes */
    if ((sizeToRead % 16) != 0)
        return CHIP_ERROR_INVALID_ARGUMENT;

    /* Check that the soft key has been correclty provisioned */
    if (selectedKey == kDCP_UseSoftKey && pKey == nullptr)
        return CHIP_ERROR_INVALID_ARGUMENT;

    /* Check if the initial vector has been provisioned if CBC mode is chosen */
    if (encryptMode == encrypt_cbc && cbcInitialVector == nullptr)
        return CHIP_ERROR_INVALID_ARGUMENT;

    if (!dcpDriverIsInitialized)
    {
        /* Initialize DCP */
        DCP_GetDefaultConfig(&dcpConfig);
        SetDCP_OTPKeySelect();
        /* Reset and initialize DCP */
        DCP_Init(DCP, &dcpConfig);
        dcpDriverIsInitialized = true;
    }

    m_handle.channel    = kDCP_Channel0;
    m_handle.swapConfig = kDCP_NoSwap;

    if (selectedKey == kDCP_UseSoftKey)
        m_handle.keySlot = kDCP_KeySlot0;
    else
        m_handle.keySlot = kDCP_OtpKey;

    status = DCP_AES_SetKey(DCP, &m_handle, pKey, 16);
    if (status != kStatus_Success)
        return CHIP_ERROR_INTERNAL;

    if (encryptMode == encrypt_ecb)
        DCP_AES_DecryptEcb(DCP, &m_handle, sourceAddr, desBuff, sizeToRead);
    else
        DCP_AES_DecryptCbc(DCP, &m_handle, sourceAddr, desBuff, sizeToRead, cbcInitialVector);

    return CHIP_NO_ERROR;
}

CHIP_ERROR DataReaderEncryptedDCP::Hash256(const uint8_t *input, size_t inputSize, uint8_t *output, size_t *outputSize)
{
    status_t status;
    dcp_handle_t m_handle;

    m_handle.channel    = kDCP_Channel0;
    m_handle.swapConfig = kDCP_NoSwap;
    m_handle.keySlot = kDCP_KeySlot0;

    status = DCP_HASH(DCP, &m_handle, kDCP_Sha256, input, inputSize, output, outputSize);

    if (status != kStatus_Success)
        return CHIP_ERROR_INTERNAL;

    return CHIP_NO_ERROR;
}

} // namespace DeviceLayer
} // namespace chip
