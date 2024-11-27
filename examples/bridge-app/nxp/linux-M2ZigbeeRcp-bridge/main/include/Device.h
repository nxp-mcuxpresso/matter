/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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

#include <app/util/attribute-storage.h>
#include <app/util/attribute-metadata.h> // EmberAfAttributeMetadata

#include <stdbool.h>
#include <stdint.h>

#include <functional>
#include <vector>

#include "BridgeConfig.h"
//#include "newDb.h"
#define USE_EXTERN_C
#ifdef USE_EXTERN_C
extern "C"{
    #include "ZigbeeRcpImpl.h"
}
#endif
#include "ZigbeeRcpMessage.h"

class Device
{
public:
    static const int kDeviceNameSize = 32;

    enum Changed_t
    {
        kChanged_Reachable = 1u << 0,
        kChanged_Location  = 1u << 1,
        kChanged_Name      = 1u << 2,
        kChanged_Last      = kChanged_Name,
    } Changed;

    Device(std::string szDeviceName, std::string szLocation);
    ~Device() {}
    //virtual ~Device() {}

    bool IsReachable();
    void SetReachable(bool aReachable);
    void SetName(std::string szDeviceName);
    void SetLocation(std::string szLocation);
    void SetClusters();
    void InitState(bool aOn);
    void SetEndpointDeviceId(std::string szDeviceId);
    inline void SetEndpointId(chip::EndpointId id) { mEndpointId = id; };
    inline void SetMatterIndex(int index) { mMatterIndex = index; };
    inline int GetMatterIndex(int index) { return mMatterIndex; };
    inline chip::EndpointId GetEndpointId() { return mEndpointId; };
    inline void SetParentEndpointId(chip::EndpointId id) { mParentEndpointId = id; };
    inline chip::EndpointId GetParentEndpointId() { return mParentEndpointId; };
    inline std::string GetName() { return mName; };
    inline std::string GetLocation() { return mLocation; };
    inline std::string GetZone() { return mZone; };
    inline std::string GetClusters() { return mClusters; };
    inline int GetEndpointClustersCount() { return ep_clusters_count; };
    inline std::string GetEndpointDeviceId() { return mEpDeviceId; };
    inline void SetZone(std::string zone) { mZone = zone; };
    void DiscoverNode(m2z_device_params_t* zigbee_node);
    bool HasCluster(uint16_t zcl_cluster_id);
    ClusterId GetMatterClusterId(uint16_t zcl_cluster_id);
    uint8_t NodeReadAttributeIdFromClusterId(m2z_device_params_t * zigbee_node, uint16_t zcl_cluster_id, uint16_t zcl_attr_id, uint8_t * buffer, uint16_t maxReadLength);
    uint8_t readAttribute(Device* dev, uint16_t zcl_cluster_id, uint16_t zcl_attr_id);
    uint8_t writeAttribute(Device* dev, uint16_t zcl_cluster_id, uint16_t zcl_attr_id, uint8_t * buffer);
    uint8_t canWriteAttribute(Device* dev, uint16_t zcl_cluster_id, uint16_t zcl_attr_id, Protocols::InteractionModel::Status * zcl_attr_status);
    uint8_t sendCommand(Device* dev);
    uint8_t GetEndpointClusterIndexesForClusterId(m2z_device_params_t * zigbee_dev, uint16_t zcl_cluster_id, zb_uint16_t* ep_id, zb_uint16_t* cluster_id);
    uint8_t GetEndpointClusterAttributeIndexesForClusterAttrId(m2z_device_params_t * zigbee_dev, uint16_t zcl_cluster_id, uint16_t zcl_attr_id, zb_uint16_t* ep_id, zb_uint16_t* cluster_id, zb_uint16_t* attr_id);
    /* Zigbee devices info */
    inline void SetZigbeeRcp(m2z_device_params_t * dev) { mBridgeZigbeeRcpNode = dev; };
    inline int GetZigbeeRcpSaddr() { return mBridgeZigbeeRcpNode->short_addr; };
    inline m2z_device_params_t* GetZigbeeRcp() { return mBridgeZigbeeRcpNode; };

    EmberAfEndpointType endpointType;
    EmberAfDeviceType deviceTypes;
    EmberAfCluster clusterList[MATTER_ZIGBEERCP_BRIDGE_MAX_CLUSTERS_PER_EP];
    DataVersion dataVersions[MATTER_ZIGBEERCP_BRIDGE_MAX_CLUSTERS_PER_EP];
    bool dev_pending_command;
    uint8_t cmdSize;
    EndpointId pendingCmdEndpointId;
    ClusterId pendingCmdClusterId;
    CommandId pendingCmdCommandId;
    uint8_t cmdData[64];

