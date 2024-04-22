/*
 * Copyright 2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OSA_FREERTOS_H
#define OSA_FREERTOS_H

/*!
 * @file    osa_freertos.h
 * @brief   Header includes specific to FreeRTOS.
 * @details This file includes FreeRTOS-specifc system headers for behavior such as
 *          standard I/O.
 */

#include <stdio.h>

#include <FreeRTOS.h>

#include "list.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

typedef TaskFunction_t OsaThread;
typedef SemaphoreHandle_t OsaSem;
typedef SemaphoreHandle_t OsaMutex;

typedef struct _OsaCondition {
    void *event_wait;
    void *event_signal;
    EventGroupHandle_t index;
} OsaCondition;

#define OSA_FREERTOS_MSG_POOL_NAME_MAX 32

typedef struct _OsaMq {
    QueueHandle_t queue_id;
    char name[OSA_FREERTOS_MSG_POOL_NAME_MAX];
} OsaMq;

typedef void (*osa_event_cb)(void*);
typedef struct _OsaEvent {
    TimerHandle_t timer_id;
    osa_event_cb cb;
    void *arg;
} OsaEvent;

#define PATHNAME_SIZE 256

#endif  /* OSA_FREERTOS_H */

