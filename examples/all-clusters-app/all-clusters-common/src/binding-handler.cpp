/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    Copyright 2024 NXP
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

#include "binding-handler.h"

#include <app-common/zap-generated/ids/Clusters.h>
#include <app-common/zap-generated/ids/Commands.h>
#include <app/CommandSender.h>
#include <app/clusters/bindings/BindingManager.h>
#include <app/server/Server.h>
#include <controller/InvokeInteraction.h>
#include <controller/ReadInteraction.h>
#include <controller/WriteInteraction.h>
#include <lib/core/CHIPError.h>
#include <platform/CHIPDeviceLayer.h>

#include <app/clusters/bindings/bindings.h>
#include <lib/support/CodeUtils.h>

#include "AppTaskBase.h"
#include "binding_table.h"
#include <app/util/binding-table.h>

#if defined(ENABLE_CHIP_SHELL)
#include <lib/shell/Engine.h> // nogncheck

using chip::Shell::Engine;
using chip::Shell::shell_command_t;
using chip::Shell::streamer_get;
using chip::Shell::streamer_printf;
#endif // defined(ENABLE_CHIP_SHELL)

#define MAX_LEVEL 99
#define MIN_LEVEL 2

bindingStruct* arrayBindingStructs = NULL;
int bindingEntriesNumber = 0;
extern uint8_t brightnessAction;

void freeArrayBindingStructs()
{
    if (arrayBindingStructs != NULL)
    {
        vPortFree(arrayBindingStructs);
        bindingEntriesNumber = 0;
        arrayBindingStructs = NULL;
    }
}

bindingStruct *getArrayBindingStructs()
{
    return arrayBindingStructs;
}

bindingStruct **getArrayBindingStructsAdr()
{
    return &arrayBindingStructs;
}

static bool sSwitchOnOffState = false;
#if defined(ENABLE_CHIP_SHELL)
static void ToggleSwitchOnOff(bool newState)
{
    sSwitchOnOffState = newState;
    chip::BindingManager::GetInstance().NotifyBoundClusterChanged(1, chip::app::Clusters::OnOff::Id, nullptr);
}

static CHIP_ERROR SwitchCommandHandler(int argc, char ** argv)
{
    if (argc == 1 && strcmp(argv[0], "on") == 0)
    {
        ToggleSwitchOnOff(true);
        return CHIP_NO_ERROR;
    }
    if (argc == 1 && strcmp(argv[0], "off") == 0)
    {
        ToggleSwitchOnOff(false);
        return CHIP_NO_ERROR;
    }
    streamer_printf(streamer_get(), "Usage: switch [on|off]");
    return CHIP_NO_ERROR;
}

static void RegisterSwitchCommands()
{
    static const shell_command_t sSwitchCommand = { SwitchCommandHandler, "switch", "Switch commands. Usage: switch [on|off]" };
    Engine::Root().RegisterCommands(&sSwitchCommand, 1);
    return;
}
#endif // defined(ENABLE_CHIP_SHELL)

static void BoundDeviceChangedHandler(const EmberBindingTableEntry & binding, chip::OperationalDeviceProxy * peer_device,
                                      void * context)
{
    using namespace chip;
    using namespace chip::app;

    if (binding.type == MATTER_MULTICAST_BINDING)
    {
        ChipLogError(NotSpecified, "Group binding is not supported now");
        return;
    }

    if(static_cast<bindingFunction_t>(reinterpret_cast<intptr_t>(context)) == kBindingFunctionBindDevice)
    {
        if (binding.type == MATTER_UNICAST_BINDING && binding.local == 1)
        {
            int currentPeer = peer_device->GetDeviceId();
            GetDeviceInfo(peer_device, currentPeer);
        }
        return;
    }

    BindingCommandData * data = static_cast<BindingCommandData *>(context);
    const EmberBindingTableEntry & entry = BindingTable::GetInstance().GetAt(data->bindingTableEntryId);

    if(entry.nodeId == peer_device->GetDeviceId())
    {
        if (binding.type == MATTER_UNICAST_BINDING && binding.local == 1)
        {
            switch (data->clusterId)
            {
                case Clusters::OnOff::Id:
                    ProcessOnOffUnicastBindingCommand(BindingTable::GetInstance().GetAt(data->bindingTableEntryId),
                        peer_device, data);
                    break;
                case Clusters::ColorControl::Id:
                    ProcessColorControlUnicastBindingCommand(BindingTable::GetInstance().GetAt(data->bindingTableEntryId),
                        peer_device, data);
                    break;
                case Clusters::LevelControl::Id:
                    ProcessLevelControlUnicastBindingCommand(BindingTable::GetInstance().GetAt(data->bindingTableEntryId),
                        peer_device, data);
                    break;
                case Clusters::WindowCovering::Id:
                    ProcessWindowCoveringUnicastBindingCommand(BindingTable::GetInstance().GetAt(data->bindingTableEntryId),
                        peer_device, data);
                    break;
                default:
                    break;
            }
        }
    }
}

