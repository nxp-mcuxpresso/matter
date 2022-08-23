/*
 * Copyright 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "SecLib.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "fsl_common.h"

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#if defined CPU_JN518X
    #include "fsl_rng.h"
#else
    #include "fsl_trng.h"
#endif
#endif

#define CLEAN_RETURN(value) \
    {                       \
        ret = value;        \
        goto cleanup;       \
    }

#if defined(MBEDTLS_SHA1_ALT) || defined(MBEDTLS_SHA256_ALT)
/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize(void *v, size_t n)
{
    volatile unsigned char *p = v;
    while (n--)
        *p++ = 0;
}
#endif /* MBEDTLS_SHA1_ALT || MBEDTLS_SHA256_ALT */

/******************************************************************************/
/******************** CRYPTO_InitHardware **************************************/
/******************************************************************************/
/*!
 * @brief Application init for various Crypto blocks.
 *
 * This function is provided to be called by MCUXpresso SDK applications.
 * It calls basic init for Crypto Hw acceleration and Hw entropy modules.
 */
void CRYPTO_InitHardware(void)
{
    { /* Init RNG module.*/
#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#if defined CPU_JN518X
    #if defined(RNG)
    #define TRNG0 RNG
    #endif
#else
    #if defined(TRNG)
    #define TRNG0 TRNG
    #endif
#endif
        trng_config_t trngConfig;

        TRNG_GetDefaultConfig(&trngConfig);
        /* Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.*/
        /* Initialize TRNG */
        TRNG_Init(TRNG0, &trngConfig);
#endif
    }
}

/******************************************************************************/
/*************************** AES **********************************************/
/******************************************************************************/

#if defined(MBEDTLS_AES_C)
#include "mbedtls/aes.h"

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    uint32_t *RK;
    
#ifdef MBEDTLS_AES_ALT_NO_192
    if (keybits == 192u)
    {
        return (MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE);
    }
#endif

#ifdef MBEDTLS_AES_ALT_NO_256
    if (keybits == 256u)
    {
        return (MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE);
    }
#endif

    const unsigned char *key_tmp = key;    
    ctx->rk = RK = ctx->buf;
    memcpy(RK, key_tmp, keybits / 8);

    switch (keybits)
    { /* Set keysize in bytes.*/
        case 128:
            ctx->nr = 16;
            break;
        case 192:
            ctx->nr = 24;
            break;
        case 256:
            ctx->nr = 32;
            break;
        default:
            return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }

    return (0);
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    uint32_t *RK;
    
#ifdef MBEDTLS_AES_ALT_NO_192
    if (keybits == 192u)
    {
        return (MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE);
    }
#endif

#ifdef MBEDTLS_AES_ALT_NO_256
    if (keybits == 256u)
    {
        return (MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE);
    }
#endif

    ctx->rk = RK = ctx->buf;
    const unsigned char *key_tmp = key;
    memcpy(RK, key_tmp, keybits / 8);

    switch (keybits)
    {
        case 128:
            ctx->nr = 16;
            break;
        case 192:
            ctx->nr = 24;
            break;
        case 256:
            ctx->nr = 32;
            break;
        default:
            return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }

    return 0;
}

/*
 * AES-ECB block encryption
 */
int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16])
{
    uint8_t *key;

    key = (uint8_t *)ctx->rk;
    AES_128_Encrypt(input, key, output);

    return (0);
}

/*
 * AES-ECB block decryption
 */
