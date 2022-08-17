/*
 *    Copyright (c) 2022, The OpenThread Authors.
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    3. Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 *    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *    @file
 *          Provides implementations for several functions defined in
 *          openthread/src/core/crypto/crypto_platform.cpp. The purpose
 *          is to bypass mbedtls_entropy_func and use a strong source
 *          for DRBG seed.
 */

#include <openthread/platform/entropy.h>
#include <core/common/code_utils.hpp>
#include <core/common/debug.hpp>
#include <core/common/instance.hpp>
#include <core/crypto/mbedtls.hpp>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>

#if (!defined(MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES) && \
     (!defined(MBEDTLS_NO_PLATFORM_ENTROPY) || defined(MBEDTLS_HAVEGE_C) || defined(MBEDTLS_ENTROPY_HARDWARE_ALT)))
#define OT_MBEDTLS_STRONG_DEFAULT_ENTROPY_PRESENT
#endif

#ifndef OT_MBEDTLS_STRONG_DEFAULT_ENTROPY_PRESENT
static constexpr uint16_t kEntropyMinThreshold = 16;
#endif

static mbedtls_ctr_drbg_context sCtrDrbgContext;
static mbedtls_entropy_context  sEntropyContext;

#ifndef OT_MBEDTLS_STRONG_DEFAULT_ENTROPY_PRESENT

static int handleMbedtlsEntropyPoll(void *aData, unsigned char *aOutput, size_t aInLen, size_t *aOutLen)
{
    int rval = MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;

    SuccessOrExit(otPlatEntropyGet(reinterpret_cast<uint8_t *>(aOutput), static_cast<uint16_t>(aInLen)));
    rval = 0;

    VerifyOrExit(aOutLen != nullptr);
    *aOutLen = aInLen;

exit:
    OT_UNUSED_VARIABLE(aData);
    return rval;
}

#endif // OT_MBEDTLS_STRONG_DEFAULT_ENTROPY_PRESENT

static int strong_entropy_func(void *data, unsigned char *output, size_t len)
{
    int result = MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;

    SuccessOrExit(otPlatEntropyGet(reinterpret_cast<uint8_t *>(output), static_cast<uint16_t>(len)));
    result = 0;

exit:
    return result;
}

void otPlatCryptoRandomInit(void)
{
    mbedtls_entropy_init(&sEntropyContext);

#ifndef OT_MBEDTLS_STRONG_DEFAULT_ENTROPY_PRESENT
    mbedtls_entropy_add_source(&sEntropyContext, handleMbedtlsEntropyPoll, nullptr, kEntropyMinThreshold,
                               MBEDTLS_ENTROPY_SOURCE_STRONG);
#endif

    mbedtls_ctr_drbg_init(&sCtrDrbgContext);

    int rval = mbedtls_ctr_drbg_seed(&sCtrDrbgContext, strong_entropy_func, &sEntropyContext, nullptr, 0);
    OT_ASSERT(rval == 0);
    OT_UNUSED_VARIABLE(rval);
}

void otPlatCryptoRandomDeinit(void)
{
    mbedtls_entropy_free(&sEntropyContext);
    mbedtls_ctr_drbg_free(&sCtrDrbgContext);
}

otError otPlatCryptoRandomGet(uint8_t *aBuffer, uint16_t aSize)
{
    return ot::Crypto::MbedTls::MapError(
        mbedtls_ctr_drbg_random(&sCtrDrbgContext, static_cast<unsigned char *>(aBuffer), static_cast<size_t>(aSize)));
}
