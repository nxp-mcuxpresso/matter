/*
 *    Copyright (c) 2022 Project CHIP Authors
 *    Copyright (c) 2022 NXP
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

#pragma once

#define SECLIB_AES_ECB_ALT /* Enable alternative implementation for SecLib AES ECB. */

#define SECLIB_SHA256_ALT  /* Enable alternative implementation for SecLib SHA256. */

#if defined(SECLIB_SHA256_ALT)
/* Arbitrary value in milliseconds. If hardware SHA256 is currently in use,
 * subsequent SHA256_Init calls from other tasks will wait for SECLIB_SHA256_MUTEX_TIMEOUT
 * in order to lock the hardware SHA256 mutex. If a task fails to lock the mutex in this 
 * interval, it will use SW SHA256 instead. Set it to 0 for polling.
 */
#define SECLIB_SHA256_MUTEX_TIMEOUT 0

#ifndef SHA256_HASH_SIZE
#define SHA256_HASH_SIZE   32 /* [bytes] */
#endif

#ifndef SHA256_BLOCK_SIZE
#define SHA256_BLOCK_SIZE  64 /* [bytes] */
#endif

/* Common SHA256 context struct between sw/hw SHA256. */
typedef struct sha256Context_tag{
    uint32_t hash[SHA256_HASH_SIZE/sizeof(uint32_t)];
    uint8_t  buffer[SHA256_BLOCK_SIZE];
    uint32_t totalBytes;
    uint8_t  bytes;
}sha256Context_t;

/* SHA256 wrappers over SW emulated functions. */
void sw_sha256_init_wrap(void* pContext);
void sw_sha256_update_wrap(void* pContext, const uint8_t* pData, uint32_t numBytes);
void sw_sha256_finish_wrap(void* pContext, uint8_t* pOutput);

#endif
