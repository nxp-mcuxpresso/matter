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

#include <app-common/zap-generated/attribute-type.h>
#include <app-common/zap-generated/callback.h>
#include <cstdint>
#include <cinttypes>

#include <app-common/zap-generated/callback.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app-common/zap-generated/ids/Commands.h>
#include <app/util/util.h>
#include <app/CommandHandler.h>
#include <app/InteractionModelEngine.h>
#include <lib/core/CHIPSafeCasts.h>
#include <lib/support/TypeTraits.h>

#include <app/util/MatterCallbacks.h>
#include "BridgeMgr.h"
#include "Bridge.h"


#define USE_EXTERN_C
#ifdef USE_EXTERN_C
extern "C"{
    #include "ZigbeeRcpImpl.h"
}
#endif
#include "ZigbeeRcpMessage.h"
#include <zap-generated/endpoint_config.h>
#ifdef GENERATED_FUNCTION_ARRAYS
    GENERATED_FUNCTION_ARRAYS
#else
    #error "GENERATED_FUNCTION_ARRAYS is NOT defined"
#endif

// If we have attributes that are more than 4 bytes, then
// we need this data block for the defaults
#if (defined(GENERATED_DEFAULTS) && GENERATED_DEFAULTS_COUNT)
constexpr const uint8_t generatedDefaults[] = GENERATED_DEFAULTS;
#define ZAP_LONG_DEFAULTS_INDEX(index)                                                                                             \
    {                                                                                                                              \
        &generatedDefaults[index]                                                                                                  \
    }
#endif // GENERATED_DEFAULTS

#if (defined(GENERATED_MIN_MAX_DEFAULTS) && GENERATED_MIN_MAX_DEFAULT_COUNT)
constexpr const EmberAfAttributeMinMaxValue minMaxDefaults[] = GENERATED_MIN_MAX_DEFAULTS;
#define ZAP_MIN_MAX_DEFAULTS_INDEX(index)                                                                                          \
    {                                                                                                                              \
        &minMaxDefaults[index]                                                                                                     \
    }
#endif // GENERATED_MIN_MAX_DEFAULTS

#if (defined(GENERATED_EVENTS) && (GENERATED_EVENT_COUNT > 0))
constexpr const chip::EventId generatedEvents[] = GENERATED_EVENTS;
#define ZAP_GENERATED_EVENTS_INDEX(index) (&generatedEvents[index])
#endif // GENERATED_EVENTS

constexpr const chip::CommandId generatedCommands[] = GENERATED_COMMANDS;
#define ZAP_GENERATED_COMMANDS_INDEX(index) (&generatedCommands[index])

constexpr const EmberAfAttributeMetadata generatedAttributes[] = GENERATED_ATTRIBUTES;
#define ZAP_ATTRIBUTE_INDEX(index) (&generatedAttributes[index])

constexpr const EmberAfCluster generatedClusters[] = GENERATED_CLUSTERS;
#define ZAP_CLUSTER_INDEX(index) (&generatedClusters[index])
#define DUMMY_ENDPOINT_2_FIRST_CLUSTER_INDEX 23

// define global variable
ZigbeeRcpMsg_t ZigbeeRcpMsg = {
    .AnnounceStart = false,
    .HandleMask    = true,
    .bridge_mutex  = PTHREAD_MUTEX_INITIALIZER,
    .bridge_cond   = PTHREAD_COND_INITIALIZER,
    .msg_type      = BRIDGE_UNKNOW,
};

pthread_t ZigbeeRcpComm_thread;
void send_message_to_comm_thread(int MsgType, m2z_device_params_t* ZigbeeRcp_dev, void* data);

Device * BridgeDevMgr::gDevices[CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT];
int ZcbNodesNum = 0;

