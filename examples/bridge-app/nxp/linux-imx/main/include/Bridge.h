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

#pragma once

#include <stdint.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <app/clusters/network-commissioning/network-commissioning.h>
#include <app/EventLogging.h>
#include <app/reporting/reporting.h>
#include <app/util/af-types.h>
#include <app/util/attribute-storage.h>
#include <app/util/util.h>

#include "Device.h"
#include "BridgeActions.h"
#include "BridgeMgr.h"
#include "zcb.h"
#include "cmd.h"

using namespace chip;
using namespace chip::app;
using namespace chip::Credentials;
using namespace chip::Inet;
using namespace chip::Transport;
using namespace chip::DeviceLayer;
using namespace chip::app::Clusters;


class Bridge
{
public:
    Bridge(const char * szSerialPortName, uint32_t szBaudRate);
    virtual ~Bridge();

    int Init();
    int ServiceStart();
    int test();

    /* Matter Bridge callback */
    friend std::vector<EndpointListInfo> GetEndpointListInfo(chip::EndpointId parentId);
    friend std::vector<Action *> GetActionListInfo(chip::EndpointId parentId);
    friend bool emberAfActionsClusterInstantActionCallback(app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
                                                const Actions::Commands::InstantAction::DecodableType & commandData);

private:
    int AddDevice();

    Bridge (const Bridge&) = delete;
    Bridge &operator=(const Bridge&) = delete;

protected:
    char mSerialPort[15];
    uint32_t mBaudRate;

    static BridgeDevMgr  *gDevMgr;
    static BridgeActions *gActions;
};