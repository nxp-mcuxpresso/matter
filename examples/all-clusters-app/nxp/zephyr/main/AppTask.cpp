/*
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

#include "AppTask.h"
// #include "AppConfig.h"
#include "AppEvent.h"
#include "binding-handler.h"
#include "CHIPDeviceManager.h"
#include "DeviceCallbacks.h"

#include <DeviceInfoProviderImpl.h>

#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>

#include <app/clusters/identify-server/identify-server.h>
#include <app/clusters/ota-requestor/OTATestEventTriggerDelegate.h>
#include <app/util/attribute-storage.h>

#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>

#if CONFIG_CHIP_APP_DEVICE_TYPE_ALL_CLUSTERS
#include <static-supported-temperature-levels.h>
#endif

#ifdef CONFIG_CHIP_WIFI
#include <app/clusters/network-commissioning/network-commissioning.h>
#include <platform/nxp/zephyr/wifi/NxpWifiDriver.h>
#endif

#if CONFIG_CHIP_FACTORY_DATA
#include <platform/nxp/common/factory_data/FactoryDataProvider.h>
#else
#include <platform/nxp/zephyr/DeviceInstanceInfoProviderImpl.h>
#endif

// #if CONFIG_CHIP_OTA_REQUESTOR
// #include "OTAUtil.h"
// #endif

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(app, CONFIG_CHIP_APP_LOG_LEVEL);

using namespace ::chip;
using namespace ::chip::app;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;
using namespace ::chip::DeviceManager;

namespace {
constexpr size_t kAppEventQueueSize                         = 10;
constexpr EndpointId kNetworkCommissioningEndpointSecondary = 0xFFFE;

// NOTE! This key is for test/certification only and should not be available in production devices!
// If CONFIG_CHIP_FACTORY_DATA is enabled, this value is read from the factory data.
uint8_t sTestEventTriggerEnableKey[TestEventTriggerDelegate::kEnableKeyLength] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                                                                   0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

#if CONFIG_CHIP_FACTORY_DATA && CONFIG_CHIP_ENCRYPTED_FACTORY_DATA

#ifdef CONFIG_CHIP_ENCRYPTED_FACTORY_DATA_AES128_KEY

#define KEY CONFIG_CHIP_ENCRYPTED_FACTORY_DATA_AES128_KEY
#define HEXTONIBBLE(c) (*(c) >= 'A' ? (*(c) - 'A')+10 : (*(c)-'0'))
#define HEXTOBYTE(c) (HEXTONIBBLE(c)*16 + HEXTONIBBLE(c+1))

#define AES128_KEY_ARRAY    HEXTOBYTE(KEY+0),  HEXTOBYTE(KEY+2),  HEXTOBYTE(KEY+4),  HEXTOBYTE(KEY+6),  \
                            HEXTOBYTE(KEY+8),  HEXTOBYTE(KEY+10), HEXTOBYTE(KEY+12), HEXTOBYTE(KEY+14), \
                            HEXTOBYTE(KEY+16), HEXTOBYTE(KEY+18), HEXTOBYTE(KEY+20), HEXTOBYTE(KEY+22), \
                            HEXTOBYTE(KEY+24), HEXTOBYTE(KEY+26), HEXTOBYTE(KEY+28), HEXTOBYTE(KEY+30)
#else

#define AES128_KEY_ARRAY    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, \
                            0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
#endif /* CONFIG_CHIP_ENCRYPTED_FACTORY_DATA_AES128_KEY */

/*
* Test key used to encrypt factory data before storing it to the flash.
*/
static const uint8_t aes128TestKey[] __attribute__((aligned)) = { AES128_KEY_ARRAY };

#endif /* CONFIG_CHIP_FACTORY_DATA && CONFIG_CHIP_ENCRYPTED_FACTORY_DATA */

K_MSGQ_DEFINE(sAppEventQueue, sizeof(AppEvent), kAppEventQueueSize, alignof(AppEvent));

chip::DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;

#if CONFIG_CHIP_APP_DEVICE_TYPE_ALL_CLUSTERS
app::Clusters::TemperatureControl::AppSupportedTemperatureLevelsDelegate sAppSupportedTemperatureLevelsDelegate;
#endif

} // namespace

#ifdef CONFIG_CHIP_WIFI
app::Clusters::NetworkCommissioning::Instance sWiFiCommissioningInstance(0, &(NetworkCommissioning::NxpWifiDriver::Instance()));
#endif

