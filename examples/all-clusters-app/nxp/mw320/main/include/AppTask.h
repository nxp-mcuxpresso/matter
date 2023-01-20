/*
 *
 *    Copyright (c) 2021 Google LLC.
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

#include <stdbool.h>
#include <stdint.h>
#include <platform/CHIPDeviceLayer.h>
#include <app/clusters/identify-server/identify-server.h>

extern "C" {
#include "wlan.h"
}

class AppTask
{
public:
//    CHIP_ERROR StartAppTask();
//    static void AppTaskMain(void * pvParameter);
private:
    friend AppTask & GetAppTask(void);

    CHIP_ERROR Init();
    static AppTask sAppTask;
};

inline AppTask & GetAppTask(void)
{
    return AppTask::sAppTask;
}

typedef enum
{
    led_yellow,
    led_amber,
    led_max
} led_id_t;

extern volatile int g_ButtonPress;
extern bool need2sync_sw_attr;

void led_on_off(led_id_t lt_id, bool is_on);
void sw2_handle(bool frm_clk);
int init_mw320_sdk(int (*cb)(enum wlan_event_reason reason, void * data));
void board_init(void);