int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16])
{
    uint8_t *key;

    key = (uint8_t *)ctx->rk;
    AES_128_Decrypt(input, key, output);

    return (0);
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
#if defined(MBEDTLS_FREESCALE_LPC_AES)
/*
 * AES-CBC buffer encryption/decryption
 */
int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    uint8_t *key;
    size_t keySize;

    if (length % 16)
        return (MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH);

    key = (uint8_t *)ctx->rk;
    keySize = (size_t)ctx->nr;

    if (mode == MBEDTLS_AES_DECRYPT)
    {
        uint8_t tmp[16];
        memcpy(tmp, input + length - 16, 16);
        AES_128_CBC_Decrypt_And_Depad(tmp, length, iv, key, output);
        memcpy(iv, tmp, 16);
    }
    else
    {
        AES_128_CBC_Encrypt(input, length, iv, key, output);
        memcpy(iv, output + length - 16, 16);
    }

    return (0);
}
#endif /* MBEDTLS_FREESCALE_LPC_AES */
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
#if defined(MBEDTLS_FREESCALE_LPC_AES)
/*
 * AES-CFB128 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb128(mbedtls_aes_context *ctx,
                             int mode,
                             size_t length,
                             size_t *iv_off,
                             unsigned char iv[16],
                             const unsigned char *input,
                             unsigned char *output)
{
    uint8_t *key;
    size_t keySize;

    key = (uint8_t *)ctx->rk;
    keySize = (size_t)ctx->nr;
    AES_SetKey(AES_INSTANCE, key, keySize);

    if (mode == MBEDTLS_AES_DECRYPT)
    {
        AES_DecryptCfb(AES_INSTANCE, input, output, length, iv);
    }
    else
    {
        AES_EncryptCfb(AES_INSTANCE, input, output, length, iv);
    }

    return (0);
}

/*
 * AES-CFB8 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb8(mbedtls_aes_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output)
{
    int status;
    unsigned char c;
    unsigned char ov[17];

    while (length--)
    {
        memcpy(ov, iv, 16);
        status = mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, iv, iv);
        if (status != 0)
        {
            return status;
        }

        if (mode == MBEDTLS_AES_DECRYPT)
            ov[16] = *input;

        c = *output++ = (unsigned char)(iv[0] ^ *input++);

        if (mode == MBEDTLS_AES_ENCRYPT)
            ov[16] = c;

        memcpy(iv, ov + 1, 16);
    }

    return (0);
}
#endif /* MBEDTLS_FREESCALE_LPC_AES */
#endif /* MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
#if defined(MBEDTLS_FREESCALE_LPC_AES)
/*
 * AES-CTR buffer encryption/decryption
 */
int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *nc_off,
                          unsigned char nonce_counter[16],
                          unsigned char stream_block[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    uint8_t *key;
    size_t keySize;

    key = (uint8_t *)ctx->rk;
    keySize = (size_t)ctx->nr;

    AES_128_CTR(input, length, nonce_counter, key, output);

    return (0);
}
#endif /* MBEDTLS_FREESCALE_LPC_AES */
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#if defined(MBEDTLS_GCM_C)
#if defined(MBEDTLS_FREESCALE_LPC_AES_GCM)

#include "mbedtls/gcm.h"

int mbedtls_gcm_crypt_and_tag(mbedtls_gcm_context *ctx,
                              int mode,
                              size_t length,
                              const unsigned char *iv,
                              size_t iv_len,
                              const unsigned char *add,
                              size_t add_len,
                              const unsigned char *input,
                              unsigned char *output,
                              size_t tag_len,
                              unsigned char *tag)
{
    status_t status;
    uint8_t *key;
    size_t keySize;
    mbedtls_aes_context *aes_ctx;

    ctx->len = length;
    ctx->add_len = add_len;
    aes_ctx = (mbedtls_aes_context *)ctx->cipher_ctx.cipher_ctx;
    key = (uint8_t *)aes_ctx->rk;
    keySize = (size_t)aes_ctx->nr;

    status = AES_SetKey(AES_INSTANCE, key, keySize);
    if (status != kStatus_Success)
    {
        return MBEDTLS_ERR_GCM_BAD_INPUT;
    }

    if (mode == MBEDTLS_GCM_ENCRYPT)
    {
        status = AES_EncryptTagGcm(AES_INSTANCE, input, output, length, iv, iv_len, add, add_len, tag, tag_len);
    }
    else
    {
        status = AES_DecryptTagGcm(AES_INSTANCE, input, output, length, iv, iv_len, add, add_len, tag, tag_len);
    }

    if (status == kStatus_InvalidArgument)
    {
        return MBEDTLS_ERR_GCM_BAD_INPUT;
    }
    else if (status != kStatus_Success)
    {
        return MBEDTLS_ERR_GCM_AUTH_FAILED;
    }

    return 0;
}

