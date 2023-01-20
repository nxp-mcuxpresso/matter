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

// ================================================================================
// Main Code
// ================================================================================

#include <lib/core/CHIPError.h>
#include <platform/CHIPDeviceLayer.h>
#include <lib/support/CHIPMem.h>
#include <lib/support/CHIPPlatformMemory.h>
#include <lib/support/logging/CHIPLogging.h>
#include "CHIPDeviceManager.h"
#include "DeviceCallbacks.h"

#if TCP_DOWNLOAD
#include "TcpDownload.h"
#endif

using namespace ::chip;
using namespace ::chip::Inet;
using namespace ::chip::DeviceLayer;
using namespace ::chip::DeviceManager;

#include <AppTask.h>

#if ENABLE_OTW_K32W0
#include "ISPInterface.h"
#endif

#if configAPPLICATION_ALLOCATED_HEAP
uint8_t __attribute__((section(".heap"))) ucHeap[configTOTAL_HEAP_SIZE];
#endif

#ifndef MAIN_TASK_SIZE
#define MAIN_TASK_SIZE ((configSTACK_DEPTH_TYPE)4096 / sizeof(portSTACK_TYPE))
#endif

#if CONFIG_CHIP_PLAT_LOAD_REAL_FACTORY_DATA
#include "FactoryDataProvider.h"
#include "DataReaderEncryptedDCP.h"
/*
* Test key used to encrypt factory data before storing it to the flash.
* The software key should be used only during development stage.
* For production usage, it is recommended to use the OTP key which needs to be fused in the RT1060 SW_GP2.
*/
static const uint8_t aes128TestKey[] __attribute__((aligned)) = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                                                0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
#endif

static DeviceCallbacks deviceCallbacks;

void main_task(void *pvParameters)
{
    CHIP_ERROR ret;

    ChipLogProgress(DeviceLayer, "Welcome to NXP All Clusters Demo App");

    // Init Chip memory management before the stack
    chip::Platform::MemoryInit();

#if ENABLE_OTW_K32W0
    if (RT::ISPInterface::getInstance().runOTWUpdater() != E_PRG_OK)
    {
        ChipLogError(DeviceLayer, "OTW Updater failed!");
        goto exit;
    }
#endif

#if CONFIG_CHIP_PLAT_LOAD_REAL_FACTORY_DATA
    /*
    * Load factory data from the flash to the RAM.
    * Needs to be done before starting other Matter modules to avoid concurrent access issues with DCP hardware module.
    */
    DataReaderEncryptedDCP *dataReaderEncryptedDCPInstance = nullptr;
    dataReaderEncryptedDCPInstance = &DataReaderEncryptedDCP::GetDefaultInstance();
    /*
    * This example demonstrates the usage of the ecb with a software key, to use other encryption mode,
    * or to use hardware keys, check available methodes from the DataReaderEncryptedDCP class.
    */
    dataReaderEncryptedDCPInstance->SetEncryptionMode(encrypt_ecb);
    dataReaderEncryptedDCPInstance->SetAes128Key(&aes128TestKey[0]);

    ret = FactoryDataProvider::GetDefaultInstance().Init(dataReaderEncryptedDCPInstance);
    if (ret != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Factory Data Provider init failed");
        goto exit;
    }
#endif

    // Initialize the CHIP stack.
    ret = PlatformMgr().InitChipStack();
    if (ret != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "PlatformMgr().InitChipStack() failed: %s", ErrorStr(ret));
        goto exit;
    }

    ret = CHIPDeviceManager::GetInstance().Init(&deviceCallbacks);
    if (ret != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "CHIPDeviceManager.Init() failed: %s", ErrorStr(ret));
        goto exit;
    }

#if CHIP_DEVICE_CONFIG_ENABLE_WPA
    ConnectivityMgrImpl().StartWiFiManagement();
#endif

#if CHIP_ENABLE_OPENTHREAD
    ret = ThreadStackMgr().InitThreadStack();
    if (ret != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Error during ThreadStackMgr().InitThreadStack()");
        goto exit;
    }

    ret = ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_Router);
    if (ret != CHIP_NO_ERROR)
    {
        goto exit;
    }
#endif

    // Start a task to run the CHIP Device event loop.
    ret = PlatformMgr().StartEventLoopTask();
    if (ret != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Error during PlatformMgr().StartEventLoopTask()");
        goto exit;
    }

#if CHIP_ENABLE_OPENTHREAD
    // Start OpenThread task
    ret = ThreadStackMgrImpl().StartThreadTask();
    if (ret != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Error during ThreadStackMgrImpl().StartThreadTask()");
        goto exit;
    }
#endif

    ret = GetAppTask().StartAppTask();
    if (ret != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Error during GetAppTask().StartAppTask()");
        goto exit;
    }

#if TCP_DOWNLOAD
    EnableTcpDownloadComponent();
#endif

exit:
    vTaskDelete(NULL);
}

extern "C" int main(int argc, char * argv[])
{
    TaskHandle_t taskHandle;

    PlatformMgrImpl().HardwareInit();

    if (xTaskCreate(&main_task, "main_task", MAIN_TASK_SIZE, NULL, configMAX_PRIORITIES - 4, &taskHandle) != pdPASS)
    {
        ChipLogError(DeviceLayer, "Failed to start main task");
        assert(false);
    }

    ChipLogProgress(DeviceLayer, "Starting FreeRTOS scheduler");
    vTaskStartScheduler();
}

bool lowPowerClusterSleep()
{
    return true;
}

#if (defined(configCHECK_FOR_STACK_OVERFLOW) && (configCHECK_FOR_STACK_OVERFLOW > 0))
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    assert(0);
}
#endif
