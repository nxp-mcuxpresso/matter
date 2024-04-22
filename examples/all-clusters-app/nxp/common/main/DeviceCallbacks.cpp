/*
 *
 *    Copyright (c) 2020-2023 Project CHIP Authors
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

/**
 * @file DeviceCallbacks.cpp
 *
 * Implements all the callbacks to the application from the CHIP Stack
 *
 **/
#include "DeviceCallbacks.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/clusters/identify-server/identify-server.h>
#include <app/server/Dnssd.h>
#include <app/util/attribute-storage.h>
#include <app/util/attribute-table.h>

#include <lib/support/CodeUtils.h>
#include "sln_rgb_led_driver.h"
#include "sln_pwm_driver_flexio.h"

extern QueueHandle_t xBlindsQueue;
static bool blindsInit = false;

using namespace chip::app;
void OnTriggerEffect(::Identify * identify)
{
    switch (identify->mCurrentEffectIdentifier)
    {
    case Clusters::Identify::EffectIdentifierEnum::kBlink:
        ChipLogProgress(Zcl, "Clusters::Identify::EffectIdentifierEnum::kBlink");
        break;
    case Clusters::Identify::EffectIdentifierEnum::kBreathe:
        ChipLogProgress(Zcl, "Clusters::Identify::EffectIdentifierEnum::kBreathe");
        break;
    case Clusters::Identify::EffectIdentifierEnum::kOkay:
        ChipLogProgress(Zcl, "Clusters::Identify::EffectIdentifierEnum::kOkay");
        break;
    case Clusters::Identify::EffectIdentifierEnum::kChannelChange:
        ChipLogProgress(Zcl, "Clusters::Identify::EffectIdentifierEnum::kChannelChange");
        break;
    default:
        ChipLogProgress(Zcl, "No identifier effect");
        return;
    }
}

Identify gIdentify0 = {
    chip::EndpointId{ 1 },
    [](Identify *) { ChipLogProgress(Zcl, "onIdentifyStart"); },
    [](Identify *) { ChipLogProgress(Zcl, "onIdentifyStop"); },
    chip::app::Clusters::Identify::IdentifyTypeEnum::kNone,
    OnTriggerEffect,
};

Identify gIdentify1 = {
    chip::EndpointId{ 1 },
    [](Identify *) { ChipLogProgress(Zcl, "onIdentifyStart"); },
    [](Identify *) { ChipLogProgress(Zcl, "onIdentifyStop"); },
    chip::app::Clusters::Identify::IdentifyTypeEnum::kNone,
    OnTriggerEffect,
};

using namespace ::chip;
using namespace ::chip::Inet;
using namespace ::chip::System;
using namespace ::chip::DeviceLayer;
using namespace chip::app::Clusters;

void AllClustersApp::DeviceCallbacks::PostAttributeChangeCallback(EndpointId endpointId, ClusterId clusterId,
                                                                  AttributeId attributeId, uint8_t type, uint16_t size,
                                                                  uint8_t * value)
{
    ChipLogProgress(DeviceLayer,
                    "endpointId " ChipLogFormatMEI " clusterId " ChipLogFormatMEI " attribute ID: " ChipLogFormatMEI
                    " Type: %u Value: %u, length %u",
                    ChipLogValueMEI(endpointId), ChipLogValueMEI(clusterId), ChipLogValueMEI(attributeId), type, *value, size);
    switch (clusterId)
    {
    case Clusters::OnOff::Id:
        OnOnOffPostAttributeChangeCallback(endpointId, attributeId, value);
        break;

    case Clusters::BasicInformation::Id:
        break;

    case Clusters::ColorControl::Id:
        OnColorControlPostAttributeChangeCallback(endpointId, attributeId, value);
        break;

    case Clusters::LevelControl::Id:
        OnLevelControlPostAttributeChangeCallback(endpointId, attributeId, value);
        break;

    case Clusters::WindowCovering::Id:
        OnWindowCoveringPostAttributeChangeCallback(endpointId, attributeId, (uint16_t *)value);
        break;
    }
}

