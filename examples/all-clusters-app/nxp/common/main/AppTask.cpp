/*
 *
 *    Copyright (c) 2021-2023 Project CHIP Authors
 *    Copyright (c) 2021 Google LLC.
 *    All rights reserved.
 *    Copyright 2023-2024 NXP
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
#include "CHIPDeviceManager.h"
#include "ICDUtil.h"
#include <app/InteractionModelEngine.h>
#include <app/util/attribute-storage.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include "binding-handler.h"
#include "binding_table.h"
#include "sln_rgb_led_driver.h"

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;

#define MAX_LEVEL 99
#define MIN_LEVEL 2

extern int bindingEntriesNumber;
extern QueueHandle_t xBlindsQueue;

void AllClustersApp::AppTask::PreInitMatterStack()
{
    ChipLogProgress(DeviceLayer, "Welcome to NXP All Clusters Demo App");
}

void AllClustersApp::AppTask::PostInitMatterStack()
{
    chip::app::InteractionModelEngine::GetInstance()->RegisterReadHandlerAppCallback(&chip::NXP::App::GetICDUtil());
}

void AllClustersApp::AppTask::PostInitMatterServerInstance()
{
    // Disable last fixed endpoint, which is used as a placeholder for all of the
    // supported clusters so that ZAP will generated the requisite code.
    emberAfEndpointEnableDisable(emberAfEndpointFromIndex(static_cast<uint16_t>(emberAfFixedEndpointCount() - 1)), false);
}

void chip::NXP::App::AppTaskBase::OnOffCommandLocalAction(char *command)
{
    if (strcmp(command, "on") == 0)
    {
        uint8_t currentHue = 0;
        uint8_t currentSat = 0;
        DataModel::Nullable<uint8_t> currentLevel;

        while(1)
        {
            if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
            {
                chip::app::Clusters::ColorControl::Attributes::CurrentSaturation::Get(1, &currentSat);
                chip::app::Clusters::ColorControl::Attributes::CurrentHue::Get(1, &currentHue);
                chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Get(1, currentLevel);

                chip::DeviceLayer::PlatformMgr().UnlockChipStack();
                break;
            }
            vTaskDelay(1);
        }

        RGB_LED_SetH(&currentHue);
        RGB_LED_SetS(&currentSat);
        RGB_LED_SetBrightness(currentLevel.Value());

        while(1)
        {
            if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
            {
                chip::app::Clusters::OnOff::Attributes::OnOff::Set(1, 1);
                chip::DeviceLayer::PlatformMgr().UnlockChipStack();
                break;
            }
            vTaskDelay(1);
        }
    }
    else if (strcmp(command, "off") == 0)
    {
        RGB_LED_SetBrightness(0);
        while(1)
        {
            if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
            {
                chip::app::Clusters::OnOff::Attributes::OnOff::Set(1, 0);
                chip::DeviceLayer::PlatformMgr().UnlockChipStack();
                break;
            }
            vTaskDelay(1);
        }
    }
}

void chip::NXP::App::AppTaskBase::BrightnessChangeLocalAction(char *action, uint8_t level)
{
    DataModel::Nullable<uint8_t> currentLevel;
    uint8_t newLevel, minLevel = 2, maxLevel = 99;

    if (strcmp(action, "max") == 0 || strcmp(action, "min") == 0)
    {
        RGB_LED_SetBrightness(level);
        while(1)
        {
            if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
            {
                chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Set(1, DataModel::Nullable<uint8_t>(level));
                chip::DeviceLayer::PlatformMgr().UnlockChipStack();
                break;
            }
            vTaskDelay(1);
        }
    }
    else if (strcmp(action, "increase") == 0)
    {
        while(1)
        {
            if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
            {
                chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Get(1, currentLevel);

                chip::DeviceLayer::PlatformMgr().UnlockChipStack();
                break;
            }
            vTaskDelay(1);
        }

        if (currentLevel.Value() + level >= MAX_LEVEL)
        {
            newLevel = MAX_LEVEL;
        }
        else
        {
            newLevel = currentLevel.Value() + level;
        }

        RGB_LED_SetBrightness(newLevel);
        while(1)
        {
            if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
            {
                chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Set(1, DataModel::Nullable<uint8_t>(newLevel));
                chip::DeviceLayer::PlatformMgr().UnlockChipStack();
                break;
            }
            vTaskDelay(1);
        }
    }
    else if (strcmp(action, "decrease") == 0)
    {
        while(1)
        {
            if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
            {
                chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Get(1, currentLevel);

                chip::DeviceLayer::PlatformMgr().UnlockChipStack();
                break;
            }
            vTaskDelay(1);
        }

        if (currentLevel.Value() - level <= MIN_LEVEL)
        {
            newLevel = MIN_LEVEL;
        }
        else
        {
            newLevel = currentLevel.Value() - level;
        }

        RGB_LED_SetBrightness(newLevel);
        while(1)
        {
            if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
            {
                chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Set(1, DataModel::Nullable<uint8_t>(newLevel));
                chip::DeviceLayer::PlatformMgr().UnlockChipStack();
                break;
            }
            vTaskDelay(1);
        }
    }
}

void chip::NXP::App::AppTaskBase::ColorChangeLocalAction(char *color)
{
    uint8_t newHue, newSaturation;

    if (strcmp(color, "blue") == 0)
    {
        newHue = 170;
        newSaturation = 254;
        RGB_LED_SetH(&newHue);
        RGB_LED_SetS(&newSaturation);
    }
    else if (strcmp(color, "red") == 0)
    {
        newHue = 0;
        newSaturation = 254;
        RGB_LED_SetH(&newHue);
        RGB_LED_SetS(&newSaturation);
    }
    else if (strcmp(color, "pink") == 0)
    {
        newHue = 247;
        newSaturation = 254;
        RGB_LED_SetH(&newHue);
        RGB_LED_SetS(&newSaturation);
    }
    else if (strcmp(color, "green") == 0)
    {
        newHue = 85;
        newSaturation = 254;
        RGB_LED_SetH(&newHue);
        RGB_LED_SetS(&newSaturation);
    }
    else if (strcmp(color, "yellow") == 0)
    {
        newHue = 43;
        newSaturation = 254;
        RGB_LED_SetH(&newHue);
        RGB_LED_SetS(&newSaturation);
    }
    else if (strcmp(color, "purple") == 0)
    {
        newHue = 213;
        newSaturation = 254;
        RGB_LED_SetH(&newHue);
        RGB_LED_SetS(&newSaturation);
    }
    else if (strcmp(color, "orange") == 0)
    {
        newHue = 28;
        newSaturation = 254;
        RGB_LED_SetH(&newHue);
        RGB_LED_SetS(&newSaturation);
    }

    while(1)
    {
        if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
        {
            chip::app::Clusters::ColorControl::Attributes::CurrentHue::Set(1, newHue);
            chip::app::Clusters::ColorControl::Attributes::CurrentSaturation::Set(1, newSaturation);
            chip::DeviceLayer::PlatformMgr().UnlockChipStack();
            break;
        }
        vTaskDelay(1);
    }
}

static void OnOffCommandFunctionSendBinding(uint8_t entryId, char *command)
{
    BindingCommandData * data = chip::Platform::New<BindingCommandData>();
    data->clusterId           = chip::app::Clusters::OnOff::Id;
    data->bindingTableEntryId = entryId;
    if (strcmp(command, "on") == 0)
    {
        data->commandId       = chip::app::Clusters::OnOff::Commands::On::Id;
    }
    else if (strcmp(command, "off") == 0)
    {
        data->commandId       = chip::app::Clusters::OnOff::Commands::Off::Id;
    }
    chip::DeviceLayer::PlatformMgr().ScheduleWork(ControllerWorkerFunction, reinterpret_cast<intptr_t>(data));
}

static void BrightnessChangeFunctionSendBinding(uint8_t entryId, uint8_t level, char *action)
{
    BindingCommandData * data  = chip::Platform::New<BindingCommandData>();
    data->clusterId            = chip::app::Clusters::LevelControl::Id;
    data->commandId            = chip::app::Clusters::LevelControl::Commands::MoveToLevel::Id;
    data->bindingTableEntryId  = entryId;
    bindingBrightnessData *brightnessData = chip::Platform::New<bindingBrightnessData>();
    brightnessData->level                 = level;
    if (strcmp(action, "max") == 0 || strcmp(action, "min") == 0)
    {
        brightnessData->action = kBindingBrightnessActionSet;
    }
    else if (strcmp(action, "increase") == 0)
    {
        brightnessData->action = kBindingBrightnessActionIncrease;
    }
    else if (strcmp(action, "decrease") == 0)
    {
        brightnessData->action = kBindingBrightnessActionDecrease;
    }
    data->value = (void *) brightnessData;
    chip::DeviceLayer::PlatformMgr().ScheduleWork(ControllerWorkerFunction, reinterpret_cast<intptr_t>(data));
}

static void ColorChangeFunctionSendBinding(uint8_t entryId, char *color)
{
    BindingCommandData * data = chip::Platform::New<BindingCommandData>();
    data->clusterId           = chip::app::Clusters::ColorControl::Id;
    data->commandId           = chip::app::Clusters::ColorControl::Commands::MoveToHueAndSaturation::Id;
    data->bindingTableEntryId = entryId;
    bindingHueSaturationData *hueSaturation = chip::Platform::New<bindingHueSaturationData>();
    if (strcmp(color, "blue") == 0)
    {
        hueSaturation->hue = 170;
        hueSaturation->saturation = 254;
    }
    else if (strcmp(color, "red") == 0)
    {
        hueSaturation->hue = 0;
        hueSaturation->saturation = 254;
    }
    else if (strcmp(color, "pink") == 0)
    {
        hueSaturation->hue = 247;
        hueSaturation->saturation = 254;
    }
    else if (strcmp(color, "green") == 0)
    {
        hueSaturation->hue = 85;
        hueSaturation->saturation = 254;
    }
    else if (strcmp(color, "yellow") == 0)
    {
        hueSaturation->hue = 43;
        hueSaturation->saturation = 254;
    }
    else if (strcmp(color, "purple") == 0)
    {
        hueSaturation->hue = 213;
        hueSaturation->saturation = 254;
    }
    else if (strcmp(color, "orange") == 0)
    {
        hueSaturation->hue = 28;
        hueSaturation->saturation = 254;
    }
    data->value = (void *) hueSaturation;
    chip::DeviceLayer::PlatformMgr().ScheduleWork(ControllerWorkerFunction, reinterpret_cast<intptr_t>(data));
}

void chip::NXP::App::AppTaskBase::OnOffCommandFunction(char *command, char *location)
{
    char nodeLabel[32] = {0};
    MutableCharSpan nodeLabelSpan(nodeLabel);

    while(1)
    {
        if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
        {
            chip::app::Clusters::BasicInformation::Attributes::NodeLabel::Get(0, nodeLabelSpan);
            nodeLabel[nodeLabelSpan.size()] = '\0';
            chip::DeviceLayer::PlatformMgr().UnlockChipStack();
            break;
        }
        vTaskDelay(1);
    }

    if (strcmp(location, "all") == 0)
    {
        if (strcmp(nodeLabel, "central") != 0)
        {
            OnOffCommandLocalAction(command);
        }

        for (int i = 0; i < bindingEntriesNumber; i++)
        {
            if (strcmp(getArrayBindingStructs()[i].deviceName, "central") != 0)
            {
                OnOffCommandFunctionSendBinding(getArrayBindingStructs()[i].entryId, command);
            }
        }
    }
    else if (strcmp(nodeLabel, location) == 0)
    {
        OnOffCommandLocalAction(command);
    }
    else
    {
        for (int i = 0; i < bindingEntriesNumber; i++)
        {
            if (strcmp(getArrayBindingStructs()[i].deviceName, location) == 0)
            {
                OnOffCommandFunctionSendBinding(getArrayBindingStructs()[i].entryId, command);
                break;
            }
        }
    }
}

void chip::NXP::App::AppTaskBase::BrightnessChangeFunction(char *action, uint8_t level, char *location)
{
    char nodeLabel[32] = "";
    MutableCharSpan nodeLabelSpan(nodeLabel);

    while(1)
    {
        if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
        {
            chip::app::Clusters::BasicInformation::Attributes::NodeLabel::Get(0, nodeLabelSpan);
            nodeLabel[nodeLabelSpan.size()] = '\0';
            chip::DeviceLayer::PlatformMgr().UnlockChipStack();
            break;
        }
        vTaskDelay(1);
    }

    if (strcmp(location, "all") == 0)
    {
        if (strcmp(nodeLabel, "central") != 0)
        {
            BrightnessChangeLocalAction(action, level);
        }

        for (int i = 0; i < bindingEntriesNumber; i++)
        {
            if (strcmp(getArrayBindingStructs()[i].deviceName, "central") != 0)
            {
                BrightnessChangeFunctionSendBinding(getArrayBindingStructs()[i].entryId, level, action);
            }
        }
    }
    else if (strcmp(nodeLabel, location) == 0)
    {
        BrightnessChangeLocalAction(action, level);
    }
    else
    {
        for (int i = 0; i < bindingEntriesNumber; i++)
        {
            if (strcmp(getArrayBindingStructs()[i].deviceName, location) == 0)
            {
                BrightnessChangeFunctionSendBinding(getArrayBindingStructs()[i].entryId, level, action);
                break;
            }
        }
    }
}

void chip::NXP::App::AppTaskBase::ColorChangeFunction(char *color, char *location)
{
    char nodeLabel[32] = "";
    MutableCharSpan nodeLabelSpan(nodeLabel);

    while(1)
    {
        if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
        {
            chip::app::Clusters::BasicInformation::Attributes::NodeLabel::Get(0, nodeLabelSpan);
            nodeLabel[nodeLabelSpan.size()] = '\0';
            chip::DeviceLayer::PlatformMgr().UnlockChipStack();
            break;
        }
        vTaskDelay(1);
    }

    if (strcmp(location, "all") == 0)
    {
        if (strcmp(nodeLabel, "central") != 0)
        {
            ColorChangeLocalAction(color);
        }

        for (int i = 0; i < bindingEntriesNumber; i++)
        {
            if (strcmp(getArrayBindingStructs()[i].deviceName, "central") != 0)
            {
                ColorChangeFunctionSendBinding(getArrayBindingStructs()[i].entryId, color);
            }
        }
    }
    else if (strcmp(nodeLabel, location) == 0)
    {
        ColorChangeLocalAction(color);
    }
    else
    {
        for (int i = 0; i < bindingEntriesNumber; i++)
        {
            if (strcmp(getArrayBindingStructs()[i].deviceName, location) == 0)
            {
                ColorChangeFunctionSendBinding(getArrayBindingStructs()[i].entryId, color);
                break;
            }
        }
    }
}

void chip::NXP::App::AppTaskBase::BlindsControlFunction(char *action, uint8_t percentage, char *location)
{
    char nodeLabel[32] = "";
    MutableCharSpan nodeLabelSpan(nodeLabel);

    while(1)
    {
        if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
        {
            chip::app::Clusters::BasicInformation::Attributes::NodeLabel::Get(0, nodeLabelSpan);
            nodeLabel[nodeLabelSpan.size()] = '\0';
            chip::DeviceLayer::PlatformMgr().UnlockChipStack();
            break;
        }
        vTaskDelay(1);
    }

    if (strcmp(nodeLabel, location) == 0)
    {
        if (strcmp(action, "lift") == 0 || strcmp(action, "close") == 0)
        {
            xQueueSend(xBlindsQueue, &percentage, portMAX_DELAY);
            while(1)
            {
                if (chip::DeviceLayer::PlatformMgr().TryLockChipStack())
                {
                    chip::app::Clusters::WindowCovering::Attributes::CurrentPositionLiftPercent100ths::Set(1, DataModel::Nullable<chip::Percent100ths>(percentage));
                    chip::DeviceLayer::PlatformMgr().UnlockChipStack();
                    break;
                }
                vTaskDelay(1);
            }
        }
    }
    else
    {
        for (int i = 0; i < bindingEntriesNumber; i++)
        {
            if (strcmp(getArrayBindingStructs()[i].deviceName, location) == 0)
            {
                if (strcmp(action, "lift") == 0 || strcmp(action, "close") == 0)
                {
                    BindingCommandData * data = chip::Platform::New<BindingCommandData>();
                    data->clusterId           = chip::app::Clusters::WindowCovering::Id;
                    data->commandId           = chip::app::Clusters::WindowCovering::Commands::GoToLiftPercentage::Id;
                    data->bindingTableEntryId = getArrayBindingStructs()[i].entryId;
                    data->value               = (void *)(intptr_t)percentage;

                    chip::DeviceLayer::PlatformMgr().ScheduleWork(ControllerWorkerFunction, reinterpret_cast<intptr_t>(data));

                    break;
                }
            }
        }
    }
}

// This returns an instance of this class.
AllClustersApp::AppTask & AllClustersApp::AppTask::GetDefaultInstance()
{
    static AllClustersApp::AppTask sAppTask;
    return sAppTask;
}

chip::NXP::App::AppTaskBase & chip::NXP::App::GetAppTask()
{
    return AllClustersApp::AppTask::GetDefaultInstance();
}