//namespace {
class BridgeCallbacks: public chip::DataModelCallbacks
{
public:
    CHIP_ERROR PreCommandReceived(const chip::app::ConcreteCommandPath & commandPath,
                            const chip::Access::SubjectDescriptor & subjectDescriptor, chip::app::CommandDataIB::Parser & aCommandElement) override
    {
        //(void) InteractiveServer::GetInstance().Command(commandPath);
        uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(commandPath.mEndpointId);
        Protocols::InteractionModel::Status ret = Protocols::InteractionModel::Status::Failure;
        CHIP_ERROR TLVError = CHIP_NO_ERROR;


        if (endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
        {
            ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: mEndpointId=%d endpointIndex=%d mClusterId=0x%x mCommandId=0x%x\n", __FUNCTION__,
                            commandPath.mEndpointId ,endpointIndex ,commandPath.mClusterId ,commandPath.mCommandId);

            Device * dev = BridgeDevMgr::gDevices[endpointIndex];
            BridgeDevMgr *gDevMgr = new BridgeDevMgr;

            dev->dev_pending_command = true;
            dev->pendingCmdEndpointId = commandPath.mEndpointId;
            dev->pendingCmdClusterId = commandPath.mClusterId;
            dev->pendingCmdCommandId = commandPath.mCommandId;
            dev->cmdSize = 0;

            CommandPathIB::Parser cmdPath;
            ConcreteCommandPath concretePath(0, 0, 0);
            TLV::TLVReader DataReader;

            TLVError = aCommandElement.GetPath(&cmdPath);
            TLVError = cmdPath.GetConcreteCommandPath(concretePath);
            TLVError = aCommandElement.GetFields(&DataReader);

            
            switch (commandPath.mClusterId)
            {
                case chip::app::Clusters::LevelControl::Id:
                {
                    switch (commandPath.mCommandId)
                    {
                        case chip::app::Clusters::LevelControl::Commands::MoveToLevel::Id:
                        {
                            chip::app::Clusters::LevelControl::Commands::MoveToLevel::DecodableType commandData;
                            DataModel::Decode(DataReader, commandData);
                            dev->cmdData[dev->cmdSize++] = commandData.level;
                            dev->cmdData[dev->cmdSize++] = (uint8_t)(commandData.transitionTime.Value() >> 0) & 0xff;
                            dev->cmdData[dev->cmdSize++] = (uint8_t)(commandData.transitionTime.Value() >> 8) & 0xff;
                            dev->cmdData[dev->cmdSize++] = (commandData.optionsMask.Raw() >> 0) & 0xff;
                            dev->cmdData[dev->cmdSize++] = (commandData.optionsOverride.Raw() >> 0) & 0xff;
                            
                            break;
                        }
                    }
                }
                case chip::app::Clusters::Groups::Id:
                {
                    switch (commandPath.mCommandId)
                    {
                        case chip::app::Clusters::Groups::Commands::AddGroup::Id:
                        {
                            chip::app::Clusters::Groups::Commands::AddGroup::DecodableType commandData;
                            DataModel::Decode(DataReader, commandData);
                            dev->cmdData[dev->cmdSize++] = (uint8_t)(commandData.groupID >> 0) & 0xff;
                            dev->cmdData[dev->cmdSize++] = (uint8_t)(commandData.groupID >> 8) & 0xff;
                            memcpy(&(dev->cmdData[dev->cmdSize]), commandData.groupName.data(), commandData.groupName.size());
                            dev->cmdSize += commandData.groupName.size();
                            
                            break;
                        }
                    }
                }
            }
            if (dev->IsReachable())
            {
                ret = gDevMgr->HandleSendCommand(static_cast<Device *>(dev));
            }            
            ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s Command successfully processed: mEndpointId=%d endpointIndex=%d clusterId=0x%x commandId=0x%x\n", __FUNCTION__,
                        dev->pendingCmdEndpointId ,commandPath.mEndpointId ,dev->pendingCmdClusterId ,dev->pendingCmdCommandId);
            //if the Cluster is OnOff, we need a workarround on the LevelControl cluster if the device has this cluster
            //end the command to move to level = 0 in case of "Off" cmd
            //send the command to move to level = 254 in case of "On" cmd
            switch (commandPath.mClusterId)
            {
                case chip::app::Clusters::OnOff::Id:
                {
                    zb_uint16_t endpoint_idx=0;
                    zb_uint16_t cluster_idx=0;

                    if(dev->GetEndpointClusterIndexesForClusterId(dev->GetZigbeeRcp(), chip::app::Clusters::LevelControl::Id, &endpoint_idx, &cluster_idx))
                    {
                        dev->cmdSize = 0;
                        dev->pendingCmdEndpointId = commandPath.mEndpointId;
                        dev->pendingCmdClusterId = chip::app::Clusters::LevelControl::Id;
                        dev->pendingCmdCommandId = chip::app::Clusters::LevelControl::Commands::MoveToLevel::Id;
                        switch (commandPath.mCommandId)
                        {
                            case chip::app::Clusters::OnOff::Commands::On::Id:
                            {
                                dev->cmdData[dev->cmdSize++] = 254;
                                dev->cmdData[dev->cmdSize++] = 0;
                                dev->cmdData[dev->cmdSize++] = 0;
                                dev->cmdData[dev->cmdSize++] = 0;
                                dev->cmdData[dev->cmdSize++] = 0;
                                break;
                            }
                            case chip::app::Clusters::OnOff::Commands::Off::Id:
                            {
                                dev->cmdData[dev->cmdSize++] = 0;
                                dev->cmdData[dev->cmdSize++] = 0;
                                dev->cmdData[dev->cmdSize++] = 0;
                                dev->cmdData[dev->cmdSize++] = 0;
                                dev->cmdData[dev->cmdSize++] = 0;
                                break;
                            }
                            case chip::app::Clusters::OnOff::Commands::Toggle::Id:
                            {
                                // Uggly workarround: get the current level to toggle it !
                                uint8_t currentLevel = 0;
                                ret = gDevMgr->HandleReadAttribute(dev, chip::app::Clusters::LevelControl::Id, chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Id, &currentLevel, sizeof(currentLevel));
                                ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s currentLevel=%d\n", __FUNCTION__, currentLevel);                                
                                if(currentLevel <=1)
                                {
                                    dev->cmdData[dev->cmdSize++] = 254;
                                }
                                else
                                {
                                    dev->cmdData[dev->cmdSize++] = 1;
                                }
                                dev->cmdData[dev->cmdSize++] = 0;
                                dev->cmdData[dev->cmdSize++] = 0;
                                dev->cmdData[dev->cmdSize++] = 0;
                                dev->cmdData[dev->cmdSize++] = 0;
                                break;
                            }
                        }
                        if (dev->IsReachable())
                        {
                            ret = gDevMgr->HandleSendCommand(static_cast<Device *>(dev));
                        }
                        ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s Command successfully processed: mEndpointId=%d endpointIndex=%d clusterId=0x%x commandId=0x%x\n", __FUNCTION__,
                            dev->pendingCmdEndpointId ,commandPath.mEndpointId ,dev->pendingCmdClusterId ,dev->pendingCmdCommandId);
                            usleep(COMMAND_COMPLETED_TIME_MS * 1000);
                            
                    }
                }
            }
        }
        return(CHIP_NO_ERROR);
    }

    void PostCommandReceived(const chip::app::ConcreteCommandPath & commandPath,
                             const chip::Access::SubjectDescriptor & subjectDescriptor, chip::app::CommandDataIB::Parser & aCommandElement) override
    {
        //(void) InteractiveServer::GetInstance().Command(commandPath);
        uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(commandPath.mEndpointId);
        Protocols::InteractionModel::Status ret = Protocols::InteractionModel::Status::Failure;


        if (endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
        {
            Device * dev = BridgeDevMgr::gDevices[endpointIndex];
            BridgeDevMgr *gDevMgr = new BridgeDevMgr;

            ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: mEndpointId=%d endpointIndex=%d mClusterId=0x%x mCommandId=0x%x dev->dev_pending_command: %d\n", __FUNCTION__,
                            commandPath.mEndpointId ,endpointIndex ,commandPath.mClusterId ,commandPath.mCommandId,dev->dev_pending_command);

            if(dev->dev_pending_command == true)
            {
                ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s Command is pending: mEndpointId=%d endpointIndex=%d clusterId=0x%x commandId=0x%x\n", __FUNCTION__,
                                dev->pendingCmdEndpointId ,commandPath.mEndpointId ,dev->pendingCmdClusterId ,dev->pendingCmdCommandId);
                if (dev->IsReachable())
                {
                    //ret = gDevMgr->HandleSendCommand(static_cast<Device *>(dev));
                    dev->dev_pending_command = false;
                    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s Command successfully processed: mEndpointId=%d endpointIndex=%d clusterId=0x%x commandId=0x%x\n", __FUNCTION__,
                                dev->pendingCmdEndpointId ,commandPath.mEndpointId ,dev->pendingCmdClusterId ,dev->pendingCmdCommandId);            }

                ////if the Cluster is OnOff, we need a workarround on the LevelControl cluster if the device has this cluster
                ////end the command to move to level = 0 in case of "Off" cmd
                ////send the command to move to level = 254 in case of "On" cmd
                //switch (commandPath.mClusterId)
                //{
                    //case chip::app::Clusters::OnOff::Id:
                    //{
                        //zb_uint16_t endpoint_idx=0;
                        //zb_uint16_t cluster_idx=0;

                        //if(dev->GetEndpointClusterIndexesForClusterId(dev->GetZigbeeRcp(), chip::app::Clusters::LevelControl::Id, &endpoint_idx, &cluster_idx))
                        //{
                            //dev->cmdSize = 0;
                            //dev->pendingCmdEndpointId = commandPath.mEndpointId;
                            //dev->pendingCmdClusterId = chip::app::Clusters::LevelControl::Id;
                            //dev->pendingCmdCommandId = chip::app::Clusters::LevelControl::Commands::MoveToLevel::Id;
                            //switch (commandPath.mCommandId)
                            //{
                                //case chip::app::Clusters::OnOff::Commands::On::Id:
                                //{
                                    //dev->cmdData[dev->cmdSize++] = 254;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //break;
                                //}
                                //case chip::app::Clusters::OnOff::Commands::Off::Id:
                                //{
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //break;
                                //}
                                //case chip::app::Clusters::OnOff::Commands::Toggle::Id:
                                //{
                                    //// Uggly workarround: get the current level to toggle it !
                                    ////uint8_t currentLevel = 0;
                                    ////ret = HandleReadAttribute(dev, chip::app::Clusters::LevelControl::Id, chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Id, &currentLevel, sizeof(currentLevel));
                                    ////ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s currentLevel=%d\n", __FUNCTION__, currentLevel);                                
                                    ////if(currentLevel <=1)
                                    ////{
                                        ////dev->cmdData[dev->cmdSize++] = 254;
                                    ////}
                                    ////else
                                    ////{
                                        ////dev->cmdData[dev->cmdSize++] = 1;
                                    ////}
                                    ////dev->cmdData[dev->cmdSize++] = 0;
                                    ////dev->cmdData[dev->cmdSize++] = 0;
                                    ////dev->cmdData[dev->cmdSize++] = 0;
                                    ////dev->cmdData[dev->cmdSize++] = 0;
                                    
                                    //// to be redesigned completly, skip it for now on...
                                
                                    //break;
                                //}
                            //}
                            //if (dev->IsReachable())
                            //{
                                //ret = gDevMgr->HandleSendCommand(static_cast<Device *>(dev));
                            //}
                            //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s Command successfully processed: mEndpointId=%d endpointIndex=%d clusterId=0x%x commandId=0x%x\n", __FUNCTION__,
                                //dev->pendingCmdEndpointId ,commandPath.mEndpointId ,dev->pendingCmdClusterId ,dev->pendingCmdCommandId);
                                //usleep(COMMAND_COMPLETED_TIME_MS * 1000);
                                
                        //}
                    //}
                //}
            }
        }
    }
};

