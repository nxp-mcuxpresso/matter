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

#include <lib/core/CHIPError.h>
#include <crypto/CHIPCryptoPAL.h>
#include <lib/core/CHIPError.h>
#include <lib/core/CHIPTLV.h>
#include <lib/support/Base64.h>
#include <lib/support/Span.h>

#ifndef FACTORY_DATA_PROVIDER_ENABLE_TESTS
#define FACTORY_DATA_PROVIDER_RUN_TESTS 0
#endif

#if FACTORY_DATA_PROVIDER_RUN_TESTS
#include <credentials/CertificationDeclaration.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/attestation_verifier/DefaultDeviceAttestationVerifier.h>
#include <credentials/attestation_verifier/DeviceAttestationVerifier.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <credentials/examples/ExampleDACs.h>
#include <credentials/examples/ExamplePAI.h>
#endif

#include "FactoryDataProvider.h"
#include "DataReaderEncryptedDCP.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#include "mflash_drv.h"

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#ifndef FACTORY_DATA_PROVIDER_LOG
#define FACTORY_DATA_PROVIDER_LOG 0
#endif

#if FACTORY_DATA_PROVIDER_LOG
#include "fsl_debug_console.h"
#define FACTORY_DATA_PROVIDER_PRINTF(...)                                                                                         \
    PRINTF("[%s] ", __FUNCTION__);                                                                                                \
    PRINTF(__VA_ARGS__);                                                                                                          \
    PRINTF("\n\r");
#else
#define FACTORY_DATA_PROVIDER_PRINTF(...)
#endif

#define VERIFIER_ID 1
#define SALT_ID 2
#define IC_ID 3
#define DAC_PRIVATE_KEY_ID 4
#define DAC_CERTIFICATE_ID 5
#define PAI_CERTIFICATE_ID 6
#define DISCRIMINATOR_ID 7
#define MAX_KEY_LEN 32
#define BLOCK_SIZE_16_BYTES 16
#define BLOCK_SIZE_4_BYTES 4
#define SHA256_OUTPUT_SIZE 32
#define HASH_ID 0xCE47BA5E

/* Grab symbol for the base address from the linker file. */
extern uint32_t __FACTORY_DATA_START_OFFSET[];
extern uint32_t __FACTORY_DATA_SIZE[];

using namespace ::chip::Credentials;
using namespace ::chip::Crypto;