void ProcessOnOffUnicastBindingCommand(const EmberBindingTableEntry & binding, chip::OperationalDeviceProxy * peer_device, BindingCommandData *data)
{
    auto onSuccess = [data](const chip::app::ConcreteCommandPath & commandPath, const chip::app::StatusIB & status, const auto & dataResponse) {
         ChipLogProgress(NotSpecified, "OnOff command succeeds");
         delete data;
    };
    auto onFailure = [data](CHIP_ERROR error) {
         ChipLogError(NotSpecified, "OnOff command failed.");
         delete data;
    };

    if (data->commandId == chip::app::Clusters::OnOff::Commands::On::Id)
    {
        chip::app::Clusters::OnOff::Commands::On::Type onCommand;
        chip::Controller::InvokeCommandRequest(peer_device->GetExchangeManager(), peer_device->GetSecureSession().Value(),
                data->remoteEndpointId, onCommand, onSuccess, onFailure);
    }
    else if (data->commandId == chip::app::Clusters::OnOff::Commands::Off::Id)
    {
        chip::app::Clusters::OnOff::Commands::Off::Type offCommand;
        chip::Controller::InvokeCommandRequest(peer_device->GetExchangeManager(), peer_device->GetSecureSession().Value(),
                data->remoteEndpointId, offCommand, onSuccess, onFailure);
    }
    else if (data->commandId == chip::app::Clusters::OnOff::Commands::Toggle::Id)
    {
        chip::app::Clusters::OnOff::Commands::Toggle::Type toggleCommand;
        chip::Controller::InvokeCommandRequest(peer_device->GetExchangeManager(), peer_device->GetSecureSession().Value(),
                data->remoteEndpointId, toggleCommand, onSuccess, onFailure);
    }
}

void ProcessColorControlUnicastBindingCommand(const EmberBindingTableEntry & binding, chip::DeviceProxy * peer_device, BindingCommandData *data)
{
    auto onSuccess = [data](const chip::app::ConcreteCommandPath & commandPath, const chip::app::StatusIB & status, const auto & dataResponse) {
        ChipLogProgress(NotSpecified, "MoveToHue command succeeds");
        delete (bindingHueSaturationData *)data->value;
        delete data;
    };
    auto onFailure = [data](CHIP_ERROR error) {
        ChipLogError(NotSpecified, "MoveToHue command failed.");
        delete (bindingHueSaturationData *)data->value;
        delete data;
    };

    chip::app::Clusters::ColorControl::Commands::MoveToHueAndSaturation::Type moveToHueAndSaturationCommand;

    moveToHueAndSaturationCommand.hue             = ((bindingHueSaturationData *)data->value)->hue;
    moveToHueAndSaturationCommand.saturation      = ((bindingHueSaturationData *)data->value)->saturation;
    moveToHueAndSaturationCommand.transitionTime  = 0;
    moveToHueAndSaturationCommand.optionsMask     = 0x00;
    moveToHueAndSaturationCommand.optionsOverride = 0x00;

    chip::Controller::InvokeCommandRequest(peer_device->GetExchangeManager(), peer_device->GetSecureSession().Value(), data->remoteEndpointId,
            moveToHueAndSaturationCommand, onSuccess, onFailure);
}

