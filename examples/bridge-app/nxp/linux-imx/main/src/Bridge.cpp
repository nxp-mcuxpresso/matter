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

#include <cstring>

#include "Bridge.h"
#include "cmd.h"

BridgeActions* Bridge::gActions;
BridgeDevMgr*  Bridge::gDevMgr;

Bridge::Bridge(const char * szDeviceName, uint32_t szBaudRate)
{
    strncpy(mSerialPort, szDeviceName, sizeof(mSerialPort));
    mBaudRate  = szBaudRate;

    gDevMgr = new BridgeDevMgr;
    gActions = new BridgeActions;
}

Bridge::~Bridge()
{
    eZCB_Finish();
    newDbClose();

    gActions->unregist();
    delete gActions;
    delete gDevMgr;

    gActions = NULL;
    gDevMgr = NULL;

    printf("free bridge ! \n");
}

int Bridge::Init()
{
    newDbOpen();

    if (eZCB_Init(mSerialPort, mBaudRate) != E_ZCB_OK) {
        printf("ZCB Init failed ! \n");
        return -1;
    }

    if (eZCB_EstablishComms() != E_ZCB_OK)
        return -1;

    if (eStartGateway() != E_ZCB_OK)
        return -1;

    if (eCheckZcb() != E_ZCB_OK)
        return -1;

    return E_ZCB_OK;
}

int Bridge::ServiceStart()
{
    gDevMgr->start();
    gActions->regist();

    return E_ZCB_OK;
}

int Bridge::test()
{
    gDevMgr->RemoveAllDevice();

    return E_ZCB_OK;
}


//-----------------------------------------------------------------------------------------
// Callback function
//-----------------------------------------------------------------------------------------
std::vector<EndpointListInfo> GetEndpointListInfo(chip::EndpointId parentId)
{
    std::vector<EndpointListInfo> infoList;

    return infoList;
}

std::vector<Action *> GetActionListInfo(chip::EndpointId parentId)
{
    return Bridge::gActions->ActionList;
}

bool emberAfActionsClusterInstantActionCallback(app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
                                                const Actions::Commands::InstantAction::DecodableType & commandData)
{
    bool hasInvokeID      = false;
    uint32_t invokeID     = 0;
    EndpointId endpointID = commandPath.mEndpointId;
    auto & actionID       = commandData.actionID;

    if (commandData.invokeID.HasValue())
    {
        hasInvokeID = true;
        invokeID    = commandData.invokeID.Value();
    }

    if ( Bridge::gActions->handle(actionID, endpointID, invokeID, hasInvokeID, Bridge::gDevMgr) == 1) {
        commandObj->AddStatus(commandPath, Protocols::InteractionModel::Status::Success);
        return true;
    }

    commandObj->AddStatus(commandPath, Protocols::InteractionModel::Status::NotFound);
    return true;
}