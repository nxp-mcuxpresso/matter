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

#include "MW320FactoryDataProvider.h"
#include "CHIPDevicePlatformConfig.h"
#include <crypto/CHIPCryptoPAL.h>
#include <lib/core/CHIPError.h>
#include <lib/core/TLV.h>
#include <lib/support/Base64.h>
#include <lib/support/Span.h>
#include <credentials/CHIPCert.h>
#include <credentials/CertificationDeclaration.h>
#include <platform/ConfigurationManager.h>

extern "C" {
#include "hkdf-sha.h"
}

/* Grab symbol for the base address from the linker file. */
extern uint8_t * __FACTORY_DATA_START;
extern uint32_t __FACTORY_DATA_SIZE;

namespace chip {
namespace {

CHIP_ERROR LoadKeypairFromRaw(ByteSpan privateKey, ByteSpan publicKey, Crypto::P256Keypair & keypair)
{
    Crypto::P256SerializedKeypair serialized_keypair;
    ReturnErrorOnFailure(serialized_keypair.SetLength(privateKey.size() + publicKey.size()));
    memcpy(serialized_keypair.Bytes(), publicKey.data(), publicKey.size());
    memcpy(serialized_keypair.Bytes() + publicKey.size(), privateKey.data(), privateKey.size());
    return keypair.Deserialize(serialized_keypair);
}
} // namespace

namespace DeviceLayer {
static constexpr size_t kSpake2pSerializedVerifier_MaxBase64Len =
    BASE64_ENCODED_LEN(chip::Crypto::kSpake2p_VerifierSerialized_Length) + 1;
static constexpr size_t kSpake2pSalt_MaxBase64Len = BASE64_ENCODED_LEN(chip::Crypto::kSpake2p_Max_PBKDF_Salt_Length) + 1;
static constexpr size_t kMaxKeyLen                = 32;
static constexpr size_t kHashLen                   = 4;
static constexpr size_t kDataSizeLen              = 4;
static constexpr size_t kHashId                   = 0xCE47BA5E;

static uint32_t factoryDataStartAddress = 0;

static uint8_t ReadDataMemCpy(uint16_t num, uint32_t src, uint8_t * dst)
{
    memcpy(dst, (void *) (src), num);
    return 0;
}

MW320FactoryDataProvider & MW320FactoryDataProvider::GetDefaultInstance()
{
    static MW320FactoryDataProvider sInstance;
    return sInstance;
}

MW320FactoryDataProvider::MW320FactoryDataProvider()
{
    maxLengths[FactoryDataId::kVerifierId]       = kSpake2pSerializedVerifier_MaxBase64Len;
    maxLengths[FactoryDataId::kSaltId]           = kSpake2pSalt_MaxBase64Len;
    maxLengths[FactoryDataId::kIcId]             = 4;
    maxLengths[FactoryDataId::kDacPrivateKeyId]  = kMaxKeyLen;
    maxLengths[FactoryDataId::kDacCertificateId] = Credentials::kMaxDERCertLength;
    maxLengths[FactoryDataId::kPaiCertificateId] = Credentials::kMaxDERCertLength;
    maxLengths[FactoryDataId::kDiscriminatorId]  = sizeof(uint32_t);
    maxLengths[FactoryDataId::kSetupPasscodeId]      = sizeof(uint32_t);
    maxLengths[FactoryDataId::kVidId]                = sizeof(uint16_t);
    maxLengths[FactoryDataId::kPidId]                = sizeof(uint16_t);
    maxLengths[FactoryDataId::kCertDeclarationId]    = Credentials::kMaxCMSSignedCDMessage;
    maxLengths[FactoryDataId::kVendorNameId]         = ConfigurationManager::kMaxVendorNameLength;
    maxLengths[FactoryDataId::kProductNameId]        = ConfigurationManager::kMaxProductNameLength;
    maxLengths[FactoryDataId::kSerialNumberId]       = ConfigurationManager::kMaxSerialNumberLength;
    maxLengths[FactoryDataId::kManufacturingDateId]  = ConfigurationManager::kMaxManufacturingDateLength;
    maxLengths[FactoryDataId::kHardwareVersionId]    = sizeof(uint16_t);
    maxLengths[FactoryDataId::kHardwareVersionStrId] = ConfigurationManager::kMaxHardwareVersionStringLength;
    maxLengths[FactoryDataId::kUniqueId]             = ConfigurationManager::kMaxUniqueIDLength;
}


CHIP_ERROR MW320FactoryDataProvider::Init()
{
    uint8_t sha256Output[SHA256HashSize] = {0};
    uint32_t start = (uint32_t)__FACTORY_DATA_START;
    uint32_t sum = 0;
    uint32_t size;
    uint32_t hashId;
    uint8_t hashReadFromFlash[kHashLen];

    // Make sure the buffer is sufficient
    for (uint8_t i = 1; i < MW320FactoryDataProvider::kNumberOfIds; i++)
    {
        sum += maxLengths[i];
    }
    if(sum > (uint32_t)__FACTORY_DATA_SIZE)
    {
        ChipLogError(DeviceLayer,
            "Max size of factory data: %" PRIu32 " is bigger than reserved factory data size: %" PRIu32,
            sum, (uint32_t)__FACTORY_DATA_SIZE
        );
    }
    /* Before trying to verify the hash, make sure there is a hash there by checking its tag */
    hashId = *(uint32_t*)start;
    if (hashId != kHashId)
    {
        ChipLogProgress(NotSpecified, "Inconsist HashId [%04lx, %04x] ", hashId, kHashId);
        return CHIP_ERROR_NOT_FOUND;
    }
    start += sizeof(kHashId);
    /* Read length of factory data from internal flash and 4 byte from computed SHA256 hash over factory data */
    size = *(uint32_t*)start;
    start += sizeof(size);

    memcpy(hashReadFromFlash, (uint8_t *)start, sizeof(hashReadFromFlash));
    start += sizeof(hashReadFromFlash);

    /* Calculate SHA256 on the factory data and compare with stored value */
    {
        SHA256Context ctx;
        SHA256Reset(&ctx);
        SHA256Input(&ctx, (uint8_t *)start, size);
        SHA256Result(&ctx, sha256Output);
    }
    if (memcmp(&sha256Output[0], &hashReadFromFlash[0], kHashLen) != 0)
    {
        ChipLogProgress(NotSpecified, "Incorrect SHA256 [%02x%02x%02x%02x, %02x%02x%02x%02x] ",
            sha256Output[0], sha256Output[1], sha256Output[2], sha256Output[3],
            hashReadFromFlash[0], hashReadFromFlash[1], hashReadFromFlash[2], hashReadFromFlash[3]);
        return CHIP_ERROR_NOT_FOUND;
    }
    /* Set factory data start address after hash id, hash and hash size */
    factoryDataStartAddress = start;
    return CHIP_NO_ERROR;
}

// format: [type:1][len:2][data:var]
CHIP_ERROR MW320FactoryDataProvider::SearchForId(uint8_t searchedType, uint8_t * pBuf, size_t bufLength, uint16_t & length)
{
    CHIP_ERROR err = CHIP_ERROR_NOT_FOUND;
    uint8_t * addr = (uint8_t *)factoryDataStartAddress;
    uint8_t type   = 0;

    while ((uint32_t)addr < (factoryDataStartAddress + __FACTORY_DATA_SIZE))
    {
        type   = addr[0];
        length = *((uint16_t *) (addr + 1));

        if ((type > MW320FactoryDataProvider::kNumberOfIds) || (length > maxLengths[type]))
        {
            break;
        }

        if (searchedType == type)
        {
            if ((type >= MW320FactoryDataProvider::kNumberOfIds) || (length > maxLengths[type]))
            {
                ChipLogError(DeviceLayer, "Failed validity check for factory data with: id=%" PRIu8 ", length=%" PRIu16, type, length);
                break;
            }
            if (bufLength < length)
            {
                err = CHIP_ERROR_BUFFER_TOO_SMALL;
            }
            else
            {
                memcpy(pBuf, addr + 3, length);
                err = CHIP_NO_ERROR;
            }
            break;
        }
        else
        {
            /* Jump past 2 bytes of length and then use length to jump to next data */
            addr = addr + 3 + length;
        }
    }

    return err;
}


CHIP_ERROR MW320FactoryDataProvider::GetCertificationDeclaration(MutableByteSpan & outBuffer)
{
    uint16_t declarationSize = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kCertDeclarationId, outBuffer.data(), outBuffer.size(), declarationSize));
    outBuffer.reduce_size(declarationSize);
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetFirmwareInformation(MutableByteSpan & out_firmware_info_buffer)
{
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetDeviceAttestationCert(MutableByteSpan & outBuffer)
{
    uint16_t certificateSize = 0;

    ReturnErrorOnFailure(SearchForId(FactoryDataId::kDacCertificateId, outBuffer.data(), outBuffer.size(), certificateSize));
    outBuffer.reduce_size(certificateSize);
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetProductAttestationIntermediateCert(MutableByteSpan & outBuffer)
{
    uint16_t certificateSize = 0;

    ReturnErrorOnFailure(SearchForId(FactoryDataId::kPaiCertificateId, outBuffer.data(), outBuffer.size(), certificateSize));
    outBuffer.reduce_size(certificateSize);
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::SignWithDeviceAttestationKey(const ByteSpan & messageToSign, MutableByteSpan & outSignBuffer)
{
    Crypto::P256ECDSASignature signature;
    Crypto::P256Keypair keypair;
    Crypto::P256SerializedKeypair serializedKeypair;

    VerifyOrReturnError(IsSpanUsable(outSignBuffer), CHIP_ERROR_INVALID_ARGUMENT);
    VerifyOrReturnError(IsSpanUsable(messageToSign), CHIP_ERROR_INVALID_ARGUMENT);
    VerifyOrReturnError(outSignBuffer.size() >= signature.Capacity(), CHIP_ERROR_BUFFER_TOO_SMALL);

    /* Get private key of DAC certificate from reserved section */
    uint8_t keyBuf[kMaxKeyLen];
    MutableByteSpan dacPrivateKeySpan(keyBuf);
    uint16_t keySize = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kDacPrivateKeyId, dacPrivateKeySpan.data(), dacPrivateKeySpan.size(), keySize));
    dacPrivateKeySpan.reduce_size(keySize);

    /* Only the private key is used when signing */
    ReturnErrorOnFailure(serializedKeypair.SetLength(Crypto::kP256_PublicKey_Length + dacPrivateKeySpan.size()));
    memcpy(serializedKeypair.Bytes() + Crypto::kP256_PublicKey_Length, dacPrivateKeySpan.data(), dacPrivateKeySpan.size());

    ReturnErrorOnFailure(keypair.Deserialize(serializedKeypair));
    ReturnErrorOnFailure(keypair.ECDSA_sign_msg(messageToSign.data(), messageToSign.size(), signature));
    return CopySpanToMutableSpan(ByteSpan{ signature.ConstBytes(), signature.Length() }, outSignBuffer);
}

CHIP_ERROR MW320FactoryDataProvider::GetSetupDiscriminator(uint16_t & setupDiscriminator)
{
    uint32_t discriminator = 0;
    uint16_t temp          = 0;

    ReturnErrorOnFailure(SearchForId(FactoryDataId::kDiscriminatorId, (uint8_t *) &discriminator, sizeof(discriminator), temp));
    setupDiscriminator = (uint16_t)(discriminator & 0x0000FFFF);

    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::SetSetupDiscriminator(uint16_t setupDiscriminator)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR MW320FactoryDataProvider::GetSpake2pIterationCount(uint32_t & iterationCount)
{
    uint16_t temp = 0;

    ReturnErrorOnFailure(SearchForId(FactoryDataId::kIcId, (uint8_t *) &iterationCount, sizeof(iterationCount), temp));
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetSpake2pSalt(MutableByteSpan & saltBuf)
{
    char saltB64[kSpake2pSalt_MaxBase64Len] = { 0 };
    uint16_t saltB64Len                     = 0;

    ReturnErrorOnFailure(SearchForId(FactoryDataId::kSaltId, (uint8_t *) (&saltB64[0]), sizeof(saltB64), saltB64Len));
    size_t saltLen = chip::Base64Decode32(saltB64, saltB64Len, reinterpret_cast<uint8_t *>(saltB64));

    ReturnErrorCodeIf(saltLen > saltBuf.size(), CHIP_ERROR_BUFFER_TOO_SMALL);
    memcpy(saltBuf.data(), saltB64, saltLen);
    saltBuf.reduce_size(saltLen);

    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetSpake2pVerifier(MutableByteSpan & verifierBuf, size_t & verifierLen)
{
    char verifierB64[kSpake2pSerializedVerifier_MaxBase64Len] = { 0 };
    uint16_t verifierB64Len                                   = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kVerifierId, (uint8_t *) &verifierB64[0], sizeof(verifierB64), verifierB64Len));

    verifierLen = chip::Base64Decode32(verifierB64, verifierB64Len, reinterpret_cast<uint8_t *>(verifierB64));
    ReturnErrorCodeIf(verifierLen > verifierBuf.size(), CHIP_ERROR_BUFFER_TOO_SMALL);
    memcpy(verifierBuf.data(), verifierB64, verifierLen);
    verifierBuf.reduce_size(verifierLen);

    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetSetupPasscode(uint32_t & setupPasscode)
{
    uint16_t length = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kSetupPasscodeId, (uint8_t *) &setupPasscode, sizeof(setupPasscode), length));
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::SetSetupPasscode(uint32_t setupPasscode)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR MW320FactoryDataProvider::GetVendorName(char * buf, size_t bufSize)
{
    uint16_t length = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kVendorNameId, (uint8_t *)buf, bufSize, length));
    buf[length] = '\0';
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetVendorId(uint16_t & vendorId)
{
    uint16_t length = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kVidId, (uint8_t *) &vendorId, sizeof(vendorId), length));
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetProductName(char * buf, size_t bufSize)
{
    uint16_t length = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kProductNameId, (uint8_t *)buf, bufSize, length));
    buf[length] = '\0';
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetProductId(uint16_t & productId)
{
    uint16_t length = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kPidId, (uint8_t *) &productId, sizeof(productId), length));
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetPartNumber(char * buf, size_t bufSize)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR MW320FactoryDataProvider::GetProductURL(char * buf, size_t bufSize)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR MW320FactoryDataProvider::GetProductLabel(char * buf, size_t bufSize)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR MW320FactoryDataProvider::GetSerialNumber(char * buf, size_t bufSize)
{
    uint16_t length = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kSerialNumberId, (uint8_t *)buf, bufSize, length));
    buf[length] = '\0';
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetManufacturingDate(uint16_t & year, uint8_t & month, uint8_t & day)
{
    uint16_t length = 0;
    uint8_t date[ConfigurationManager::kMaxManufacturingDateLength];
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kManufacturingDateId, date, ConfigurationManager::kMaxManufacturingDateLength, length));
    date[length] = '\0';
    if (length == 10 && isdigit(date[0]) && isdigit(date[1]) && isdigit(date[2]) &&
        isdigit(date[3]) && date[4] == '-' && isdigit(date[5]) && isdigit(date[6]) &&
        date[7] == '-' && isdigit(date[8]) && isdigit(date[9]))
    {
        year  = 1000 * (date[0] - '0') + 100 * (date[1] - '0') + 10 * (date[2] - '0') + date[3] - '0';
        month = 10 * (date[5] - '0') + date[6] - '0';
        day   = 10 * (date[8] - '0') + date[9] - '0';
    }
    else
    {
        ChipLogError(DeviceLayer, "Manufacturing date is not formatted correctly: YYYY-MM-DD.");
        return CHIP_ERROR_INVALID_ARGUMENT;
    }
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetHardwareVersion(uint16_t & hardwareVersion)
{
    uint16_t length = 0;
    ReturnErrorOnFailure(
        SearchForId(FactoryDataId::kHardwareVersionId, (uint8_t *) &hardwareVersion, sizeof(hardwareVersion), length)
    );
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetHardwareVersionString(char * buf, size_t bufSize)
{
    uint16_t length = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kHardwareVersionStrId, (uint8_t *)buf, bufSize, length));    buf[length] = '\0';
    return CHIP_NO_ERROR;
}

