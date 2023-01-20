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
#pragma once

#include <platform/internal/CHIPDeviceLayerInternal.h>

namespace chip {
namespace DeviceLayer {

enum EncryptionMode {encrypt_ecb = 0U, encrypt_cbc = 1U};
enum KeySelect {
    kDCP_UseSoftKey = 0U,
    kDCP_OTPMKKeyLow  = 1U, /* Use [127:0] from snvs key as dcp key */
    kDCP_OTPMKKeyHigh = 2U, /* Use [255:128] from snvs key as dcp key */
    kDCP_OCOTPKeyLow  = 3U, /* Use [127:0] from ocotp key as dcp key */
    kDCP_OCOTPKeyHigh = 4U  /* Use [255:128] from ocotp key as dcp key */
};

class DataReaderEncryptedDCP
{
    private:
        const uint8_t *pKey = nullptr;
        const uint8_t *cbcInitialVector = nullptr;
        EncryptionMode encryptMode;
        KeySelect selectedKey;
        bool dcpDriverIsInitialized;
        void SetDCP_OTPKeySelect(void);
    public:
        DataReaderEncryptedDCP():  encryptMode(encrypt_ecb), selectedKey(kDCP_UseSoftKey), dcpDriverIsInitialized(false) {}
        static DataReaderEncryptedDCP & GetDefaultInstance();
        CHIP_ERROR SetAes128Key(const uint8_t *keyAes128);
        CHIP_ERROR SetKeySelected(KeySelect key);
        CHIP_ERROR SetEncryptionMode(EncryptionMode mode);
        CHIP_ERROR SetCbcInitialVector(const uint8_t *iv, uint16_t ivSize);
        CHIP_ERROR ReadData(uint8_t * desBuff, uint8_t * sourceAddr, uint16_t sizeToRead);
        CHIP_ERROR Hash256(const uint8_t *input, size_t inputSize, uint8_t *output, size_t *outputSize);
};

} // namespace DeviceLayer
} // namespace chip