BridgeCallbacks gBridgeMatterCallbacks;

//} // namespace



BridgeDevMgr::BridgeDevMgr()
{
}

BridgeDevMgr::~BridgeDevMgr()
{
    RemoveAllDevice();
    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : free BridgeDevMgr ! \n");
}

void BridgeDevMgr::AddNewZigbeeNode(m2z_device_params_t* zigbee_node)
{
    Device * DiscoveredDevice     = new Device("Dummy Zigbee Device", "Dummy Location");
    DiscoveredDevice->SetZigbeeRcp(zigbee_node);
    DiscoveredDevice->DiscoverNode(zigbee_node);
    DiscoveredDevice->SetClusters();
    DiscoveredDevice->SetChangeCallback(&HandleZigbeeDeviceStatusChanged);
    std::string name=DiscoveredDevice->GetName();
    std::string location=DiscoveredDevice->GetLocation();
    std::string supported_clusters=DiscoveredDevice->GetClusters();
    std::string device_id=DiscoveredDevice->GetEndpointDeviceId();
    
    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge: %s : Discovered Zigbee Device: Name %s Location: %s Device Id: %s SupportedClusters: %s",__FUNCTION__,
                    name.c_str(), location.c_str(), device_id.c_str(), supported_clusters.c_str());
    
    bool state = false;
    //if(DiscoveredDevice->NodeReadAttributeIdFromClusterId(zigbee_node, ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID) == 1)
    //{
        //state = true;
    //}
    //else
    //{
        //state = false;
    //}
    DiscoveredDevice->InitState(state);

    zb_uint16_t mapped_clusters_number = 0;
    // First of all, add the "Descriptors" and "Bridged Device Basic Information" clusters
    for ( zb_uint16_t attr_idx = DUMMY_ENDPOINT_2_FIRST_CLUSTER_INDEX; attr_idx < GENERATED_CLUSTER_COUNT; attr_idx++)
    {
        if (generatedClusters[attr_idx].clusterId == Descriptor::Id)
        {
            ChipLogProgress(DeviceLayer, "\t---> matter-zigbee-bridge: %s : Found GENERATED_CLUSTERS clusterId: %d ",__FUNCTION__, generatedClusters[attr_idx].clusterId);
            DiscoveredDevice->clusterList[mapped_clusters_number++] = generatedClusters[attr_idx];
        }
        if (generatedClusters[attr_idx].clusterId == BridgedDeviceBasicInformation::Id)
        {
            ChipLogProgress(DeviceLayer, "\t---> matter-zigbee-bridge: %s : Found GENERATED_CLUSTERS clusterId: %d ",__FUNCTION__, generatedClusters[attr_idx].clusterId);
            DiscoveredDevice->clusterList[mapped_clusters_number++] = generatedClusters[attr_idx];
        }
    }

    // Then, try to map all Zigbee clusters to Matter clusters
    for ( zb_uint16_t i = 0; i < zigbee_node->endpoint; i++)
    {
        ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge:%s : EndpointId: %d",__FUNCTION__, zigbee_node->endpoints[i].ep_id);
        ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge:%s : Clusters number: %d",__FUNCTION__, zigbee_node->endpoints[i].num_in_clusters + zigbee_node->endpoints[i].num_out_clusters);
        for ( zb_uint16_t j = 0; j < (zigbee_node->endpoints[i].num_in_clusters + zigbee_node->endpoints[i].num_out_clusters); j++)
        {
            ChipLogProgress(DeviceLayer, "\t---> matter-zigbee-bridge: %s: ClusterId: %d",__FUNCTION__, zigbee_node->endpoints[i].ep_cluster[j].cluster_id);
            ChipLogProgress(DeviceLayer, "\t---> matter-zigbee-bridge: %s : Attributes number: %d",__FUNCTION__, zigbee_node->endpoints[i].ep_cluster[j].num_attrs);
            for ( zb_uint16_t k = 0; k < zigbee_node->endpoints[i].ep_cluster[j].num_attrs; k++)
            {
                ChipLogProgress(DeviceLayer, "\t\t---> matter-zigbee-bridge: %s : AttrId: %d",__FUNCTION__, zigbee_node->endpoints[i].ep_cluster[j].attribute[k].attr_id);
            }
            for ( zb_uint16_t attr_idx = DUMMY_ENDPOINT_2_FIRST_CLUSTER_INDEX; attr_idx < GENERATED_CLUSTER_COUNT; attr_idx++)
            {
                // get the Matter clusterId from the Zigbee clusterId
                ClusterId clusterId = DiscoveredDevice->GetMatterClusterId(zigbee_node->endpoints[i].ep_cluster[j].cluster_id);
                
                if (generatedClusters[attr_idx].clusterId == clusterId)
                {
                    ChipLogProgress(DeviceLayer, "\t---> matter-zigbee-bridge: %s : Found GENERATED_CLUSTERS clusterId: %d == zigbee_node->endpoints[%d].ep_cluster[%d].cluster_id: %d",__FUNCTION__, generatedClusters[attr_idx].clusterId,
                                    i ,j, zigbee_node->endpoints[i].ep_cluster[j].cluster_id);
                    DiscoveredDevice->clusterList[mapped_clusters_number++] = generatedClusters[attr_idx];
                    break;
                }
            }
        }
    }
    
    DiscoveredDevice->endpointType.cluster = (const EmberAfCluster*)(DiscoveredDevice->clusterList);
    DiscoveredDevice->endpointType.clusterCount = (zb_uint8_t)mapped_clusters_number;
    DiscoveredDevice->endpointType.endpointSize = 0;
    AddDeviceEndpoint(DiscoveredDevice, &(DiscoveredDevice->endpointType), &(DiscoveredDevice->deviceTypes),
                    DiscoveredDevice->dataVersions, 1);
}

//void BridgeDevMgr::MapZigbeeNodes()
//{
    //ZigbeeDev_t * ZigbeeDevList[CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT];
    ////newDbGetZigbeeDeviceList(ZigbeeDevList, CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT, &ZcbNodesNum);

    //for (int i = 0; i < ZcbNodesNum; i++)
    //{
        ////AddNewZigbeeNode(ZigbeeDevList[i]);
    //}

    ////newDbfreeZigbeeDeviceList(ZigbeeDevList, ZcbNodesNum);
//}

void BridgeDevMgr::init()
{
    m2z_impl_init();
}


void BridgeDevMgr::start()
{
    mFirstDynamicEndpointId = static_cast<chip::EndpointId>(
    static_cast<int>(emberAfEndpointFromIndex(static_cast<uint16_t>(emberAfFixedEndpointCount() - 1))) + 1);
    mCurrentEndpointId = mFirstDynamicEndpointId;

    chip::DataModelCallbacks::SetInstance(&gBridgeMatterCallbacks);

    // start monitor
    m2z_impl_start_threads();
    start_ZigbeeRcp_comm_thread();
    m2z_register_message_callback(send_message_to_comm_thread);

}

void BridgeDevMgr::RemoveAllDevice()
{
    for (uint16_t i = 0; i < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT; i++)
    {
        if (gDevices[i] != NULL)
        {
            RemoveDevice(gDevices[i]);
        }
    }

    memset(gDevices, 0, sizeof(Device *) * CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT);
    mCurrentEndpointId = mFirstDynamicEndpointId;
}