namespace chip {
namespace DeviceLayer {

FactoryDataProvider & FactoryDataProvider::GetDefaultInstance()
{
    static FactoryDataProvider sInstance;
    return sInstance;
}

uint8_t * FactoryDataProvider::SearchForId(uint8_t searchedType, uint8_t *pBuf, size_t bufLength, uint16_t &length)
{
    uint8_t type = 0;
    uint32_t index = 0;
    uint8_t *addrContent = NULL;
    uint8_t *factoryDataAddress = &factoryDataRamBuffer[0];
    uint32_t factoryDataSize = sizeof(factoryDataRamBuffer);
    uint16_t currentLen = 0;

    while (index < factoryDataSize)
    {
        /* Read the type */
        memcpy((uint8_t *)&type, factoryDataAddress+index, sizeof(type));
        index += sizeof(type);

        /* Read the len */
        memcpy((uint8_t *)&currentLen, factoryDataAddress+index, sizeof(currentLen));
        index += sizeof(currentLen);

        /* Check if the type gotten is the expected one */
        if (searchedType == type)
        {
            FACTORY_DATA_PROVIDER_PRINTF("type = %d, currentLen = %d, bufLength =%d", type, currentLen, bufLength);
            /* If pBuf is null it means that we only want to know if the Type has been found */
            if (pBuf != NULL)
            {
                /* If the buffer given is too small, fill only the available space */
                if (bufLength < currentLen)
                {
                    currentLen = bufLength;
                }
                memcpy((uint8_t *)pBuf, factoryDataAddress+index, currentLen); 
            }
            length = currentLen;
            addrContent = factoryDataAddress+index;
            break;
        }
        else if (type == 0)
        {
            /* No more type available , break the loop */
            break;
        }
        else
        {
            /* Jump to next data */
            index += currentLen;
        }
    }

    return addrContent;
}

CHIP_ERROR FactoryDataProvider::LoadKeypairFromRaw(ByteSpan privateKey, ByteSpan publicKey, Crypto::P256Keypair & keypair)
{
    Crypto::P256SerializedKeypair serialized_keypair;
    ReturnErrorOnFailure(serialized_keypair.SetLength(privateKey.size() + publicKey.size()));
    memcpy(serialized_keypair.Bytes(), publicKey.data(), publicKey.size());
    memcpy(serialized_keypair.Bytes() + publicKey.size(), privateKey.data(), privateKey.size());
    return keypair.Deserialize(serialized_keypair);
}

CHIP_ERROR FactoryDataProvider::Init(DataReaderEncryptedDCP *encryptedReader)
{
    uint16_t len;
    status_t status;
    uint32_t factoryDataAddress = (uint32_t)__FACTORY_DATA_START_OFFSET;
    uint32_t factoryDataSize = (uint32_t)__FACTORY_DATA_SIZE;
    uint32_t sizeOfFactoryData = 0;
    uint32_t hashId;
	uint32_t kHashId = HASH_ID;
    uint8_t currentBlock[BLOCK_SIZE_16_BYTES];
    uint8_t hashReadFromFlash[BLOCK_SIZE_4_BYTES];
    uint8_t calculatedHash[SHA256_OUTPUT_SIZE];
    size_t outputHashSize;
    uint16_t i;
    CHIP_ERROR res;

    /* Init mflash */
    status = mflash_drv_init();

    if (status != kStatus_Success || factoryDataSize > sizeof(factoryDataRamBuffer))
        return CHIP_ERROR_INTERNAL;

    /* Read hash id saved in flash */
    if (mflash_drv_read(factoryDataAddress, (uint32_t *)&hashId, sizeof(hashId)) != kStatus_Success)
    {
        return CHIP_ERROR_INTERNAL;
    }

    if (memcmp(&hashId, &kHashId, BLOCK_SIZE_4_BYTES) != 0)
    {
        return CHIP_ERROR_NOT_FOUND;
    }

    /* Update address to start after hash id to read size of factory data */
    factoryDataAddress += BLOCK_SIZE_4_BYTES;

    /* Read size of data saved in flash to be used in calculating the hash */
    if (mflash_drv_read(factoryDataAddress, (uint32_t *)&sizeOfFactoryData, sizeof(sizeOfFactoryData)) != kStatus_Success)
    {
        return CHIP_ERROR_INTERNAL;
    }

    /* Update address to start after size to read actual hash */
    factoryDataAddress += BLOCK_SIZE_4_BYTES;

    /* Read hash saved in flash */
    if (mflash_drv_read(factoryDataAddress, (uint32_t *)&hashReadFromFlash[0], sizeof(hashReadFromFlash)) != kStatus_Success)
    {
        return CHIP_ERROR_INTERNAL;
    }

    /* Update address to start after hash to read factory data */
    factoryDataAddress += BLOCK_SIZE_4_BYTES;

    /* Load the buffer into RAM by reading each 16 bytes blocks */
    for (i=0; i<(factoryDataSize/BLOCK_SIZE_16_BYTES); i++)
    {
        if (mflash_drv_read(factoryDataAddress+i*BLOCK_SIZE_16_BYTES, (uint32_t *)&currentBlock[0], sizeof(currentBlock)) != kStatus_Success)
        {
            return CHIP_ERROR_INTERNAL;
        }

        /* Decrypt data if an encryptedReader has been given as an arg */
        if (encryptedReader != nullptr)
        {
            res = encryptedReader->ReadData(&factoryDataRamBuffer[i*BLOCK_SIZE_16_BYTES], &currentBlock[0], sizeof(currentBlock));
            if (res != CHIP_NO_ERROR)
                return res;
        }
        else
        {
            /* Store the block unencrypted */
            memcpy(&factoryDataRamBuffer[i*BLOCK_SIZE_16_BYTES], &currentBlock[0], sizeof(currentBlock));
        }
    }

    /* Calculate SHA256 value over the factory data and compare with stored value */
    if (encryptedReader != nullptr)
    {
    	res = encryptedReader->Hash256(&factoryDataRamBuffer[0], sizeOfFactoryData, &calculatedHash[0], &outputHashSize);
    }
    else
    {
    	res = CHIP_ERROR_INTERNAL;
    }

    if (res != CHIP_NO_ERROR)
        return res;

    if (memcmp(&calculatedHash[0], &hashReadFromFlash[0], BLOCK_SIZE_4_BYTES) != 0)
    {
        return CHIP_ERROR_NOT_FOUND;
    }

    if (SearchForId(VERIFIER_ID, NULL, 0, len) == NULL)
        return CHIP_ERROR_INTERNAL;
    FACTORY_DATA_PROVIDER_PRINTF("[%d] len = %d", VERIFIER_ID, len);
    if (SearchForId(SALT_ID, NULL, 0, len) == NULL)
        return CHIP_ERROR_INTERNAL;
    FACTORY_DATA_PROVIDER_PRINTF("[%d] len = %d", SALT_ID, len);
    if (SearchForId(IC_ID, NULL, 0, len) == NULL)
        return CHIP_ERROR_INTERNAL;
    FACTORY_DATA_PROVIDER_PRINTF("[%d] len = %d", IC_ID, len);
    if (SearchForId(DAC_PRIVATE_KEY_ID, NULL, 0, len) == NULL)
        return CHIP_ERROR_INTERNAL;
    FACTORY_DATA_PROVIDER_PRINTF("[%d] len = %d", DAC_PRIVATE_KEY_ID, len);
    if (SearchForId(DAC_CERTIFICATE_ID, NULL, 0, len) == NULL)
        return CHIP_ERROR_INTERNAL;
    FACTORY_DATA_PROVIDER_PRINTF("[%d] len = %d", DAC_CERTIFICATE_ID, len);
    if (SearchForId(PAI_CERTIFICATE_ID, NULL, 0, len) == NULL)
        return CHIP_ERROR_INTERNAL;
    FACTORY_DATA_PROVIDER_PRINTF("[%d] len = %d", PAI_CERTIFICATE_ID, len);
    if (SearchForId(DISCRIMINATOR_ID, NULL, 0, len) == NULL)
        return CHIP_ERROR_INTERNAL;

    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProvider::GetCertificationDeclaration(MutableByteSpan & outBuffer)
{
    constexpr uint8_t kCdForAllExamples[] = CHIP_DEVICE_CONFIG_CERTIFICATION_DECLARATION;
    return CopySpanToMutableSpan(ByteSpan{ kCdForAllExamples }, outBuffer);
}

CHIP_ERROR FactoryDataProvider::GetFirmwareInformation(MutableByteSpan & out_firmware_info_buffer)
{
    out_firmware_info_buffer.reduce_size(0);
    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProvider::GetDeviceAttestationCert(MutableByteSpan & outBuffer)
{
    CHIP_ERROR err = CHIP_ERROR_NOT_FOUND;
    uint16_t certificateSize = 0;
    uint8_t *certificateAddr = SearchForId(DAC_CERTIFICATE_ID, NULL, 0, certificateSize);

    if (certificateAddr != NULL && certificateSize > 0 && outBuffer.size() >= certificateSize)
    {
        memcpy(outBuffer.data(), certificateAddr, certificateSize);
        err = CHIP_NO_ERROR;
    }
    outBuffer.reduce_size(certificateSize);
    return err;
}

CHIP_ERROR FactoryDataProvider::GetProductAttestationIntermediateCert(MutableByteSpan & outBuffer)
{
    CHIP_ERROR err = CHIP_ERROR_NOT_FOUND;
    uint16_t certificateSize = 0;
    if (SearchForId(PAI_CERTIFICATE_ID, outBuffer.data(), outBuffer.size(), certificateSize) != NULL)
        err = CHIP_NO_ERROR;
    outBuffer.reduce_size(certificateSize);
    return err;
}

CHIP_ERROR FactoryDataProvider::SignWithDeviceAttestationKey(const ByteSpan & digestToSign, MutableByteSpan & outSignBuffer)
{
    Crypto::P256ECDSASignature signature;
    Crypto::P256Keypair keypair;

    VerifyOrReturnError(IsSpanUsable(outSignBuffer), CHIP_ERROR_INVALID_ARGUMENT);
    VerifyOrReturnError(IsSpanUsable(digestToSign), CHIP_ERROR_INVALID_ARGUMENT);
    VerifyOrReturnError(outSignBuffer.size() >= signature.Capacity(), CHIP_ERROR_BUFFER_TOO_SMALL);

    // In a non-exemplary implementation, the public key is not needed here. It is used here merely because
    // Crypto::P256Keypair is only (currently) constructable from raw keys if both private/public keys are present.
    Crypto::P256PublicKey dacPublicKey;
    uint16_t certificateSize = 0;
    uint8_t *certificateAddr = SearchForId(DAC_CERTIFICATE_ID, NULL, 0, certificateSize);
    if (certificateAddr == NULL)
        return CHIP_ERROR_NOT_FOUND;
    MutableByteSpan dacCertSpan(certificateAddr, certificateSize);

    /* Extract Public Key of DAC certificate from itself */
    ReturnErrorOnFailure(Crypto::ExtractPubkeyFromX509Cert(dacCertSpan, dacPublicKey));

    /* Get private key of DAC certificate from reserved section */
    uint16_t keySize = 0;
    uint8_t *keyAddr = SearchForId(DAC_PRIVATE_KEY_ID, NULL, 0, keySize);
    if (keyAddr == NULL)
        return CHIP_ERROR_NOT_FOUND;
    MutableByteSpan dacPrivateKeySpan(keyAddr, keySize);

    ReturnErrorOnFailure(LoadKeypairFromRaw(ByteSpan(dacPrivateKeySpan.data(), dacPrivateKeySpan.size()), ByteSpan(dacPublicKey.Bytes(), dacPublicKey.Length()), keypair));

    ReturnErrorOnFailure(keypair.ECDSA_sign_msg(digestToSign.data(), digestToSign.size(), signature));

    return CopySpanToMutableSpan(ByteSpan{ signature.ConstBytes(), signature.Length() }, outSignBuffer);
}

CHIP_ERROR FactoryDataProvider::GetSetupDiscriminator(uint16_t & setupDiscriminator)
{
    CHIP_ERROR err = CHIP_ERROR_NOT_FOUND;
    uint16_t discriminatorLen = 0;

    if (SearchForId(DISCRIMINATOR_ID, (uint8_t *)&setupDiscriminator, sizeof(setupDiscriminator), discriminatorLen) != NULL)
        err = CHIP_NO_ERROR;

    return err;
}

CHIP_ERROR FactoryDataProvider:: SetSetupDiscriminator(uint16_t setupDiscriminator)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR FactoryDataProvider::GetSpake2pIterationCount(uint32_t & iterationCount)
{
    uint16_t iterationCountLen = 0;
    CHIP_ERROR err = CHIP_ERROR_NOT_FOUND;

    if (SearchForId(IC_ID, (uint8_t *)&iterationCount, sizeof(iterationCount), iterationCountLen) != NULL)
        err = CHIP_NO_ERROR;

    return err;
}

CHIP_ERROR FactoryDataProvider::GetSpake2pSalt(MutableByteSpan & saltBuf)
{
    static constexpr size_t kSpake2pSalt_MaxBase64Len = BASE64_ENCODED_LEN(chip::Crypto::kSpake2p_Max_PBKDF_Salt_Length) + 1;
    uint8_t saltB64[kSpake2pSalt_MaxBase64Len];
    uint16_t saltB64Len                       = 0;

    if (SearchForId(SALT_ID, &saltB64[0], sizeof(saltB64), saltB64Len) == NULL)
        return CHIP_ERROR_NOT_FOUND;

    size_t saltLen = chip::Base64Decode32((char *)saltB64, saltB64Len, reinterpret_cast<uint8_t *>(saltB64));

    ReturnErrorCodeIf(saltLen > saltBuf.size(), CHIP_ERROR_BUFFER_TOO_SMALL);
    memcpy(saltBuf.data(), saltB64, saltLen);
    saltBuf.reduce_size(saltLen);

    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProvider::GetSpake2pVerifier(MutableByteSpan & verifierBuf, size_t & verifierLen)
{
    static constexpr size_t kSpake2pSerializedVerifier_MaxBase64Len = 
        BASE64_ENCODED_LEN(chip::Crypto::kSpake2p_VerifierSerialized_Length) + 1;
    uint8_t verifierB64[kSpake2pSerializedVerifier_MaxBase64Len];
    uint16_t verifierB64Len                                     = 0;
    
    if (SearchForId(VERIFIER_ID, &verifierB64[0], sizeof(verifierB64), verifierB64Len) == NULL)
        return CHIP_ERROR_NOT_FOUND;

    verifierLen = chip::Base64Decode32((char *) verifierB64, verifierB64Len, reinterpret_cast<uint8_t *>(verifierB64));
    ReturnErrorCodeIf(verifierLen > verifierBuf.size(), CHIP_ERROR_BUFFER_TOO_SMALL);
    memcpy(verifierBuf.data(), verifierB64, verifierLen);
    verifierBuf.reduce_size(verifierLen);

    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProvider::GetSetupPasscode(uint32_t & setupPasscode)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR FactoryDataProvider::SetSetupPasscode(uint32_t setupPasscode)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR FactoryDataProvider::GetVendorName(char * buf, size_t bufSize)
{
    ReturnErrorCodeIf(bufSize < sizeof(CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME), CHIP_ERROR_BUFFER_TOO_SMALL);
    strcpy(buf, CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME);
    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProvider::GetVendorId(uint16_t & vendorId)
{
    vendorId = static_cast<uint16_t>(CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID);
    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProvider::GetProductName(char * buf, size_t bufSize)
{
    ReturnErrorCodeIf(bufSize < sizeof(CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME), CHIP_ERROR_BUFFER_TOO_SMALL);
    strcpy(buf, CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME);
    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProvider::GetProductId(uint16_t & productId)
{
    productId = static_cast<uint16_t>(CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_ID);
    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProvider::GetSerialNumber(char * buf, size_t bufSize)
{
    ChipError err       = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    size_t serialNumLen = 0; // without counting null-terminator

#ifdef CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER
    if (CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER[0] != 0)
    {
        ReturnErrorCodeIf(sizeof(CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER) > bufSize, CHIP_ERROR_BUFFER_TOO_SMALL);
        memcpy(buf, CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER, sizeof(CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER));
        serialNumLen = sizeof(CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER) - 1;
        err          = CHIP_NO_ERROR;
    }
#endif // CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER

    ReturnErrorCodeIf(serialNumLen >= bufSize, CHIP_ERROR_BUFFER_TOO_SMALL);
    ReturnErrorCodeIf(buf[serialNumLen] != 0, CHIP_ERROR_INVALID_STRING_LENGTH);

    return err;
}

CHIP_ERROR FactoryDataProvider::GetManufacturingDate(uint16_t & year, uint8_t & month, uint8_t & day)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR FactoryDataProvider::GetHardwareVersion(uint16_t & hardwareVersion)
{
    hardwareVersion = static_cast<uint16_t>(CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION);
    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProvider::GetHardwareVersionString(char * buf, size_t bufSize)
{
    ReturnErrorCodeIf(bufSize < sizeof(CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING), CHIP_ERROR_BUFFER_TOO_SMALL);
    strcpy(buf, CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING);
    return CHIP_NO_ERROR;
}

CHIP_ERROR FactoryDataProvider::GetRotatingDeviceIdUniqueId(MutableByteSpan & uniqueIdSpan)
{
    ChipError err = CHIP_ERROR_WRONG_KEY_TYPE;
#if CHIP_ENABLE_ROTATING_DEVICE_ID && defined(CHIP_DEVICE_CONFIG_ROTATING_DEVICE_ID_UNIQUE_ID)
    static_assert(ConfigurationManager::kRotatingDeviceIDUniqueIDLength >= ConfigurationManager::kMinRotatingDeviceIDUniqueIDLength,
                  "Length of unique ID for rotating device ID is smaller than minimum.");
    constexpr uint8_t uniqueId[] = CHIP_DEVICE_CONFIG_ROTATING_DEVICE_ID_UNIQUE_ID;

    ReturnErrorCodeIf(sizeof(uniqueId) > uniqueIdSpan.size(), CHIP_ERROR_BUFFER_TOO_SMALL);
    ReturnErrorCodeIf(sizeof(uniqueId) != ConfigurationManager::kRotatingDeviceIDUniqueIDLength, CHIP_ERROR_BUFFER_TOO_SMALL);
    memcpy(uniqueIdSpan.data(), uniqueId, sizeof(uniqueId));
    uniqueIdSpan.reduce_size(sizeof(uniqueId));
    return CHIP_NO_ERROR;
#endif
    return err;
}

void FactoryDataProvider::FactoryDataProviderRunTests(void)
{
#if FACTORY_DATA_PROVIDER_RUN_TESTS
    static const ByteSpan kExpectedDacPublicKey = DevelopmentCerts::kDacPublicKey;
    constexpr uint8_t kExampleDigest[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12,
                                           0x13, 0x14, 0x15, 0x16, 0x17, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
                                           0x26, 0x27, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37 };

    // Sign using the example attestation private key
    P256ECDSASignature da_signature;
    MutableByteSpan out_sig_span(da_signature.Bytes(), da_signature.Capacity());
    CHIP_ERROR err = SignWithDeviceAttestationKey(ByteSpan{ kExampleDigest }, out_sig_span);
    assert(err == CHIP_NO_ERROR);

    assert( out_sig_span.size() == kP256_ECDSA_Signature_Length_Raw);
    da_signature.SetLength(out_sig_span.size());

    // Get DAC from the provider
    uint8_t dac_cert_buf[kMaxDERCertLength];
    MutableByteSpan dac_cert_span(dac_cert_buf);

    memset(dac_cert_span.data(), 0, dac_cert_span.size());
    err = GetDeviceAttestationCert(dac_cert_span);
    assert( err == CHIP_NO_ERROR);

    // Extract public key from DAC, prior to signature verification
    P256PublicKey dac_public_key;
    err = ExtractPubkeyFromX509Cert(dac_cert_span, dac_public_key);
    assert( err == CHIP_NO_ERROR);
    assert( dac_public_key.Length() == kExpectedDacPublicKey.size());
    assert( 0 == memcmp(dac_public_key.ConstBytes(), kExpectedDacPublicKey.data(), kExpectedDacPublicKey.size()));

    // Verify round trip signature
    err = dac_public_key.ECDSA_validate_hash_signature(&kExampleDigest[0], sizeof(kExampleDigest), da_signature);
    assert( err == CHIP_NO_ERROR);
#endif
}

} // namespace DeviceLayer
} // namespace chip
