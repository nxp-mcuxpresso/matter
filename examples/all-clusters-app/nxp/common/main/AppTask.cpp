/*
 *
 *    Copyright (c) 2021-2022 Project CHIP Authors
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
#include "AppTask.h"
#include "AppEvent.h"
#include "lib/support/ErrorStr.h"
#include <app/server/Server.h>
#include <app/server/Dnssd.h>
#include <lib/dnssd/Advertiser.h>

#if CHIP_DEVICE_CONFIG_ENABLE_WPA
#include <platform/nxp/common/NetworkCommissioningDriver.h>
#endif // CHIP_DEVICE_CONFIG_ENABLE_WPA

#include <app/clusters/network-commissioning/network-commissioning.h>
#include <app/server/OnboardingCodesUtil.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/CommissionableDataProvider.h>
#include <platform/internal/DeviceNetworkInfo.h>
#include <platform/DeviceInstanceInfoProvider.h>
#include <lib/support/ThreadOperationalDataset.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>

#include <app-common/zap-generated/attribute-id.h>
#include <app-common/zap-generated/attribute-type.h>
#include <app-common/zap-generated/cluster-id.h>
#include <app/util/attribute-storage.h>

#include "application_config.h"

#include "AppMatterCli.h"
#include "AppMatterButton.h"

#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR
#include "OTARequestorInitiator.h"
#endif

#if CONFIG_CHIP_PLAT_LOAD_REAL_FACTORY_DATA
#include "FactoryDataProvider.h"
#endif

#if CHIP_ENABLE_OPENTHREAD
#include <inet/EndPointStateOpenThread.h>
#endif

#define FACTORY_RESET_TRIGGER_TIMEOUT 6000
#define FACTORY_RESET_CANCEL_WINDOW_TIMEOUT 3000
#define APP_TASK_STACK_SIZE (4096)
#define APP_TASK_PRIORITY 2
#define APP_EVENT_QUEUE_SIZE 10

static QueueHandle_t sAppEventQueue;

using namespace chip;
using namespace chip::TLV;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;
using namespace ::chip::app::Clusters;

#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR
OTARequestorInitiator gOTARequestorInitiator;
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_WPA
namespace {
chip::app::Clusters::NetworkCommissioning::Instance
    sWiFiNetworkCommissioningInstance(0 /* Endpoint Id */, &(::chip::DeviceLayer::NetworkCommissioning::NXPWiFiDriver::GetInstance()));
} // namespace

void NetWorkCommissioningInstInit()
{
    sWiFiNetworkCommissioningInstance.Init();
}
#endif // CHIP_DEVICE_CONFIG_ENABLE_WPA

AppTask AppTask::sAppTask;

CHIP_ERROR AppTask::StartAppTask()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    TaskHandle_t taskHandle;

    sAppEventQueue = xQueueCreate(APP_EVENT_QUEUE_SIZE, sizeof(AppEvent));
    if (sAppEventQueue == NULL)
    {
        err = CHIP_ERROR_NO_MEMORY;
        ChipLogError(DeviceLayer, "Failed to allocate app event queue");
        assert(err == CHIP_NO_ERROR);
    }

    if (xTaskCreate(&AppTask::AppTaskMain, "AppTaskMain", 1600,
                    &sAppTask, APP_TASK_PRIORITY, &taskHandle) != pdPASS)
    {
        err = CHIP_ERROR_NO_MEMORY;
        ChipLogError(DeviceLayer, "Failed to start app task");
        assert(err == CHIP_NO_ERROR);
    }

    return err;
}

#if CHIP_ENABLE_OPENTHREAD
void LockOpenThreadTask(void)
{
    chip::DeviceLayer::ThreadStackMgr().LockThreadStack();
}

void UnlockOpenThreadTask(void)
{
    chip::DeviceLayer::ThreadStackMgr().UnlockThreadStack();
}
#endif

void AppTask::InitServer(intptr_t arg)
{
    static chip::CommonCaseDeviceServerInitParams initParams;
    (void) initParams.InitializeStaticResourcesBeforeServerInit();

#if CHIP_ENABLE_OPENTHREAD
    // Init ZCL Data Model and start server
    chip::Inet::EndPointStateOpenThread::OpenThreadEndpointInitParam nativeParams;
    nativeParams.lockCb                = LockOpenThreadTask;
    nativeParams.unlockCb              = UnlockOpenThreadTask;
    nativeParams.openThreadInstancePtr = chip::DeviceLayer::ThreadStackMgrImpl().OTInstance();
    initParams.endpointNativeParams    = static_cast<void *>(&nativeParams);
#endif
    VerifyOrDie((chip::Server::GetInstance().Init(initParams)) == CHIP_NO_ERROR);

    // Disable last fixed endpoint, which is used as a placeholder for all of the
    // supported clusters so that ZAP will generated the requisite code.
    emberAfEndpointEnableDisable(emberAfEndpointFromIndex(static_cast<uint16_t>(emberAfFixedEndpointCount() - 1)), false);

#if ENABLE_OTA_PROVIDER
    InitOTAServer();
#endif

}

