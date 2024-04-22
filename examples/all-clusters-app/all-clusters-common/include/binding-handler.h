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

#pragma once

#include "lib/core/CHIPError.h"
#include "app-common/zap-generated/ids/Clusters.h"
#include "app-common/zap-generated/ids/Commands.h"
#include "app/clusters/bindings/bindings.h"
#include "binding_table.h"

extern int bindingsNumber;

typedef enum
{
    kBindingFunctionBindDevice = 99,
} bindingFunction_t;

struct BindingCommandData
{
    chip::NodeId bindingTableEntryId = 0;
    chip::EndpointId remoteEndpointId = 1;
    chip::CommandId commandId = 0;
    chip::ClusterId clusterId = 0;
    void *value;
};

bindingStruct *getArrayBindingStructs();
bindingStruct **getArrayBindingStructsAdr();
void freeArrayBindingStructs();
void BindingCommandsOnOff(int newState);
CHIP_ERROR InitBindingHandlers();
void ControllerWorkerFunction(intptr_t context);
void BindDeviceWorkerFunction(intptr_t context);
void GetDeviceInfo(chip::DeviceProxy * peer_device, int currentPeerId);
void ProcessOnOffUnicastBindingCommand(const EmberBindingTableEntry & binding, chip::OperationalDeviceProxy * peer_device,
        BindingCommandData *data);
void ProcessColorControlUnicastBindingCommand(const EmberBindingTableEntry & binding, chip::DeviceProxy * peer_device,
        BindingCommandData *data);
void ProcessWindowCoveringUnicastBindingCommand(const EmberBindingTableEntry & binding, chip::DeviceProxy * peer_device,
        BindingCommandData *data);
void ProcessLevelControlUnicastBindingCommand(const EmberBindingTableEntry & binding, chip::DeviceProxy * peer_device,
        BindingCommandData *data);
void ReadAndModifyBrightnessLevel(const EmberBindingTableEntry & binding, chip::DeviceProxy * peer_device,
        BindingCommandData *data);
