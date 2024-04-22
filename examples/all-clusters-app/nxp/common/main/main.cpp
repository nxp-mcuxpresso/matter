/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2021-2023 Google LLC.
 *    Copyright 2024 NXP
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

// ================================================================================
// Main Code
// ================================================================================

#include "FreeRTOS.h"
#include "task.h"
#include <AppTask.h>
#include <lib/core/CHIPError.h>
#include <lib/support/logging/CHIPLogging.h>
#include <app/util/binding-table.h>
#include "binding_table.h"
#include "demo_actions.h"
#include "binding-handler.h"
#include "board.h"

#if configAPPLICATION_ALLOCATED_HEAP
uint8_t __attribute__((section(".heap"))) ucHeap[configTOTAL_HEAP_SIZE];
#endif

#define QUEUE_SIZE 10
#define STRUCT_ELEMENTS_SIZE_1 2
#define STRUCT_ELEMENTS_SIZE_2 3

using namespace ::chip::DeviceLayer;

extern bindingStruct *arrayBindingStructs;
extern int bindingEntriesNumber;

QueueHandle_t xMatterActionsQueue;

void vMatterActionsTask(void * pvParameters)
{
    matterActionStruct matterAction;

    while(1)
    {
        if (xMatterActionsQueue != NULL)
        {
            if (xQueueReceive(xMatterActionsQueue, &matterAction, portMAX_DELAY) == pdPASS)
            {
                switch(matterAction.action)
                {
                case kMatterActionColorChange:
                    chip::NXP::App::GetAppTask().ColorChangeFunction((char *)matterAction.value, matterAction.location);
                    break;
                case kMatterActionOnOff:
                    chip::NXP::App::GetAppTask().OnOffCommandFunction(matterAction.command, matterAction.location);
                    break;
                case kMatterActionBrightnessChange:
                    chip::NXP::App::GetAppTask().BrightnessChangeFunction(matterAction.command, (int)matterAction.value, matterAction.location);
                    break;
                case kMatterActionBlindsControl:
                    chip::NXP::App::GetAppTask().BlindsControlFunction(matterAction.command, (int)matterAction.value, matterAction.location);
                    break;
                }
            }
        }
    }
}

extern "C" int main(int argc, char * argv[])
{
#if RELOCATE_VECTOR_TABLE
        BOARD_RelocateVectorTableToRam();
#endif
    TaskHandle_t taskHandle;

    xMatterActionsQueue = xQueueCreate(1, sizeof(matterActionStruct));
    xTaskCreate(vMatterActionsTask, "vMatterActionsTask", 1024, NULL, 2, NULL);

    PlatformMgrImpl().HardwareInit();

    binding_table_flash_get(getArrayBindingStructsAdr(), &bindingEntriesNumber);

    chip::NXP::App::GetAppTask().Start();
    vTaskStartScheduler();
}
