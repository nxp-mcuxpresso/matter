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

#include "Device.h"

#define USE_EXTERN_C
#ifdef USE_EXTERN_C
extern "C"{
    #include "ZigbeeRcpImpl.h"
}
#endif
#include "ZigbeeRcpMessage.h"
#include <app/util/MatterCallbacks.h>

class BridgeDevMgr
{
public:
    BridgeDevMgr();
    virtual ~BridgeDevMgr();

    void init();
    void start();
    void RemoveAllDevice();

    static void HandleZigbeeDeviceStatusChanged(Device * dev, Device::Changed_t itemChangedMask);
    Protocols::InteractionModel::Status HandleSendCommand(Device* dev);
    Protocols::InteractionModel::Status HandleReadAttribute(Device * dev, ClusterId clusterId, chip::AttributeId attributeId, uint8_t * buffer, uint16_t maxReadLength);
    Protocols::InteractionModel::Status HandleWriteAttribute(Device * dev, ClusterId clusterId, chip::AttributeId attributeId, uint8_t * buffer);

    friend Protocols::InteractionModel::Status emberAfExternalAttributeReadCallback(EndpointId endpoint, ClusterId clusterId,
                                                   const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer,
                                                   uint16_t maxReadLength);
    friend Protocols::InteractionModel::Status emberAfExternalAttributeWriteCallback(EndpointId endpoint, ClusterId clusterId,
                                                    const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer);
    static Device* gDevices[CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT];
    //chip::BridgeCallbacks gBridgeMatterCallbacks;
    
private:
    static void* ZigbeeRcpComm_handler(void *context);

    void AddNewZigbeeNode(m2z_device_params_t* zigbee_node);

    void RemoveDevice(Device *dev);
    int  start_ZigbeeRcp_comm_thread();

    int AddDeviceEndpoint(Device * dev, EmberAfEndpointType *ep, EmberAfDeviceType *deviceTypeList,
                                    DataVersion * dataVersionStorage, chip::EndpointId parentEndpointId);

    int RemoveDeviceEndpoint(Device * dev);

    //bool WriteAttributeToDynamicEndpoint(newdb_zcb_t zcb,uint16_t u16ClusterID,uint16_t u16AttributeID, uint64_t u64Data, uint8_t u8DataType);

    BridgeDevMgr (const BridgeDevMgr&) = delete;
    BridgeDevMgr &operator=(const BridgeDevMgr&) = delete;

protected:

    EndpointId mCurrentEndpointId;
    EndpointId mFirstDynamicEndpointId;

};