    using DeviceCallback_fn = std::function<void(Device *, Device::Changed_t)>;
    void SetChangeCallback(DeviceCallback_fn aChanged_CB);

private:
    void HandleDeviceChange(Device * device, Device::Changed_t changeMask);
    std::string GetManufacturer(m2z_device_params_t* zigbee_node);
    std::string GetModelIdentifier(m2z_device_params_t* zigbee_node);
    std::string GetLocation(m2z_device_params_t* zigbee_node);
    std::string GetEndpointDeviceId(m2z_device_params_t* zigbee_node);
    bool NodeReadAllAttributes(m2z_device_params_t* zigbee_node);
    void BasicClusterForceAttributesRead(zb_uint8_t device_index, zb_uint8_t endpoint_index, m2z_device_cluster_t *cluster);
    std::string GetSupportedClusterString();
    DeviceCallback_fn mChanged_CB;
    bool mOn;


protected:
    bool mReachable;
    std::string mName;
    std::string mLocation;
    std::string mClusters;
    std::string mEpDeviceId;
    chip::EndpointId mEndpointId;
    chip::EndpointId mParentEndpointId;
    int ep_clusters_count;
    std::string mZone;
    int mMatterIndex;
    m2z_device_params_t * mBridgeZigbeeRcpNode;
};

class EndpointListInfo
{
public:
    EndpointListInfo(uint16_t endpointListId, std::string name, chip::app::Clusters::Actions::EndpointListTypeEnum type);
    EndpointListInfo(uint16_t endpointListId, std::string name, chip::app::Clusters::Actions::EndpointListTypeEnum type,
                     chip::EndpointId endpointId);
    void AddEndpointId(chip::EndpointId endpointId);
    inline uint16_t GetEndpointListId() { return mEndpointListId; };
    std::string GetName() { return mName; };
    inline chip::app::Clusters::Actions::EndpointListTypeEnum GetType() { return mType; };
    inline chip::EndpointId * GetEndpointListData() { return mEndpoints.data(); };
    inline size_t GetEndpointListSize() { return mEndpoints.size(); };

private:
    uint16_t mEndpointListId = static_cast<uint16_t>(0);
    std::string mName;
    chip::app::Clusters::Actions::EndpointListTypeEnum mType = static_cast<chip::app::Clusters::Actions::EndpointListTypeEnum>(0);
    std::vector<chip::EndpointId> mEndpoints;
};

class Room
{
public:
    Room(std::string name, uint16_t endpointListId, chip::app::Clusters::Actions::EndpointListTypeEnum type, bool isVisible);
    inline void setIsVisible(bool isVisible) { mIsVisible = isVisible; };
    inline bool getIsVisible() { return mIsVisible; };
    inline void setName(std::string name) { mName = name; };
    inline std::string getName() { return mName; };
    inline chip::app::Clusters::Actions::EndpointListTypeEnum getType() { return mType; };
    inline uint16_t getEndpointListId() { return mEndpointListId; };

private:
    bool mIsVisible;
    std::string mName;
    uint16_t mEndpointListId;
    chip::app::Clusters::Actions::EndpointListTypeEnum mType;
};

class Action
{
public:
    Action();
    Action(uint16_t actionId, std::string name, chip::app::Clusters::Actions::ActionTypeEnum type, uint16_t endpointListId,
           uint16_t supportedCommands, chip::app::Clusters::Actions::ActionStateEnum status, bool isVisible);
    inline void setName(std::string name) { mName = name; };
    inline std::string getName() { return mName; };
    inline chip::app::Clusters::Actions::ActionTypeEnum getType() { return mType; };
    inline chip::app::Clusters::Actions::ActionStateEnum getStatus() { return mStatus; };
    inline uint16_t getActionId() { return mActionId; };
    inline uint16_t getEndpointListId() { return mEndpointListId; };
    inline uint16_t getSupportedCommands() { return mSupportedCommands; };
    inline void setIsVisible(bool isVisible) { mIsVisible = isVisible; };
    inline bool getIsVisible() { return mIsVisible; };

private:
    std::string mName;
    chip::app::Clusters::Actions::ActionTypeEnum mType;
    chip::app::Clusters::Actions::ActionStateEnum mStatus;
    uint16_t mActionId;
    uint16_t mEndpointListId;
    uint16_t mSupportedCommands;
    bool mIsVisible;
};