int mbedtls_gcm_auth_decrypt(mbedtls_gcm_context *ctx,
                             size_t length,
                             const unsigned char *iv,
                             size_t iv_len,
                             const unsigned char *add,
                             size_t add_len,
                             const unsigned char *tag,
                             size_t tag_len,
                             const unsigned char *input,
                             unsigned char *output)
{
    unsigned char tag_copy[16];

    memcpy(tag_copy, tag, tag_len);
    return (mbedtls_gcm_crypt_and_tag(ctx, MBEDTLS_GCM_DECRYPT, length, iv, iv_len, add, add_len, input, output,
                                      tag_len, tag_copy));
}

#endif /* MBEDTLS_FREESCALE_LPC_AES */
#endif /* MBEDTLS_GCM_C */

#endif /* MBEDTLS_AES_C */


/******************************************************************************/
/*************************** SHA1 *********************************************/
/******************************************************************************/

#if defined(MBEDTLS_SHA1_C)

#if defined(MBEDTLS_FREESCALE_LPC_SHA1)
#include "mbedtls/sha1.h"

void mbedtls_sha1_init(mbedtls_sha1_context *ctx)
{
    memset(ctx, 0, sizeof(mbedtls_sha1_context));
}

void mbedtls_sha1_free(mbedtls_sha1_context *ctx)
{
    if (ctx == NULL)
        return;

    mbedtls_zeroize(ctx, sizeof(mbedtls_sha1_context));
}

void mbedtls_sha1_clone(mbedtls_sha1_context *dst, const mbedtls_sha1_context *src)
{
    memcpy(dst, src, sizeof(mbedtls_sha1_context));
}

/*
 * SHA-1 context setup
 */
int mbedtls_sha1_starts_ret(mbedtls_sha1_context *ctx)
{
    status_t ret = kStatus_Fail;
    ret = SHA_Init(SHA_INSTANCE, ctx, kSHA_Sha1);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    return 0;
}

int mbedtls_internal_sha1_process(mbedtls_sha1_context *ctx, const unsigned char data[64])
{
    status_t ret = kStatus_Fail;
    ret = SHA_Update(SHA_INSTANCE, ctx, data, 64);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    return 0;
}

/*
 * SHA-1 process buffer
 */
int mbedtls_sha1_update_ret(mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen)
{
    status_t ret = kStatus_Fail;
    ret = SHA_Update(SHA_INSTANCE, ctx, input, ilen);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    return 0;
}

/*
 * SHA-1 final digest
 */
int mbedtls_sha1_finish_ret(mbedtls_sha1_context *ctx, unsigned char output[20])
{
    size_t outputSize = 20u;
    status_t ret = kStatus_Fail;
    ret = SHA_Finish(SHA_INSTANCE, ctx, output, &outputSize);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    return 0;
}

#endif /* MBEDTLS_FREESCALE_LPC_SHA1 */
#if !defined(MBEDTLS_DEPRECATED_REMOVED) && defined(MBEDTLS_SHA1_ALT)
#include "mbedtls/sha1.h"

void mbedtls_sha1_starts(mbedtls_sha1_context *ctx)
{
    mbedtls_sha1_starts_ret(ctx);
}

void mbedtls_sha1_update(mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen)
{
    mbedtls_sha1_update_ret(ctx, input, ilen);
}

void mbedtls_sha1_finish(mbedtls_sha1_context *ctx, unsigned char output[20])
{
    mbedtls_sha1_finish_ret(ctx, output);
}

void mbedtls_sha1_process(mbedtls_sha1_context *ctx, const unsigned char data[64])
{
    mbedtls_internal_sha1_process(ctx, data);
}
#endif /* MBEDTLS_DEPRECATED_REMOVED */
#endif /* MBEDTLS_SHA1_C */