void BridgeDevMgr::RemoveDevice(Device * dev)
{
    RemoveDeviceEndpoint(dev);
    //if (dev->GetZigbee()->zcb.uSupportedClusters.sClusterBitmap.hasOnOff)
    //{
        //RemoveOnOffNode(static_cast<DeviceOnOff *>(dev));
    //}

    //if (dev->GetZigbee()->zcb.uSupportedClusters.sClusterBitmap.hasTemperatureSensing)
    //{
        //RemoveTempMeasurementNode(static_cast<DeviceTempSensor *>(dev));
    //}
}

//void BridgeDevMgr::RemoveOnOffNode(DeviceOnOff * dev)
//{
    //delete dev;
    //dev = nullptr;
//}

//void BridgeDevMgr::RemoveTempMeasurementNode(DeviceTempSensor * dev)
//{
    //delete dev;
    //dev = nullptr;
//}

// -----------------------------------------------------------------------------------------
// Device Management
// -----------------------------------------------------------------------------------------

int BridgeDevMgr::AddDeviceEndpoint(Device * dev, EmberAfEndpointType *ep, EmberAfDeviceType *deviceTypeList,
                                    DataVersion * dataVersionStorage,
                                    chip::EndpointId parentEndpointId)
{
    uint8_t index = 0;

    if (dev->IsReachable() == true)
    {
        ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : The endpoints has been added!");
        return -1;
    }

    while (index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    {
        if (nullptr == gDevices[index])
        {
            gDevices[index] = dev;
            CHIP_ERROR ret;
            while (1)
            {
                DeviceLayer::StackLock lock;
                dev->SetEndpointId(mCurrentEndpointId);
                dev->SetParentEndpointId(parentEndpointId);
                
                chip::Span<EmberAfDeviceType> device_types(deviceTypeList, /* current_endpoint->device_type_count*/ 2);
                chip::Span<chip::DataVersion> data_versions(dataVersionStorage, dev->GetEndpointClustersCount());
                
                ret = emberAfSetDynamicEndpoint(index, mCurrentEndpointId, ep, data_versions, device_types, parentEndpointId);
                if (ret == CHIP_NO_ERROR)
                {
                    dev->SetReachable(true);
                    dev->SetMatterIndex(index);
                    std::string name=dev->GetName();
                    ChipLogProgress(DeviceLayer,
                                    " ---> matter-zigbee-bridge : Added device %s addr %x to dynamic endpoint %d (index=%d)",
                                    (dev->GetName()).c_str(), dev->GetZigbeeRcpSaddr(), mCurrentEndpointId, index);
                    return index;
                }
                if (ret != CHIP_ERROR_ENDPOINT_EXISTS)
                {
                    return -1;
                }
                // Handle wrap condition
                if (++mCurrentEndpointId < mFirstDynamicEndpointId)
                {
                    mCurrentEndpointId = mFirstDynamicEndpointId;
                }
            }
        }
        index++;
    }
    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : Failed to add dynamic endpoint: No endpoints available!");
    return -1;
}

int BridgeDevMgr::RemoveDeviceEndpoint(Device * dev)
{
    uint8_t index = 0;
    while (index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    {
        if (gDevices[index] == dev)
        {
            // Todo: Update this to schedule the work rather than use this lock
            DeviceLayer::StackLock lock;
            // Silence complaints about unused ep when progress logging
            // disabled.
            [[maybe_unused]] EndpointId ep   = emberAfClearDynamicEndpoint(index);
            gDevices[index] = nullptr;
            ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : Removed device %s from dynamic endpoint %d (index=%d)",
                            (dev->GetName()).c_str(), ep, index);

            return index;
        }
        index++;
    }
    return -1;
}

void * BridgeDevMgr::ZigbeeRcpComm_handler(void * context)
{
    BridgeDevMgr * ThisMgr = (BridgeDevMgr *) context;
    m2z_device_params_t *ZigbeeRcp_device;
    WCS_TRACE_DBGREL(" ---> matter-zigbee-bridge : BridgeDevMgr::ZigbeeRcpMonitor is running !!!\n");
    while (1)
    {
        pthread_mutex_lock(&ZigbeeRcpMsg.bridge_mutex);
        while (ZigbeeRcpMsg.HandleMask)
        {
            if (pthread_cond_wait(&ZigbeeRcpMsg.bridge_cond, &ZigbeeRcpMsg.bridge_mutex) == 0)
            {
                WCS_TRACE_DBGREL(" ---> ZigbeeRcpComm_handler RECEIVED A MESSAGE\n");
                ZigbeeRcp_device = (m2z_device_params_t *)ZigbeeRcpMsg.ZigbeeRcp_dev;
                
                switch (ZigbeeRcpMsg.msg_type)
                {
                    case BRIDGE_ADD_DEV: {
                        //ThisMgr->NodeReadAllAttributes(ZigbeeRcp_device);
                        ZigbeeRcp_device->dev_state = ANNOUNCE_COMPLETED;
                        ThisMgr->AddNewZigbeeNode(ZigbeeRcp_device);
                        ZigbeeRcpMsg.ZigbeeRcp_dev      = { 0 };
                        ZigbeeRcpMsg.msg_type = BRIDGE_UNKNOW;
                        while(ZigbeeRcp_device->dev_state == ANNOUNCE_COMPLETED)
                        {
                            sleep(1);
                        }
                        WCS_TRACE_DBGREL(" ---> all attributes were read !!!\n");
                        //ThisMgr->AddNewZigbeeNode(ZigbeeRcp_device);
                    }
                    break;

                //case BRIDGE_REMOVE_DEV: {
                    //ThisMgr->RemoveDevice(gDevices[ZcbMsg.zcb.matterIndex]);
                    //ZcbMsg.zcb      = { 0 };
                    //ZcbMsg.msg_type = BRIDGE_UNKNOW;
                //}
                //break;

                //case BRIDGE_FACTORY_RESET: {
                    //sleep(1);
                    //ThisMgr->RemoveAllDevice();
                    //ZcbMsg.zcb      = { 0 };
                    //ZcbMsg.msg_type = BRIDGE_UNKNOW;
                //}
                //break;

                //case BRIDGE_WRITE_ATTRIBUTE: {
                    //ZcbAttribute_t * Data = (ZcbAttribute_t *) ZcbMsg.msg_data;
                    //ChipLogProgress(DeviceLayer,
                                    //" ---> matter-zigbee-bridge : WriteAttributeToDynamicEndpoint: u16ClusterID: %d "
                                    //"u16AttributeID: %d u64Data: %ld\n",
                                    //Data->u16ClusterID, Data->u16AttributeID, Data->u64Data);
                    //ThisMgr->WriteAttributeToDynamicEndpoint(ZcbMsg.zcb, Data->u16ClusterID, Data->u16AttributeID, Data->u64Data,
                                                             //ZCL_INT16S_ATTRIBUTE_TYPE);
                    //free(Data);
                    //ZcbMsg.zcb      = { 0 };
                    //ZcbMsg.msg_type = BRIDGE_UNKNOW;
                //}
                //break;

                    default:
                        break;
                }
            }

            //if (ThisMgr->thread_exit)
            //    return NULL;
        }

        ZigbeeRcpMsg.HandleMask = true;
        pthread_mutex_unlock(&ZigbeeRcpMsg.bridge_mutex);
    }
    return NULL;
}

int BridgeDevMgr::start_ZigbeeRcp_comm_thread()
{
    int res = pthread_create(&ZigbeeRcpComm_thread, NULL, ZigbeeRcpComm_handler, (void *) this);
    if (res)
    {
        WCS_TRACE_DBGREL(" ---> matter-zigbee-bridge : Error creating polling thread: %d\n", res);
        return -1;
    }

    return 0;
}

void send_message_to_comm_thread(int MsgType, m2z_device_params_t* ZigbeeRcp_dev, void* data)
{
    pthread_mutex_lock(&ZigbeeRcpMsg.bridge_mutex);

    ZigbeeRcpMsg.HandleMask = false;
    pthread_cond_signal(&ZigbeeRcpMsg.bridge_cond);

    if( ZigbeeRcp_dev != NULL ) {
        ZigbeeRcpMsg.ZigbeeRcp_dev = ZigbeeRcp_dev;
        ZigbeeRcpMsg.msg_type = MsgType;
        ZigbeeRcpMsg.msg_data = data;
        WCS_TRACE_DBGREL(" ---> matter-zigbee-bridge : type: %d", ZigbeeRcpMsg.msg_type);
    } else {
        ZigbeeRcpMsg.msg_type = MsgType;
    }

    pthread_mutex_unlock(&ZigbeeRcpMsg.bridge_mutex);
}

// -----------------------------------------------------------------------------------------
// Device callback
// -----------------------------------------------------------------------------------------
void CallReportingCallback(intptr_t closure)
{
    auto path = reinterpret_cast<app::ConcreteAttributePath *>(closure);
    MatterReportingAttributeChangeCallback(*path);
    Platform::Delete(path);
}

void ScheduleReportingCallback(Device * dev, ClusterId cluster, AttributeId attribute)
{
    auto * path = Platform::New<app::ConcreteAttributePath>(dev->GetEndpointId(), cluster, attribute);
    PlatformMgr().ScheduleWork(CallReportingCallback, reinterpret_cast<intptr_t>(path));
}

void HandleDeviceStatusChanged(Device * dev, Device::Changed_t itemChangedMask)
{
    if (itemChangedMask & Device::kChanged_Reachable)
    {
        ScheduleReportingCallback(dev, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::Reachable::Id);
    }

    if (itemChangedMask & Device::kChanged_Name)
    {
        ScheduleReportingCallback(dev, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::NodeLabel::Id);
    }
}

Protocols::InteractionModel::Status HandleReadBridgedDeviceBasicAttribute(Device * dev, chip::AttributeId attributeId, uint8_t * buffer,
                                                    uint16_t maxReadLength)
{
    using namespace BridgedDeviceBasicInformation::Attributes;

    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: attrId=%d, maxReadLength=%d",__FUNCTION__,
                    attributeId, maxReadLength);

    //if ((attributeId == Reachable::Id) && (maxReadLength == 1))
    //{
        //*buffer = dev->IsReachable() ? 1 : 0;
    //}
    //else if ((attributeId == NodeLabel::Id) && (maxReadLength == 32))
    //{
        //MutableByteSpan zclNameSpan(buffer, maxReadLength);
        //MakeZclCharString(zclNameSpan, (dev->GetName()).c_str());
    //}
    //else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2))
    //{
        //uint16_t rev = ZCL_BRIDGED_DEVICE_BASIC_INFORMATION_CLUSTER_REVISION;
        //memcpy(buffer, &rev, sizeof(rev));
    //}
    //else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4))
    //{
        //uint32_t featureMap = ZCL_BRIDGED_DEVICE_BASIC_INFORMATION_FEATURE_MAP;
        //memcpy(buffer, &featureMap, sizeof(featureMap));
    //}
    //else
    //{
        //return Protocols::InteractionModel::Status::Failure;
    //}

    return Protocols::InteractionModel::Status::Success;
}