CHIP_ERROR AppTask::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    // Init ZCL Data Model and start server
    PlatformMgr().ScheduleWork(InitServer, 0);

#if CONFIG_CHIP_PLAT_LOAD_REAL_FACTORY_DATA
    FactoryDataProvider *factoryDataProvider = &FactoryDataProvider::GetDefaultInstance();
    SetDeviceInstanceInfoProvider(factoryDataProvider);
    SetDeviceAttestationCredentialsProvider(factoryDataProvider);
    SetCommissionableDataProvider(factoryDataProvider);
#else
    // Initialize device attestation when the example (only for debug purpose)
    SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_WPA
    NetWorkCommissioningInstInit();
#endif // CHIP_DEVICE_CONFIG_ENABLE_WPA

    uint16_t discriminator;
    err = GetCommissionableDataProvider()->GetSetupDiscriminator(discriminator);
    if (err != CHIP_NO_ERROR)
    {
        PRINTF("\r\nCouldn't get discriminator: %s", chip::ErrorStr(err));
    }
    PRINTF("\r\nSetup discriminator: %u (0x%x)", discriminator, discriminator);

    uint32_t setupPasscode=20202021;
    err = GetCommissionableDataProvider()->GetSetupPasscode(setupPasscode);
    if (err != CHIP_NO_ERROR)
    {
        PRINTF("\r\nCouldn't get setupPasscode: %s", chip::ErrorStr(err));
    }
    PRINTF("\r\nSetup passcode: %u (0x%x)", setupPasscode, setupPasscode);

    uint16_t vendorId;
    err = GetDeviceInstanceInfoProvider()->GetVendorId(vendorId);
    if (err != CHIP_NO_ERROR)
    {
        PRINTF("\r\nCouldn't get vendorId: %s", chip::ErrorStr(err));
    }
    PRINTF("\r\nVendor ID: %u (0x%x)", vendorId, vendorId);


    uint16_t productId;
    err = GetDeviceInstanceInfoProvider()->GetProductId(productId);
    if (err != CHIP_NO_ERROR)
    {
        PRINTF("\r\nCouldn't get productId: %s", chip::ErrorStr(err));
    }
    PRINTF("\r\nProduct ID: %u (0x%x)\r\n", productId, productId);
     
    // QR code will be used with CHIP Tool
    PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kOnNetwork));

    err = ClusterMgr().Init();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "ClusterMgr().Init() failed");
        assert(err == CHIP_NO_ERROR);
    }

    ClusterMgr().SetCallbacks(ActionInitiated, ActionCompleted);

    // Print the current software version
    char currentSoftwareVer[ConfigurationManager::kMaxSoftwareVersionStringLength + 1] = { 0 };
    err = ConfigurationMgr().GetSoftwareVersionString(currentSoftwareVer, sizeof(currentSoftwareVer));
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Get version error");
        assert(err == CHIP_NO_ERROR);
    }

    ChipLogProgress(DeviceLayer, "Current Software Version: %s", currentSoftwareVer);

#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR 
    /* If an update is under test make it permanent */
    gOTARequestorInitiator.HandleSelfTest();
    /* Initialize OTA Requestor */
    PlatformMgr().ScheduleWork(gOTARequestorInitiator.InitOTA, reinterpret_cast<intptr_t>(&gOTARequestorInitiator));
#endif

    /* Register Matter CLI cmds */
    err = AppMatterCli_RegisterCommands();
    assert(err == CHIP_NO_ERROR);
    /* Register Matter buttons */
    err = AppMatterButton_registerButtons();
    assert(err == CHIP_NO_ERROR);

    return err;
}

void AppTask::AppTaskMain(void * pvParameter)
{
    AppTask *task = (AppTask *)pvParameter;
    CHIP_ERROR err;
    AppEvent event;

    err = task->Init();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "AppTask.Init() failed");
        assert(err == CHIP_NO_ERROR);
    }

    while (true)
    {
        BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, portMAX_DELAY);
        while (eventReceived == pdTRUE)
        {
            sAppTask.DispatchEvent(&event);
            eventReceived = xQueueReceive(sAppEventQueue, &event, 0);
        }
    }
}