CHIP_ERROR MW320FactoryDataProvider::GetRotatingDeviceIdUniqueId(MutableByteSpan & uniqueIdSpan)
{
    ChipError err = CHIP_ERROR_WRONG_KEY_TYPE;
#if CHIP_ENABLE_ROTATING_DEVICE_ID && defined(CHIP_DEVICE_CONFIG_ROTATING_DEVICE_ID_UNIQUE_ID)
    char uniqueId[ConfigurationManager::kMaxUniqueIDLength] = { 0 };
    uint16_t uniqueIdLen = 0;
    ReturnErrorOnFailure(SearchForId(FactoryDataId::kUniqueId, (uint8_t *) (&uniqueId[0]), sizeof(uniqueId), uniqueIdLen));
    static_assert(
        ConfigurationManager::kRotatingDeviceIDUniqueIDLength >= ConfigurationManager::kMinRotatingDeviceIDUniqueIDLength,
        "Length of unique ID for rotating device ID is smaller than minimum."
    );
    VerifyOrReturnValue((uniqueIdLen <= uniqueIdSpan.size()), CHIP_ERROR_BUFFER_TOO_SMALL,
        ChipLogProgress(NotSpecified, "GetRotatingDeviceIdUniqueId: %u, %u", uniqueIdLen, uniqueIdSpan.size()));
    VerifyOrReturnValue((uniqueIdLen == ConfigurationManager::kRotatingDeviceIDUniqueIDLength), CHIP_ERROR_BUFFER_TOO_SMALL,
        ChipLogProgress(NotSpecified, "kRotatingDeviceIDUniqueIDLength: %u \r\n", ConfigurationManager::kRotatingDeviceIDUniqueIDLength));
    memcpy(uniqueIdSpan.data(), uniqueId, uniqueIdLen);
    uniqueIdSpan.reduce_size(uniqueIdLen);
#endif
    return err;

}

} // namespace DeviceLayer
} // namespace chip
