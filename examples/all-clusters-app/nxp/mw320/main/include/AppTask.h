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

// Device operation state
typedef enum
{
    frst_state=0,		// Factory Reset State, both uap & sta interfaces are on
    work_state,		// working state, UUT can connect to AP & uap is not needed anymore
    max_op_state,		// Max OpState
} op_state_t;

#define DEFAP_SSID		"nxp_matter"
#define DEFAP_PWD		"nxp12345"

class AppTask
{
public:
    static void AppTaskMain(void * pvParameter);
    op_state_t GetOpState(void);
    void SetOpState(op_state_t op_state);
    void SaveNetwork(char * ssid, char * pwd);
    void UapOnoff(bool do_on);

    struct wlan_network uap_network;
private:
    friend AppTask & GetAppTask(void);
    CHIP_ERROR Init();
    // Device operation state functions
    op_state_t ReadOpState(void);
    void WriteOpState(op_state_t opstat);
    int UapInit(void);
    void LoadNetwork(char * ssid, char * pwd);
#if (MW320_FEATURE_BIND == 1)
    static void TriggerBinding_all(void);
#endif //MW320_FEATURE_BIND

    static AppTask sAppTask;
    op_state_t	OpState;	// Device operation state
};

inline AppTask & GetAppTask(void)
{
    return AppTask::sAppTask;
}

inline op_state_t AppTask::GetOpState(void)
{
	return OpState;
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