/******************************************************************************/
/*************************** SHA256********************************************/
/******************************************************************************/

#if defined(MBEDTLS_SHA256_C)

#if defined(MBEDTLS_FREESCALE_LPC_SHA256)
#include "mbedtls/sha256.h"

void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    memset(ctx, 0, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if (ctx == NULL)
        return;

    mbedtls_zeroize(ctx, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst, const mbedtls_sha256_context *src)
{
    memcpy(dst, src, sizeof(*dst));
}

/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts_ret(mbedtls_sha256_context *ctx, int is224)
{
    int status = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;
    if (!is224) /* SHA-224 not supported */
    {
        SHA256_Init(ctx);
        status = 0;
    }
    return status;
}

int mbedtls_internal_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[64])
{
    SHA256_HashUpdate(ctx, data, 64);
    return 0;
}

/*
 * SHA-256 process buffer
 */
int mbedtls_sha256_update_ret(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
    SHA256_HashUpdate(ctx, input, ilen);

    return 0;
}

/*
 * SHA-256 final digest
 */
int mbedtls_sha256_finish_ret(mbedtls_sha256_context *ctx, unsigned char output[32])
{
    SHA256_HashFinish(ctx, output);
    return 0;
}

#endif /* MBEDTLS_FREESCALE_LPC_SHA256 */
#if !defined(MBEDTLS_DEPRECATED_REMOVED) && defined(MBEDTLS_SHA256_ALT)
#include "mbedtls/sha256.h"

void mbedtls_sha256_starts(mbedtls_sha256_context *ctx, int is224)
{
    mbedtls_sha256_starts_ret(ctx, is224);
}

void mbedtls_sha256_update(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
    mbedtls_sha256_update_ret(ctx, input, ilen);
}

void mbedtls_sha256_finish(mbedtls_sha256_context *ctx, unsigned char output[32])
{
    mbedtls_sha256_finish_ret(ctx, output);
}

void mbedtls_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[64])
{
    mbedtls_internal_sha256_process(ctx, data);
}
#endif /* MBEDTLS_DEPRECATED_REMOVED */
#endif /* MBEDTLS_SHA256_C */

/* Entropy poll callback for a hardware source */
#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#if defined CPU_JN518X
    #include "fsl_rng.h"
#else
    #include "fsl_trng.h"
#endif
#endif

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    status_t result = kStatus_Success;

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#if defined CPU_JN518X
    #ifndef TRNG0
    #define TRNG0 RNG
    #endif
#else
    #ifndef TRNG0
    #define TRNG0 TRNG
    #endif
#endif
    result = TRNG_GetRandomData(TRNG0, output, len);
#endif
    if (result == kStatus_Success)
    {
        *olen = len;
        return 0;
    }
    else
    {
        return result;
    }
}

#endif /* MBEDTLS_ENTROPY_HARDWARE_ALT */

/******************************************************************************/
/*************************** FreeRTOS ********************************************/
/******************************************************************************/
#if USE_RTOS && defined(FSL_RTOS_FREE_RTOS) && defined(MBEDTLS_FREESCALE_FREERTOS_CALLOC_ALT)
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"

/*---------HEAP_3 calloc --------------------------------------------------*/

void *pvPortCalloc(size_t num, size_t size)
{
    void *pvReturn;

    vTaskSuspendAll();
    {
        pvReturn = calloc(num, size);
        traceMALLOC(pvReturn, xWantedSize);
    }
    (void)xTaskResumeAll();

#if (configUSE_MALLOC_FAILED_HOOK == 1)
    {
        if (pvReturn == NULL)
        {
            extern void vApplicationMallocFailedHook(void);
            vApplicationMallocFailedHook();
        }
    }
#endif

    return pvReturn;
}
#endif /* USE_RTOS && defined(FSL_RTOS_FREE_RTOS) && defined(MBEDTLS_FREESCALE_FREERTOS_CALLOC_ALT) */