Protocols::InteractionModel::Status BridgeDevMgr::HandleReadAttribute(Device * dev, ClusterId clusterId, chip::AttributeId attributeId, uint8_t * buffer, uint16_t maxReadLength)
{
    chip::AttributeId translated_attributeId = attributeId;
    
    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: attrId=%d, maxReadLength=%d",__FUNCTION__, translated_attributeId,
                    maxReadLength);

    // if clusterId is BridgedDeviceBasicInformation::Id
    if(clusterId == BridgedDeviceBasicInformation::Id)
    {
        // Zigbee clusterId should be 0x0 (the Basic Zigbee cluster)
        clusterId = 0;
        // a translation is also required from Matter to Zigbee attribute ID
        switch(attributeId)
        {
			case chip::app::Clusters::BasicInformation::Attributes::DataModelRevision::Id: 		translated_attributeId = ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::VendorName::Id: 			translated_attributeId = ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::VendorID::Id: 				translated_attributeId = 0xFF;break;
			case chip::app::Clusters::BasicInformation::Attributes::ProductName::Id: 			translated_attributeId = 0xff;break;
			case chip::app::Clusters::BasicInformation::Attributes::ProductID::Id: 				translated_attributeId = ZB_ZCL_ATTR_BASIC_PRODUCT_CODE_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::NodeLabel::Id: 				translated_attributeId = ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::Location::Id: 				translated_attributeId = ZB_ZCL_ATTR_BASIC_LOCATION_DESCRIPTION_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::HardwareVersion::Id: 		translated_attributeId = ZB_ZCL_ATTR_BASIC_HW_VERSION_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::HardwareVersionString::Id: 	translated_attributeId = 0xff;break;
			case chip::app::Clusters::BasicInformation::Attributes::SoftwareVersion::Id: 		translated_attributeId = ZB_ZCL_ATTR_BASIC_SW_BUILD_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::SoftwareVersionString::Id:	translated_attributeId = 0xff;break;
			case chip::app::Clusters::BasicInformation::Attributes::ManufacturingDate::Id: 		translated_attributeId = ZB_ZCL_ATTR_BASIC_DATE_CODE_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::PartNumber::Id: 			translated_attributeId = ZB_ZCL_ATTR_BASIC_MANUFACTURER_VERSION_DETAILS_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::ProductURL::Id: 			translated_attributeId = ZB_ZCL_ATTR_BASIC_PRODUCT_URL_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::ProductLabel::Id: 			translated_attributeId = ZB_ZCL_ATTR_BASIC_PRODUCT_LABEL_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::SerialNumber::Id: 			translated_attributeId = ZB_ZCL_ATTR_BASIC_SERIAL_NUMBER_ID;break;
			case chip::app::Clusters::BasicInformation::Attributes::LocalConfigDisabled::Id: 	translated_attributeId = 0xff;break;
			case chip::app::Clusters::BasicInformation::Attributes::Reachable::Id: 				translated_attributeId = 0xff;break;
			case chip::app::Clusters::BasicInformation::Attributes::UniqueID::Id: 				translated_attributeId = 0xff;break;
			case chip::app::Clusters::BasicInformation::Attributes::CapabilityMinima::Id: 		translated_attributeId = 0xff;break;
			case chip::app::Clusters::BasicInformation::Attributes::ProductAppearance::Id: 		translated_attributeId = 0xff;break;
			case chip::app::Clusters::BasicInformation::Attributes::SpecificationVersion::Id: 	translated_attributeId = 0xff;break;
			case chip::app::Clusters::BasicInformation::Attributes::MaxPathsPerInvoke::Id: 		translated_attributeId = 0xff;break;
			//case chip::app::Clusters::BasicInformation::Attributes::DeviceLocation::Id: 		translated_attributeId = 0xff;break;
		}
        
    }
    dev->readAttribute(dev, (uint16_t)clusterId, (uint16_t)translated_attributeId);
    usleep(COMMAND_COMPLETED_TIME_MS * 1000);
    dev->NodeReadAttributeIdFromClusterId(dev->GetZigbeeRcp(), (uint16_t)clusterId, (uint16_t)translated_attributeId, buffer, maxReadLength);
    //if ((attributeId == OnOff::Attributes::OnOff::Id) && (maxReadLength == 1))
    //{
        //*buffer = dev->IsOn() ? 1 : 0;
    //}
    //else if ((attributeId == OnOff::Attributes::ClusterRevision::Id) && (maxReadLength == 2))
    //{
        //uint16_t rev = ZCL_ON_OFF_CLUSTER_REVISION;
        //memcpy(buffer, &rev, sizeof(rev));
    //}
    //else
    //{
        //return Protocols::InteractionModel::Status::Failure;
    //}

    return Protocols::InteractionModel::Status::Success;
}