void AllClustersApp::DeviceCallbacks::OnOnOffPostAttributeChangeCallback(chip::EndpointId endpointId, chip::AttributeId attributeId,
                                                                         uint8_t * value)
{
    switch (attributeId)
    {
    case Clusters::OnOff::Attributes::OnOff::Id:
        if (*value) {
            uint8_t currentHue = 0;
            uint8_t currentSat = 0;
            DataModel::Nullable<uint8_t> currentLevel;
            chip::app::Clusters::ColorControl::Attributes::CurrentSaturation::Get(1, &currentSat);
            chip::app::Clusters::ColorControl::Attributes::CurrentHue::Get(1, &currentHue);
            chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Get(1, currentLevel);
            RGB_LED_SetH(&currentHue);
            RGB_LED_SetS(&currentSat);
            RGB_LED_SetBrightness(currentLevel.Value());
        } else {
            RGB_LED_SetBrightness(0);
        }
        break;
    default:;
    }
}

void AllClustersApp::DeviceCallbacks::OnColorControlPostAttributeChangeCallback(chip::EndpointId endpointId, chip::AttributeId attributeId,
                                                                         uint8_t * value)
{
    switch (attributeId)
    {
    case Clusters::ColorControl::Attributes::CurrentHue::Id:
        RGB_LED_SetH(value);
        break;
    case Clusters::ColorControl::Attributes::CurrentSaturation::Id:
        RGB_LED_SetS(value);
        break;
    default:;
    }
}

void AllClustersApp::DeviceCallbacks::OnLevelControlPostAttributeChangeCallback(chip::EndpointId endpointId, chip::AttributeId attributeId,
                                                                         uint8_t * value)
{
    uint8_t minLevel = 1;
    uint8_t maxLevel = 100;

    switch (attributeId)
    {
    case Clusters::LevelControl::Attributes::CurrentLevel::Id:
        if (*value > minLevel && *value < maxLevel)
        {
            bool onoff = false;
            uint8_t currentHue = 0;
            uint8_t currentSat = 0;
            chip::app::Clusters::ColorControl::Attributes::CurrentSaturation::Get(1, &currentSat);
            chip::app::Clusters::ColorControl::Attributes::CurrentHue::Get(1, &currentHue);
            chip::app::Clusters::OnOff::Attributes::OnOff::Get(1, &onoff);
            if (onoff)
            {
                RGB_LED_SetH(&currentHue);
                RGB_LED_SetS(&currentSat);
                RGB_LED_SetBrightness(*value);
            }
        }
        break;
    default:;
    }
}

void AllClustersApp::DeviceCallbacks::OnWindowCoveringPostAttributeChangeCallback(chip::EndpointId endpointId, chip::AttributeId attributeId,
                                                                         uint16_t * value)
{
   uint8_t valueToSend = (uint8_t)*value;

    /* update internal lift percentage */
   if (!blindsInit)
   {
       DataModel::Nullable<chip::Percent100ths> liftPercentage;
       chip::app::Clusters::WindowCovering::Attributes::CurrentPositionLiftPercent100ths::Get(1, liftPercentage);
       PWM_UpdatePercentageOpen(liftPercentage.Value());
       blindsInit = true;
   }

   switch (attributeId)
   {
   case Clusters::WindowCovering::Attributes::TargetPositionLiftPercent100ths::Id:
       xQueueSend(xBlindsQueue, &valueToSend, portMAX_DELAY);
       chip::app::Clusters::WindowCovering::Attributes::CurrentPositionLiftPercent100ths::Set(1, DataModel::Nullable<chip::Percent100ths>(valueToSend));
       break;
   default:;
   }
}


// This returns an instance of this class.
AllClustersApp::DeviceCallbacks & AllClustersApp::DeviceCallbacks::GetDefaultInstance()
{
    static AllClustersApp::DeviceCallbacks sDeviceCallbacks;
    return sDeviceCallbacks;
}

chip::DeviceManager::CHIPDeviceManagerCallbacks & chip::NXP::App::GetDeviceCallbacks()
{
    return AllClustersApp::DeviceCallbacks::GetDefaultInstance();
}