CHIP_ERROR AppTask::Init()
{
    // Initialize CHIP stack
    LOG_INF("Init CHIP stack");

    CHIP_ERROR err = chip::Platform::MemoryInit();
    if (err != CHIP_NO_ERROR)
    {
        LOG_ERR("Platform::MemoryInit() failed");
        return err;
    }

    err = PlatformMgr().InitChipStack();
    if (err != CHIP_NO_ERROR)
    {
        LOG_ERR("PlatformMgr().InitChipStack() failed");
        return err;
    }

    /*
    * Register all application callbacks allowing to be informed of stack events
    */
    err = CHIPDeviceManager::GetInstance().Init(&deviceCallbacks);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "CHIPDeviceManager.Init() failed: %s", ErrorStr(err));
        return err;
    }

#if defined(CONFIG_CHIP_WIFI)
    sWiFiCommissioningInstance.Init();
#else
    return CHIP_ERROR_INTERNAL;
#endif // CONFIG_CHIP_WIFI

    err = InitBindingHandlers();
    if (err != CHIP_NO_ERROR)
    {
        LOG_ERR("InitBindingHandlers() failed");
    }

#if CONFIG_CHIP_FACTORY_DATA
#if CONFIG_CHIP_ENCRYPTED_FACTORY_DATA
    FactoryDataPrvdImpl().SetEncryptionMode(FactoryDataProvider::encrypt_ecb);
    FactoryDataPrvdImpl().SetAes128Key(&aes128TestKey[0]);
#endif /* CONFIG_CHIP_ENCRYPTED_FACTORY_DATA */
    ReturnErrorOnFailure(FactoryDataPrvdImpl().Init());
    if (err == CHIP_NO_ERROR)
    {
        SetDeviceInstanceInfoProvider(&FactoryDataPrvd());
        SetDeviceAttestationCredentialsProvider(&FactoryDataPrvd());
        SetCommissionableDataProvider(&FactoryDataPrvd());
    }
#else
    SetDeviceInstanceInfoProvider(&DeviceInstanceInfoProviderMgrImpl());
    SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
#endif /* CONFIG_CHIP_FACTORY_DATA */

    // Initialize CHIP server
    static CommonCaseDeviceServerInitParams initParams;
    static OTATestEventTriggerDelegate testEventTriggerDelegate{ ByteSpan(sTestEventTriggerEnableKey) };
    (void) initParams.InitializeStaticResourcesBeforeServerInit();
    initParams.testEventTriggerDelegate = &testEventTriggerDelegate;
    ReturnErrorOnFailure(chip::Server::GetInstance().Init(initParams));

    gExampleDeviceInfoProvider.SetStorageDelegate(&Server::GetInstance().GetPersistentStorage());
    chip::DeviceLayer::SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

    // We only have network commissioning on endpoint 0.
    emberAfEndpointEnableDisable(kNetworkCommissioningEndpointSecondary, false);
    ConfigurationMgr().LogDeviceConfig();
    PrintOnboardingCodes(chip::RendezvousInformationFlag(chip::RendezvousInformationFlag::kBLE));

    // Start CHIP thread.
    // Note that all the initialization code should happen prior to this point to avoid data races
    // between the main and the CHIP threads
    err = PlatformMgr().StartEventLoopTask();
    if (err != CHIP_NO_ERROR)
    {
        LOG_ERR("PlatformMgr().StartEventLoopTask() failed");
    }

#if CONFIG_CHIP_APP_DEVICE_TYPE_ALL_CLUSTERS
    app::Clusters::TemperatureControl::SetInstance(&sAppSupportedTemperatureLevelsDelegate);
#endif

    return err;
}

CHIP_ERROR AppTask::StartApp()
{
    ReturnErrorOnFailure(Init());

    AppEvent event{};

    ChipLogProgress(DeviceLayer, "Welcome to NXP All Clusters Demo App");

    while (true)
    {
        k_msgq_get(&sAppEventQueue, &event, K_FOREVER);
        DispatchEvent(event);
    }

    return CHIP_NO_ERROR;
}

void AppTask::PostEvent(const AppEvent & event)
{
    if (k_msgq_put(&sAppEventQueue, &event, K_NO_WAIT) != 0)
    {
        LOG_INF("Failed to post event to app task event queue");
    }
}

void AppTask::DispatchEvent(const AppEvent & event)
{
    if (event.Handler)
    {
        event.Handler(event);
    }
    else
    {
        LOG_INF("Event received with no handler. Dropping event.");
    }
}