Protocols::InteractionModel::Status BridgeDevMgr::HandleWriteAttribute(Device * dev, ClusterId clusterId, chip::AttributeId attributeId, uint8_t * buffer)
{
    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: clusterId=%d attrId=%d",__FUNCTION__, clusterId, attributeId);
    dev->writeAttribute(dev, (uint16_t)clusterId, (uint16_t)attributeId, buffer);
    usleep(COMMAND_COMPLETED_TIME_MS * 1000);

    //if ((attributeId == OnOff::Attributes::OnOff::Id) && (dev->IsReachable()))
    //{
        //if (*buffer)
        //{
            //dev->SetOnOff(true);
        //}
        //else
        //{
            //dev->SetOnOff(false);
        //}
    //}
    //else
    //{
        //return Protocols::InteractionModel::Status::Failure;
    //}

    return Protocols::InteractionModel::Status::Success;
}

void GetAttributeReadOnlyStatus(ClusterId clusterId, chip::AttributeId attributeId, Protocols::InteractionModel::Status *attr_zb_status)
{
    switch (clusterId)
    {   case chip::app::Clusters::OnOff::Id:
        {
            switch(attributeId)
            {
                case chip::app::Clusters::OnOff::Attributes::OnOff::Id:
                case chip::app::Clusters::OnOff::Attributes::GlobalSceneControl::Id:
                {
                    *attr_zb_status = Protocols::InteractionModel::Status::UnsupportedWrite;
                }
                break;
            }
        }
        break;
        case chip::app::Clusters::LevelControl::Id:
        {
            switch(attributeId)
            {
                case chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Id:
                case chip::app::Clusters::LevelControl::Attributes::RemainingTime::Id:
                case chip::app::Clusters::LevelControl::Attributes::MinLevel::Id:
                case chip::app::Clusters::LevelControl::Attributes::MaxLevel::Id:
                case chip::app::Clusters::LevelControl::Attributes::CurrentFrequency::Id:
                case chip::app::Clusters::LevelControl::Attributes::MinFrequency::Id:
                case chip::app::Clusters::LevelControl::Attributes::MaxFrequency::Id:
                {
                    *attr_zb_status = Protocols::InteractionModel::Status::UnsupportedWrite;
                }
                break;
            }
        }
        break;
    }
    
    WCS_TRACE_DBGREL("<< %s attribute_zb_status: %d ", __FUNCTION__, *attr_zb_status);
    
}

Protocols::InteractionModel::Status CanWriteAttribute(Device * dev, ClusterId clusterId, chip::AttributeId attributeId, bool check_from_standard_spec)
{
    Protocols::InteractionModel::Status zcl_attr_status = Protocols::InteractionModel::Status::Success;
    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: clusterId=%d attrId=%d", __FUNCTION__, clusterId, attributeId);
    if(check_from_standard_spec)
    {
        GetAttributeReadOnlyStatus((uint16_t)clusterId, (uint16_t)attributeId, (Protocols::InteractionModel::Status *)&zcl_attr_status);
    }
    else
    {
        dev->canWriteAttribute(dev, (uint16_t)clusterId, (uint16_t)attributeId, &zcl_attr_status);
    }
    
    return(zcl_attr_status);
}

Protocols::InteractionModel::Status BridgeDevMgr::HandleSendCommand(Device* dev)
{
    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s:",__FUNCTION__);
    uint8_t ret = 0;
    ret = dev->sendCommand(dev);
    if (ret != 0)
    {
        return Protocols::InteractionModel::Status::Failure;
    }
    return Protocols::InteractionModel::Status::Success;
}


void BridgeDevMgr::HandleZigbeeDeviceStatusChanged(Device * dev, Device::Changed_t itemChangedMask)
{
    if (itemChangedMask & (Device::kChanged_Reachable | Device::kChanged_Name | Device::kChanged_Location))
    {
        HandleDeviceStatusChanged(static_cast<Device *>(dev), (Device::Changed_t) itemChangedMask);
    }

    if (itemChangedMask & Device::kChanged_Reachable)
    {
        ScheduleReportingCallback(dev, OnOff::Id, OnOff::Attributes::OnOff::Id);
    }
}

Protocols::InteractionModel::Status emberAfExternalAttributeReadCallback(EndpointId endpoint, ClusterId clusterId,
                                                   const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer,
                                                   uint16_t maxReadLength)
{
    uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(endpoint);
    BridgeDevMgr *gDevMgr = new BridgeDevMgr;

    Protocols::InteractionModel::Status ret = Protocols::InteractionModel::Status::Failure;
    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: ep=%d endpointIndex=%d clusterId=%d attributeId=%d  maxReadLength=%d\n",__FUNCTION__,
                    endpoint, endpointIndex, clusterId, attributeMetadata->attributeId, maxReadLength);

    if ((endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT) && (BridgeDevMgr::gDevices[endpointIndex] != nullptr))
    {
        Device * dev = BridgeDevMgr::gDevices[endpointIndex];

        if (dev->IsReachable())
        {
            ret = gDevMgr->HandleReadAttribute(dev, clusterId, attributeMetadata->attributeId, buffer, maxReadLength);
        }
    }

    return ret;
}

