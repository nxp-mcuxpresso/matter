/*
 *
 *    Copyright (c) 2023 Project CHIP Authors
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

#include <pthread.h>
#include <stdbool.h>

/*
*  ops:
*  0x00  unknow
*  0x01  network started
*  0x02  add zcb node
*  0x03  remove zcb node
*/
typedef enum {
    BRIDGE_UNKNOW   = 0,
    BRIDGE_ADD_DEV,
    BRIDGE_REMOVE_DEV,
    BRIDGE_FACTORY_RESET,
    BRIDGE_WRITE_ATTRIBUTE,
    BRIDGE_LEAVE,
} msgType;

typedef struct {
    uint16_t u16ClusterID;
    uint16_t u16AttributeID;
    uint64_t u64Data;
} ZcbAttribute_t;

typedef struct {
    bool AnnounceStart;
    bool HandleMask;
    pthread_mutex_t bridge_mutex;
    pthread_cond_t  bridge_cond;
    int msg_type;
    newdb_zcb_t zcb;
    void *msg_data;
} ZcbMsg_t;

extern ZcbMsg_t ZcbMsg;
