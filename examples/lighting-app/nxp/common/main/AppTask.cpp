/*
 *
 *    Copyright (c) 2021-2023 Project CHIP Authors
 *    Copyright (c) 2021 Google LLC.
 *    Copyright 2023-2024 NXP
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
#include "CHIPDeviceManager.h"
#include "ICDUtil.h"
#include <app/InteractionModelEngine.h>
#include <app/util/attribute-storage.h>
#include <platform/CHIPDeviceLayer.h>

#include "LightingManager.h"
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>

#ifdef ENABLE_CHIP_SHELL
#include <lib/shell/Engine.h>
#include <map>
using namespace chip::Shell;
#define MATTER_CLI_LOG(message) (streamer_printf(streamer_get(), message))
#endif /* ENABLE_CHIP_SHELL */

using namespace chip;
using namespace chip::TLV;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;
using namespace ::chip::DeviceManager;
using namespace ::chip::app::Clusters;


void LightingApp::AppTask::PreInitMatterStack()
{
    ChipLogProgress(DeviceLayer, "Welcome to NXP Dimmable Light Demo App");
}

void LightingApp::AppTask::PostInitMatterStack()
{
	CHIP_ERROR err = CHIP_NO_ERROR;
    chip::app::InteractionModelEngine::GetInstance()->RegisterReadHandlerAppCallback(&chip::NXP::App::GetICDUtil());
	if (LightingMgr().Init() != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Init lighting failed: %s", ErrorStr(err));
    }
	
    LightingMgr().SetCallbacks(ActionInitiated, ActionCompleted);
}


// This returns an instance of this class.
LightingApp::AppTask & LightingApp::AppTask::GetDefaultInstance()
{
    static LightingApp::AppTask sAppTask;
    return sAppTask;
}

chip::NXP::App::AppTaskBase & chip::NXP::App::GetAppTask()
{
    return LightingApp::AppTask::GetDefaultInstance();
}

void LightingApp::AppTask::ActionInitiated(LightingManager::Action_t aAction, int32_t aActor)
{
    if (aAction == LightingManager::TURNON_ACTION)
    {
        ChipLogProgress(DeviceLayer, "Turn on Action has been initiated");
    }
    else if (aAction == LightingManager::TURNOFF_ACTION)
    {
        ChipLogProgress(DeviceLayer, "Turn off Action has been initiated");
    }

    LightingApp::AppTask::GetDefaultInstance().mFunction = kFunctionTurnOnTurnOff;
}

void LightingApp::AppTask::ActionCompleted(LightingManager::Action_t aAction)
{
    if (aAction == LightingManager::TURNON_ACTION)
    {
        ChipLogProgress(DeviceLayer, "Turn on action has been completed");
    }
    else if (aAction == LightingManager::TURNOFF_ACTION)
    {
        ChipLogProgress(DeviceLayer, "Turn off action has been completed");
    }

    LightingApp::AppTask::GetDefaultInstance().mFunction = kFunction_NoneSelected;
}

void LightingApp::AppTask::UpdateClusterState(void)
{
    uint8_t newValue = 0;//!LightingMgr().IsTurnedOff();

    // write the new on/off value
    Protocols::InteractionModel::Status status = emberAfWriteAttribute(1, OnOff::Id, OnOff::Attributes::OnOff::Id,
                                                 (uint8_t *) &newValue, ZCL_BOOLEAN_ATTRIBUTE_TYPE);
    if (status != Protocols::InteractionModel::Status::Success)
    {
        ChipLogError(DeviceLayer, "ERR: updating on/off state");
    }
}