Protocols::InteractionModel::Status emberAfExternalAttributeWriteCallback(EndpointId endpoint, ClusterId clusterId,
                                                    const EmberAfAttributeMetadata * attributeMetadata, uint8_t * buffer)
{
    uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(endpoint);

    Protocols::InteractionModel::Status ret = Protocols::InteractionModel::Status::Failure;
    bool do_send_command = false;

    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: ep=%d endpointIndex=%d clusterId=%d attributeId=%d\n", __FUNCTION__,
                    endpoint, endpointIndex, clusterId, attributeMetadata->attributeId);

    if (endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    {
        Device * dev = BridgeDevMgr::gDevices[endpointIndex];
        BridgeDevMgr *gDevMgr = new BridgeDevMgr;

        if (dev->IsReachable())
        {
            ret = CanWriteAttribute(static_cast<Device *>(dev), clusterId, attributeMetadata->attributeId, true);
            if(ret == Protocols::InteractionModel::Status::Success)
            {
                ret = gDevMgr->HandleWriteAttribute(static_cast<Device *>(dev), clusterId, attributeMetadata->attributeId, buffer);
            }
            if(ret == Protocols::InteractionModel::Status::UnsupportedWrite)
            {
                zb_uint16_t endpoint_idx=0;
                zb_uint16_t cluster_idx=0;
                ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: UNSUPPORTED_WRITE for ep=%d endpointIndex=%d clusterId=%d attributeId=%d\n", __FUNCTION__,
                                endpoint, endpointIndex, clusterId, attributeMetadata->attributeId);
                // Matter wants to write a Zigbee bridged node attribute which is ReadOnly attribute
                // A translation to a Command may be needed here, but not implemented yet...
                
                //if(dev->GetEndpointClusterIndexesForClusterId(dev->GetZigbeeRcp(), clusterId, &endpoint_idx, &cluster_idx))
                //{
                    //dev->cmdSize = 0;
                    //dev->pendingCmdEndpointId = endpoint;
                    //dev->pendingCmdClusterId = clusterId;
                    //switch (clusterId)
                    //{
                        //case chip::app::Clusters::OnOff::Id:
                        //{
                            //switch(attributeMetadata->attributeId)
                            //{
                                //case chip::app::Clusters::OnOff::Attributes::OnOff::Id:
                                //{
                                    //if(*buffer == 0)
                                    //{
                                        //dev->pendingCmdCommandId = chip::app::Clusters::OnOff::Commands::Off::Id;
                                    //}
                                    //else
                                    //{
                                        //dev->pendingCmdCommandId = chip::app::Clusters::OnOff::Commands::On::Id;
                                    //}
                                    //do_send_command = true;
                                //}
                                //break;
                            //}
                        //}
                        //break;
                        //case chip::app::Clusters::LevelControl::Id:
                        //{
                            //switch(attributeMetadata->attributeId)
                            //{
                                //case chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Id:
                                //{
                                    //dev->pendingCmdCommandId = chip::app::Clusters::LevelControl::Commands::MoveToLevel::Id;
                                    //memcpy(&(dev->cmdData[dev->cmdSize]), buffer, attributeMetadata->size);
                                    //dev->cmdSize += attributeMetadata->size;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //dev->cmdData[dev->cmdSize++] = 0;
                                    //do_send_command = true;
                                //}
                                //break;
                            //}
                        //}
                        //break;
                    //}
                    //if (dev->IsReachable() && do_send_command)
                    //{
                        //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: HandleSendCommand for ep=%d endpointIndex=%d clusterId=%d attributeId=%d\n", __FUNCTION__,
                            //endpoint, endpointIndex, clusterId, attributeMetadata->attributeId);
                        ////ret = gDevMgr->HandleSendCommand(static_cast<Device *>(dev));
                        //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: Command successfully processed for ep=%d endpointIndex=%d clusterId=%d attributeId=%d\n", __FUNCTION__,
                            //endpoint, endpointIndex, clusterId, attributeMetadata->attributeId);
                        
                    //}
                //}
            }
            if(ret == Protocols::InteractionModel::Status::UnsupportedAttribute)
            {
                ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: UNSUPPORTED_ATTRIBUTE for ep=%d endpointIndex=%d clusterId=%d attributeId=%d\n", __FUNCTION__,
                                endpoint, endpointIndex, clusterId, attributeMetadata->attributeId);
            }
        }
    }

    //return ret;
    return(Protocols::InteractionModel::Status::Success);
}

#include <lib/core/TLVData.h>
#include <lib/core/TLVTags.h>
#include <lib/core/TLVUtilities.h>
//constexpr uint8_t sNoFields[] = {
    //CHIP_TLV_STRUCTURE(CHIP_TLV_TAG_ANONYMOUS),
    //CHIP_TLV_END_OF_CONTAINER,
//};

//CHIP_ERROR __attribute__((weak)) MatterPreCommandReceivedCallback(const chip::app::ConcreteCommandPath & commandPath,
                                                                  //const chip::Access::SubjectDescriptor & subjectDescriptor, chip::app::CommandDataIB::Parser & aCommandElement)
//chip::DataModelCallbacks::PreCommandReceived(const chip::app::ConcreteCommandPath & commandPath,
                                                                  //const chip::Access::SubjectDescriptor & subjectDescriptor, chip::app::CommandDataIB::Parser & aCommandElement)
//{
    //uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(commandPath.mEndpointId);
    //Protocols::InteractionModel::Status ret = Protocols::InteractionModel::Status::Failure;
    //CHIP_ERROR TLVError = CHIP_NO_ERROR;

    //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: mEndpointId=%d endpointIndex=%d mClusterId=0x%x mCommandId=0x%x\n", __FUNCTION__,
                    //commandPath.mEndpointId ,endpointIndex ,commandPath.mClusterId ,commandPath.mCommandId);

    //if (endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    //{
        //Device * dev = BridgeDevMgr::gDevices[endpointIndex];
        //dev->dev_pending_command = true;
        //dev->pendingCmdEndpointId = commandPath.mEndpointId;
        //dev->pendingCmdClusterId = commandPath.mClusterId;
        //dev->pendingCmdCommandId = commandPath.mCommandId;
        //dev->cmdSize = 0;

        //CommandPathIB::Parser cmdPath;
        //ConcreteCommandPath concretePath(0, 0, 0);
        //TLV::TLVReader DataReader;

        //TLVError = aCommandElement.GetPath(&cmdPath);
        //TLVError = cmdPath.GetConcreteCommandPath(concretePath);
        //TLVError = aCommandElement.GetFields(&DataReader);

        
        //switch (commandPath.mClusterId)
        //{
            //case chip::app::Clusters::LevelControl::Id:
            //{
                //switch (commandPath.mCommandId)
                //{
                    //case chip::app::Clusters::LevelControl::Commands::MoveToLevel::Id:
                    //{
                        //chip::app::Clusters::LevelControl::Commands::MoveToLevel::DecodableType commandData;
                        //DataModel::Decode(DataReader, commandData);
                        //dev->cmdData[dev->cmdSize++] = commandData.level;
                        //dev->cmdData[dev->cmdSize++] = (uint8_t)(commandData.transitionTime.Value() >> 0) & 0xff;
                        //dev->cmdData[dev->cmdSize++] = (uint8_t)(commandData.transitionTime.Value() >> 8) & 0xff;
                        //dev->cmdData[dev->cmdSize++] = (commandData.optionsMask.Raw() >> 0) & 0xff;
                        //dev->cmdData[dev->cmdSize++] = (commandData.optionsOverride.Raw() >> 0) & 0xff;
                        
                        //break;
                    //}
                //}
            //}
            //case chip::app::Clusters::Groups::Id:
            //{
                //switch (commandPath.mCommandId)
                //{
                    //case chip::app::Clusters::Groups::Commands::AddGroup::Id:
                    //{
                        //chip::app::Clusters::Groups::Commands::AddGroup::DecodableType commandData;
                        //DataModel::Decode(DataReader, commandData);
                        //dev->cmdData[dev->cmdSize++] = (uint8_t)(commandData.groupID >> 0) & 0xff;
                        //dev->cmdData[dev->cmdSize++] = (uint8_t)(commandData.groupID >> 8) & 0xff;
                        //memcpy(&(dev->cmdData[dev->cmdSize]), commandData.groupName.data(), commandData.groupName.size());
                        //dev->cmdSize += commandData.groupName.size();
                        
                        //break;
                    //}
                //}
            //}
        //}
    //}
    //return(CHIP_NO_ERROR);
//}
//void __attribute__((weak)) MatterPostCommandReceivedCallback(const chip::app::ConcreteCommandPath & commandPath,
                                                             //const chip::Access::SubjectDescriptor & subjectDescriptor, chip::app::CommandDataIB::Parser & aCommandElement)
//chip::DataModelCallbacks::PostCommandReceived(const chip::app::ConcreteCommandPath & commandPath,
                                                             //const chip::Access::SubjectDescriptor & subjectDescriptor, chip::app::CommandDataIB::Parser & aCommandElement)