void ReadAndModifyBrightnessLevel(const EmberBindingTableEntry & binding, chip::DeviceProxy * peer_device, BindingCommandData *data)
{
    using namespace chip;
    using namespace chip::app;

    auto readSuccess = [data](const chip::app::ConcreteDataAttributePath & attributePath, const auto & dataResponse) {
       ChipLogProgress(DeviceLayer, "Reading level attribute");

       uint8_t newLevel;
       DataModel::Nullable<uint8_t> currLevel = dataResponse;

       if (((bindingBrightnessData *)data->value)->action == kBindingBrightnessActionIncrease)
       {
           if (currLevel.Value() + ((bindingBrightnessData *)data->value)->level >= MAX_LEVEL)
           {
               newLevel = MAX_LEVEL;
           }
           else
           {
               newLevel = currLevel.Value() + ((bindingBrightnessData *)data->value)->level;
           }
       }
       else if (((bindingBrightnessData *)data->value)->action == kBindingBrightnessActionDecrease)
       {
           if (currLevel.Value() - ((bindingBrightnessData *)data->value)->level <= MIN_LEVEL)
           {
               newLevel = MIN_LEVEL;
           }
           else
           {
               newLevel = currLevel.Value() - ((bindingBrightnessData *)data->value)->level;
           }
       }

       BindingCommandData * newLevelData     = chip::Platform::New<BindingCommandData>();
       newLevelData->bindingTableEntryId     = data->bindingTableEntryId;
       newLevelData->clusterId               = chip::app::Clusters::LevelControl::Id;
       newLevelData->commandId               = chip::app::Clusters::LevelControl::Commands::MoveToLevel::Id;
       bindingBrightnessData *brightnessData = chip::Platform::New<bindingBrightnessData>();
       brightnessData->action                = kBindingBrightnessActionSet;
       brightnessData->level                 = newLevel;
       newLevelData->value                   = (void *) brightnessData;

       delete (bindingBrightnessData *)data->value;
       delete data;

       chip::DeviceLayer::PlatformMgr().ScheduleWork(ControllerWorkerFunction, reinterpret_cast<intptr_t>(newLevelData));
    };

    auto readFailure = [data](const chip::app::ConcreteDataAttributePath * attributePath, CHIP_ERROR aError) {
        ChipLogError(DeviceLayer, "Could not read level attribute, error: %s",  chip::ErrorStr(aError));
        delete (bindingBrightnessData *)data->value;
        delete data;
    };

    chip::Controller::ReadAttribute<chip::app::Clusters::LevelControl::Attributes::CurrentLevel::TypeInfo::DecodableType>(peer_device->GetExchangeManager(),
                                            peer_device->GetSecureSession().Value(),
                                            data->remoteEndpointId,
                                            chip::app::Clusters::LevelControl::Id,
                                            chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Id,
                                            readSuccess, readFailure);
}

void ProcessLevelControlUnicastBindingCommand(const EmberBindingTableEntry & binding, chip::DeviceProxy * peer_device, BindingCommandData *data)
{
    auto onSuccess = [data](const chip::app::ConcreteCommandPath & commandPath, const chip::app::StatusIB & status, const auto & dataResponse) {
        ChipLogProgress(NotSpecified, "MoveToLevel command succeeds");
        delete (bindingBrightnessData *)data->value;
        delete data;
    };
    auto onFailure = [data](CHIP_ERROR error) {
        ChipLogError(NotSpecified, "MoveToLevel command failed.");
        delete (bindingBrightnessData *)data->value;
        delete data;
    };

    if (((bindingBrightnessData *)data->value)->action == kBindingBrightnessActionSet)
    {
        chip::app::Clusters::LevelControl::Commands::MoveToLevel::Type moveToLevelCommand;

        moveToLevelCommand.level = ((bindingBrightnessData *)data->value)->level;

        chip::Controller::InvokeCommandRequest(peer_device->GetExchangeManager(), peer_device->GetSecureSession().Value(), data->remoteEndpointId,
                moveToLevelCommand, onSuccess, onFailure);
    }
    else
    {
        ReadAndModifyBrightnessLevel(binding, peer_device, data);
    }
}

void ProcessWindowCoveringUnicastBindingCommand(const EmberBindingTableEntry & binding, chip::DeviceProxy * peer_device, BindingCommandData *data)
{
    auto onSuccess = [data](const chip::app::ConcreteCommandPath & commandPath, const chip::app::StatusIB & status, const auto & dataResponse) {
        ChipLogProgress(NotSpecified, "MoveToPercentage command succeeds");
        delete data;
    };
    auto onFailure = [data](CHIP_ERROR error) {
        ChipLogError(NotSpecified, "MoveToPercentage command failed.");
        delete data;
    };

    chip::app::Clusters::WindowCovering::Commands::GoToLiftPercentage::Type goToLiftPercentageCommand;

    goToLiftPercentageCommand.liftPercent100thsValue = (int)data->value;

    chip::Controller::InvokeCommandRequest(peer_device->GetExchangeManager(), peer_device->GetSecureSession().Value(), data->remoteEndpointId,
            goToLiftPercentageCommand, onSuccess, onFailure);

}