void AppTask::ActionInitiated(ClusterManager::Action_t aAction, int32_t aActor)
{
    // start flashing the LEDs rapidly to indicate action initiation.
    if (aAction == ClusterManager::TURNON_ACTION)
    {
        ChipLogProgress(DeviceLayer, "Turn on Action has been initiated");
    }
    else if (aAction == ClusterManager::TURNOFF_ACTION)
    {
        ChipLogProgress(DeviceLayer, "Turn off Action has been initiated");
    }

    sAppTask.mFunction = kFunctionTurnOnTurnOff;
}

void AppTask::ActionCompleted(ClusterManager::Action_t aAction)
{
    if (aAction == ClusterManager::TURNON_ACTION)
    {
        ChipLogProgress(DeviceLayer, "Turn on action has been completed");
    }
    else if (aAction == ClusterManager::TURNOFF_ACTION)
    {
        ChipLogProgress(DeviceLayer, "Turn off action has been completed");
    }

    sAppTask.mFunction = kFunction_NoneSelected;
}

void AppTask::PostEvent(const AppEvent * aEvent)
{
    if (sAppEventQueue != NULL)
    {
        if (!xQueueSend(sAppEventQueue, aEvent, 0))
        {
            ChipLogError(DeviceLayer, "Failed to post event to app task event queue");
        }
    }
}

void AppTask::DispatchEvent(AppEvent * aEvent)
{
    if (aEvent->Handler)
    {
        aEvent->Handler(aEvent);
    }
    else
    {
        ChipLogProgress(DeviceLayer, "Event received with no handler. Dropping event.");
    }
}

void AppTask::UpdateClusterState(void)
{
    uint8_t newValue = 0;//!ClusterMgr().IsTurnedOff();

    // write the new on/off value
    EmberAfStatus status = emberAfWriteAttribute(1, ZCL_ON_OFF_CLUSTER_ID, ZCL_ON_OFF_ATTRIBUTE_ID,
                                                 (uint8_t *) &newValue, ZCL_BOOLEAN_ATTRIBUTE_TYPE);
    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        ChipLogError(DeviceLayer, "ERR: updating on/off %x", status);
    }
}

void AppTask::StartCommissioning(intptr_t arg)
{
    /* Check the status of the commissioning */
    if (ConfigurationMgr().IsFullyProvisioned())
    {
        ChipLogProgress(DeviceLayer, "Device already commissioned");
    }
    else if (chip::Server::GetInstance().GetCommissioningWindowManager().IsCommissioningWindowOpen())
    {
         ChipLogProgress(DeviceLayer, "Commissioning window already opened");
    }
    else
    {
        chip::Server::GetInstance().GetCommissioningWindowManager().OpenBasicCommissioningWindow();
    }
}

void AppTask::StopCommissioning(intptr_t arg)
{
    /* Check the status of the commissioning */
    if (ConfigurationMgr().IsFullyProvisioned())
    {
        ChipLogProgress(DeviceLayer, "Device already commissioned");
    }
    else if (!chip::Server::GetInstance().GetCommissioningWindowManager().IsCommissioningWindowOpen())
    {
         ChipLogProgress(DeviceLayer, "Commissioning window not opened");
    }
    else
    {
        chip::Server::GetInstance().GetCommissioningWindowManager().CloseCommissioningWindow();
    }
}

void AppTask::SwitchCommissioningState(intptr_t arg)
{
     /* Check the status of the commissioning */
    if (ConfigurationMgr().IsFullyProvisioned())
    {
        ChipLogProgress(DeviceLayer, "Device already commissioned");
    }
    else if (!chip::Server::GetInstance().GetCommissioningWindowManager().IsCommissioningWindowOpen())
    {
        chip::Server::GetInstance().GetCommissioningWindowManager().OpenBasicCommissioningWindow();
    }
    else
    {
        chip::Server::GetInstance().GetCommissioningWindowManager().CloseCommissioningWindow();
    }
}

void AppTask::FactoryReset(intptr_t arg)
{
    // Actually trigger Factory Reset
    ConfigurationMgr().InitiateFactoryReset();
}

void AppTask::StartCommissioningHandler(void)
{
    /* Publish an event to the Matter task to always set the commissioning state in the Matter task context */
    PlatformMgr().ScheduleWork(StartCommissioning, 0);
}

void AppTask::StopCommissioningHandler(void)
{
    /* Publish an event to the Matter task to always set the commissioning state in the Matter task context */
    PlatformMgr().ScheduleWork(StopCommissioning, 0);
}

void AppTask::SwitchCommissioningStateHandler(void)
{
    /* Publish an event to the Matter task to always set the commissioning state in the Matter task context */
    PlatformMgr().ScheduleWork(SwitchCommissioningState, 0);
}

void AppTask::FactoryResetHandler(void)
{
    /* Publish an event to the Matter task to trigger the factory reset in the Matter task context */
    PlatformMgr().ScheduleWork(FactoryReset, 0);
}