//{
    //uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(commandPath.mEndpointId);
    //Protocols::InteractionModel::Status ret = Protocols::InteractionModel::Status::Failure;

    //if (endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    //{
        //Device * dev = BridgeDevMgr::gDevices[endpointIndex];
        //if(dev->dev_pending_command == true)
        //{
            //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s Command is pending: mEndpointId=%d endpointIndex=%d clusterId=0x%x commandId=0x%x\n", __FUNCTION__,
                            //dev->pendingCmdEndpointId ,commandPath.mEndpointId ,dev->pendingCmdClusterId ,dev->pendingCmdCommandId);
            //if (dev->IsReachable())
            //{
                //ret = HandleSendCommand(static_cast<Device *>(dev));
                //dev->dev_pending_command = false;
                //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s Command successfully processed: mEndpointId=%d endpointIndex=%d clusterId=0x%x commandId=0x%x\n", __FUNCTION__,
                            //dev->pendingCmdEndpointId ,commandPath.mEndpointId ,dev->pendingCmdClusterId ,dev->pendingCmdCommandId);            }

            ////if the Cluster is OnOff, we need a workarround on the LevelControl cluster if the device has this cluster
            ////end the command to move to level = 0 in case of "Off" cmd
            ////send the command to move to level = 254 in case of "On" cmd
            //switch (commandPath.mClusterId)
            //{
                //case chip::app::Clusters::OnOff::Id:
                //{
                    //zb_uint16_t endpoint_idx=0;
                    //zb_uint16_t cluster_idx=0;

                    //if(dev->GetEndpointClusterIndexesForClusterId(dev->GetZigbeeRcp(), chip::app::Clusters::LevelControl::Id, &endpoint_idx, &cluster_idx))
                    //{
                        //dev->cmdSize = 0;
                        //dev->pendingCmdEndpointId = commandPath.mEndpointId;
                        //dev->pendingCmdClusterId = chip::app::Clusters::LevelControl::Id;
                        //dev->pendingCmdCommandId = chip::app::Clusters::LevelControl::Commands::MoveToLevel::Id;
                        //switch (commandPath.mCommandId)
                        //{
                            //case chip::app::Clusters::OnOff::Commands::On::Id:
                            //{
                                //dev->cmdData[dev->cmdSize++] = 254;
                                //dev->cmdData[dev->cmdSize++] = 0;
                                //dev->cmdData[dev->cmdSize++] = 0;
                                //dev->cmdData[dev->cmdSize++] = 0;
                                //dev->cmdData[dev->cmdSize++] = 0;
                                //break;
                            //}
                            //case chip::app::Clusters::OnOff::Commands::Off::Id:
                            //{
                                //dev->cmdData[dev->cmdSize++] = 0;
                                //dev->cmdData[dev->cmdSize++] = 0;
                                //dev->cmdData[dev->cmdSize++] = 0;
                                //dev->cmdData[dev->cmdSize++] = 0;
                                //dev->cmdData[dev->cmdSize++] = 0;
                                //break;
                            //}
                            //case chip::app::Clusters::OnOff::Commands::Toggle::Id:
                            //{
                                //// Uggly workarround: get the current level to toggle it !
                                ////uint8_t currentLevel = 0;
                                ////ret = HandleReadAttribute(dev, chip::app::Clusters::LevelControl::Id, chip::app::Clusters::LevelControl::Attributes::CurrentLevel::Id, &currentLevel, sizeof(currentLevel));
                                ////ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s currentLevel=%d\n", __FUNCTION__, currentLevel);                                
                                ////if(currentLevel <=1)
                                ////{
                                    ////dev->cmdData[dev->cmdSize++] = 254;
                                ////}
                                ////else
                                ////{
                                    ////dev->cmdData[dev->cmdSize++] = 1;
                                ////}
                                ////dev->cmdData[dev->cmdSize++] = 0;
                                ////dev->cmdData[dev->cmdSize++] = 0;
                                ////dev->cmdData[dev->cmdSize++] = 0;
                                ////dev->cmdData[dev->cmdSize++] = 0;
                                
                                //// to be redesigned completly, skip it for now on...
                            
                                //break;
                            //}
                        //}
                        //if (dev->IsReachable())
                        //{
                            //ret = HandleSendCommand(static_cast<Device *>(dev));
                        //}
                        //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s Command successfully processed: mEndpointId=%d endpointIndex=%d clusterId=0x%x commandId=0x%x\n", __FUNCTION__,
                            //dev->pendingCmdEndpointId ,commandPath.mEndpointId ,dev->pendingCmdClusterId ,dev->pendingCmdCommandId);
                            
                    //}
                //}
            //}
        //}
    //}
//}

//void MatterPreAttributeReadCallback(const chip::app::ConcreteAttributePath & attributePath)
//{
    //uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(attributePath.mEndpointId);
    //Protocols::InteractionModel::Status ret = Protocols::InteractionModel::Status::Failure;

    //if (endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    //{
        //Device * dev = BridgeDevMgr::gDevices[endpointIndex];
        //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: mEndpointId=%d endpointIndex=%d mClusterId=0x%x mAttributeId=0x%x\n", __FUNCTION__,
                        //dev->pendingCmdEndpointId ,attributePath.mEndpointId ,attributePath.mClusterId, attributePath.mAttributeId );
    //}
//}

//void MatterPostAttributeReadCallback(const chip::app::ConcreteAttributePath & attributePath)
//{
    //uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(attributePath.mEndpointId);
    //Protocols::InteractionModel::Status ret = Protocols::InteractionModel::Status::Failure;

    //if (endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    //{
        //Device * dev = BridgeDevMgr::gDevices[endpointIndex];
        //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: mEndpointId=%d endpointIndex=%d mClusterId=0x%x mAttributeId=0x%x\n", __FUNCTION__,
                        //dev->pendingCmdEndpointId ,attributePath.mEndpointId ,attributePath.mClusterId, attributePath.mAttributeId );
    //}
//}

//void MatterPreAttributeWriteCallback(const chip::app::ConcreteAttributePath & attributePath)
//{
    //uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(attributePath.mEndpointId);
    //Protocols::InteractionModel::Status ret = Protocols::InteractionModel::Status::Failure;

    //if (endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    //{
        //Device * dev = BridgeDevMgr::gDevices[endpointIndex];
        //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: mEndpointId=%d endpointIndex=%d mClusterId=0x%x mAttributeId=0x%x\n", __FUNCTION__,
                        //dev->pendingCmdEndpointId ,attributePath.mEndpointId ,attributePath.mClusterId, attributePath.mAttributeId );
    //}
//}

//void MatterPostAttributeWriteCallback(const chip::app::ConcreteAttributePath & attributePath)
//{
    //uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(attributePath.mEndpointId);
    //Protocols::InteractionModel::Status ret = Protocols::InteractionModel::Status::Failure;

    //if (endpointIndex < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT)
    //{
        //Device * dev = BridgeDevMgr::gDevices[endpointIndex];
        //ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : %s: mEndpointId=%d endpointIndex=%d mClusterId=0x%x mAttributeId=0x%x\n", __FUNCTION__,
                        //dev->pendingCmdEndpointId ,attributePath.mEndpointId ,attributePath.mClusterId, attributePath.mAttributeId );
    //}
//}



// -----------------------------------------------------------------------------------------
//  zcb node report attribute to Bridge
// -----------------------------------------------------------------------------------------
//bool BridgeDevMgr::WriteAttributeToDynamicEndpoint(newdb_zcb_t zcb, uint16_t u16ClusterID, uint16_t u16AttributeID,
                                                   //uint64_t u64Data, uint8_t u8DataType)
//{
    //if (gDevices[zcb.matterIndex] == NULL)
    //{
        //return false;
    //}

    //if (u8DataType == ZCL_INT16S_ATTRIBUTE_TYPE)
    //{
        //DeviceTempSensor * TempSensor = static_cast<DeviceTempSensor *>(gDevices[zcb.matterIndex]);
        //TempSensor->SetMeasuredValue(static_cast<int16_t>(u64Data));
        //return true;
    //}

    //return false;
//}