void GetDeviceInfo(chip::DeviceProxy * peer_device, int currentPeerId)
{
    using namespace chip;
    using namespace chip::app;

    auto readSuccess = [currentPeerId](const chip::app::ConcreteDataAttributePath & attributePath, const auto & dataResponse) {
       ChipLogProgress(DeviceLayer, "Getting device info");

       char *deviceName = (char *)pvPortMalloc(32 * sizeof(char));

       int i = 0;

       for (const auto &entry : dataResponse) {
           deviceName[i] = static_cast<char>(entry);
           i++;
       }

       deviceName[i] = '\0';

       bindingStruct *newArrayBindingStructs = (bindingStruct *)pvPortMalloc((bindingEntriesNumber + 1) * sizeof(bindingStruct));
       if (newArrayBindingStructs != NULL)
       {
           for (int i = 0; i < bindingEntriesNumber; i++)
           {
               memcpy(&newArrayBindingStructs[i], &arrayBindingStructs[i], sizeof(bindingStruct));
           }

           for (auto iter = BindingTable::GetInstance().begin(); iter != BindingTable::GetInstance().end(); ++iter)
           {
               if (iter->nodeId == currentPeerId)
               {
                   newArrayBindingStructs[bindingEntriesNumber].entryId = iter.GetIndex();
               }
           }

           strcpy(newArrayBindingStructs[bindingEntriesNumber].deviceName, deviceName);
           if (arrayBindingStructs != NULL)
           {
               vPortFree(arrayBindingStructs);
           }
           arrayBindingStructs = newArrayBindingStructs;
           bindingEntriesNumber++;

           binding_table_flash_set(arrayBindingStructs, &bindingEntriesNumber, bindingEntriesNumber * sizeof(bindingStruct));
       }
       else
       {
           ChipLogError(NotSpecified, "newArrayBindingStructs allocation failed.");
       }
    };

    auto readFailure = [](const chip::app::ConcreteDataAttributePath * attributePath, CHIP_ERROR aError) {
        ChipLogError(DeviceLayer, "Could not retrieve device info, error: %s",  chip::ErrorStr(aError));
    };

    chip::Controller::ReadAttribute<chip::app::Clusters::BasicInformation::Attributes::NodeLabel::TypeInfo::DecodableType>(peer_device->GetExchangeManager(),
                                             peer_device->GetSecureSession().Value(),
                                            0,
                                            chip::app::Clusters::BasicInformation::Id,
                                            chip::app::Clusters::BasicInformation::Attributes::NodeLabel::Id,
                                            readSuccess, readFailure);
}

static void BoundDeviceContextReleaseHandler(void * context)
{
    (void) context;
}


static void InitBindingHandlerInternal(intptr_t arg)
{
    auto & server = chip::Server::GetInstance();
    chip::BindingManager::GetInstance().Init(
        { &server.GetFabricTable(), server.GetCASESessionManager(), &server.GetPersistentStorage() });
    chip::BindingManager::GetInstance().RegisterBoundDeviceChangedHandler(BoundDeviceChangedHandler);
    chip::BindingManager::GetInstance().RegisterBoundDeviceContextReleaseHandler(BoundDeviceContextReleaseHandler);
}

void ControllerWorkerFunction(intptr_t context)
{
    VerifyOrReturn(context != 0, ChipLogError(NotSpecified, "ControllerWorkerFunction - Invalid work data"));
    BindingCommandData * data = reinterpret_cast<BindingCommandData *>(context);
    chip::BindingManager::GetInstance().NotifyBoundClusterChanged(data->remoteEndpointId, chip::app::Clusters::OnOff::Id, static_cast<void *>(data));
}

void BindDeviceWorkerFunction(intptr_t context)
{
    chip::BindingManager::GetInstance().NotifyBoundClusterChanged(1, chip::app::Clusters::OnOff::Id, (void *)context);
}

CHIP_ERROR InitBindingHandlers()
{
    // The initialization of binding manager will try establishing connection with unicast peers
    // so it requires the Server instance to be correctly initialized. Post the init function to
    // the event queue so that everything is ready when initialization is conducted.
    // TODO: Fix initialization order issue in Matter server.
    chip::DeviceLayer::PlatformMgr().ScheduleWork(InitBindingHandlerInternal);
#if defined(ENABLE_CHIP_SHELL)
    RegisterSwitchCommands();
#endif
    return CHIP_NO_ERROR;
}
