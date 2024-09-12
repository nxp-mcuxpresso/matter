/*
 *
 *    Copyright (c) 2021-2022 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
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

#include "Device.h"

#include <cstdio>
#include <platform/CHIPDeviceLayer.h>

//#include "zcb.h"
#define USE_EXTERN_C
#ifdef USE_EXTERN_C
extern "C"{
    #include "ZigbeeRcpImpl.h"
}
#endif

using namespace chip::app::Clusters::Actions;

Device::Device(std::string szDeviceName, std::string szLocation)
{
    mName = szDeviceName;
    mLocation   = szLocation;
    mReachable  = false;
    mEndpointId = 0;
}
uint8_t Device::GetEndpointClusterIndexesForClusterId(m2z_device_params_t * zigbee_dev, uint16_t zcl_cluster_id, zb_uint16_t* ep_id, zb_uint16_t* cluster_id)
{
    zb_uint16_t endpoint_num = zigbee_dev->endpoint, endpoint_idx;
    zb_uint16_t cluster_num=0, cluster_idx=0;
    uint8_t cluster_match=0;
    for(endpoint_idx=0; endpoint_idx <=endpoint_num;endpoint_idx++)
    {
        cluster_num=zigbee_dev->endpoints[endpoint_idx].num_in_clusters+zigbee_dev->endpoints[endpoint_idx].num_out_clusters;
        for(cluster_idx=0; cluster_idx <=cluster_num;cluster_idx++)
        {
            if(zigbee_dev->endpoints[endpoint_idx].ep_cluster[cluster_idx].cluster_id == zcl_cluster_id)
            {
                cluster_match=1;
                *ep_id = endpoint_idx;
                *cluster_id = cluster_idx;
                break;
            }
        }
        if(cluster_match)
            break;
    }
    return cluster_match;
}

uint8_t Device::NodeReadAttributeIdFromClusterId(m2z_device_params_t * zigbee_node, uint16_t zcl_cluster_id, uint16_t zcl_attr_id, uint8_t * buffer, uint16_t maxReadLength)
{
    zb_uint16_t ep_id, cluster_id, attr_id;
    m2z_device_cluster_attr_t *attribute = NULL;
    
    if(GetEndpointClusterAttributeIndexesForClusterAttrId(zigbee_node, zcl_cluster_id, zcl_attr_id, &ep_id, &cluster_id, &attr_id))
    {
        attribute = &(zigbee_node->endpoints[ep_id].ep_cluster[cluster_id].attribute[attr_id]);
        memcpy(buffer, &(attribute->attr_value_array), maxReadLength);
        return (0);
    }
    return (0xFF);
}

uint8_t Device::readAttribute(Device* dev, uint16_t zcl_cluster_id, uint16_t zcl_attr_id)
{
    m2z_schedule_read_attribute(dev->mBridgeZigbeeRcpNode, zcl_cluster_id, zcl_attr_id);
    return(0);
}

uint8_t Device::writeAttribute(Device* dev, uint16_t zcl_cluster_id, uint16_t zcl_attr_id, uint8_t * buffer)
{
    m2z_schedule_write_attribute(dev->mBridgeZigbeeRcpNode, zcl_cluster_id, zcl_attr_id, buffer);
    return(0);
}

uint8_t Device::canWriteAttribute(Device* dev, uint16_t zcl_cluster_id, uint16_t zcl_attr_id, Protocols::InteractionModel::Status * zcl_attr_status)
{
    m2z_get_attribute_zb_status(dev->mBridgeZigbeeRcpNode, zcl_cluster_id, zcl_attr_id, (uint8_t*)zcl_attr_status);
    return(0);
}

uint8_t Device::sendCommand(Device* dev)
{
    m2z_schedule_send_command(dev->mBridgeZigbeeRcpNode, (uint16_t)dev->pendingCmdClusterId, (uint16_t)dev->pendingCmdCommandId, dev->cmdData, dev->cmdSize);
    return(0);
}

uint8_t Device::GetEndpointClusterAttributeIndexesForClusterAttrId(m2z_device_params_t * zigbee_dev, uint16_t zcl_cluster_id, uint16_t zcl_attr_id, zb_uint16_t* ep_id, zb_uint16_t* cluster_id, zb_uint16_t* attr_id)
{
    zb_uint16_t endpoint_idx=0;
    zb_uint16_t cluster_idx=0;
    zb_uint16_t attr_num=0, attr_idx=0;
    uint8_t attribute_match=0;
    if(GetEndpointClusterIndexesForClusterId(zigbee_dev, zcl_cluster_id, &endpoint_idx, &cluster_idx))
    {
        attr_num=zigbee_dev->endpoints[endpoint_idx].ep_cluster[cluster_idx].num_attrs;
        for(attr_idx=0; attr_idx <=attr_num;attr_idx++)
        {
            if(zigbee_dev->endpoints[endpoint_idx].ep_cluster[cluster_idx].attribute[attr_idx].attr_id == zcl_attr_id)
            {
                attribute_match = 1;
                *ep_id = endpoint_idx;
                *cluster_id = cluster_idx;
                *attr_id = attr_idx;
                return attribute_match;
            }
        }
    }
    return attribute_match;
}

bool Device::NodeReadAllAttributes(m2z_device_params_t* zigbee_node)
{
    m2z_device_ep_t *endpoint = NULL;
    m2z_device_cluster_t *cluster = NULL;
    m2z_device_cluster_attr_t *attribute = NULL;
    bool attr_to_read = false;
    
    for( zb_uint8_t ep_idx=0; ep_idx < zigbee_node->endpoint; ep_idx++)
    {
        endpoint=&(zigbee_node->endpoints[ep_idx]);
        for( zb_uint8_t cluster_idx=0; cluster_idx < (endpoint->num_in_clusters + endpoint->num_out_clusters); cluster_idx++)
        {
            cluster = &(endpoint->ep_cluster[cluster_idx]);
            for ( zb_uint8_t attr_idx=0; attr_idx < cluster->num_attrs; attr_idx++)
            {
                attribute = &(cluster->attribute[attr_idx]);
                if(attribute->attr_state == DISCOVERED_ATTR)
                {
                    m2z_schedule_read_attribute(zigbee_node, cluster->cluster_id, attribute->read_req.attr_id);
                    usleep(COMMAND_COMPLETED_TIME_MS * 200);
                    attr_to_read = true;
                }
            }
        }
    }
    return(attr_to_read);
}

std::string Device::GetManufacturer(m2z_device_params_t* zigbee_node)
{
    zb_uint16_t ep_id, cluster_id, attr_id;
    m2z_device_cluster_attr_t *attribute = NULL;
    
    if(GetEndpointClusterAttributeIndexesForClusterAttrId(zigbee_node, ZB_ZCL_CLUSTER_ID_BASIC, ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, &ep_id, &cluster_id, &attr_id))
    {
        attribute = &(zigbee_node->endpoints[ep_id].ep_cluster[cluster_id].attribute[attr_id]);
        std::string modelId((char *)(&attribute->attr_value_array[1]), attribute->attr_value_array[0]);
        return (modelId);
    }
    return ("");
}

std::string Device::GetModelIdentifier(m2z_device_params_t* zigbee_node)
{
    zb_uint16_t ep_id, cluster_id, attr_id;
    m2z_device_cluster_attr_t *attribute = NULL;
    
    if(GetEndpointClusterAttributeIndexesForClusterAttrId(zigbee_node, ZB_ZCL_CLUSTER_ID_BASIC, ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, &ep_id, &cluster_id, &attr_id))
    {
        attribute = &(zigbee_node->endpoints[ep_id].ep_cluster[cluster_id].attribute[attr_id]);
        std::string modelId((char *)(&attribute->attr_value_array[1]), attribute->attr_value_array[0]);
        return (modelId);
    }
    return ("");
}

std::string Device::GetLocation(m2z_device_params_t* zigbee_node)
{
    zb_uint16_t ep_id, cluster_id, attr_id;
    m2z_device_cluster_attr_t *attribute = NULL;
    
    if(GetEndpointClusterAttributeIndexesForClusterAttrId(zigbee_node, ZB_ZCL_CLUSTER_ID_BASIC, ZB_ZCL_ATTR_BASIC_LOCATION_DESCRIPTION_ID, &ep_id, &cluster_id, &attr_id))
    {
        attribute = &(zigbee_node->endpoints[ep_id].ep_cluster[cluster_id].attribute[attr_id]);
        std::string modelId((char *)(&attribute->attr_value_array[1]), attribute->attr_value_array[0]);
        return (modelId);
    }
    return ("");
}

std::string Device::GetEndpointDeviceId(m2z_device_params_t* zigbee_node)
{
    switch(zigbee_node->endpoints[0].device_id)
    {
        case 0x0:
            return("On/Off Switch");
        break;
        case 0x1:
            return("Level Control Switch");
        break;
        case 0x2:
            return("On/Off Output");
        break;
        case 0x3:
            return("Level Controllable Ouput");
        break;
        case 0x4:
            return("Scene Selector");
        break;
        case 0x5:
            return("Configuration Tool");
        break;
        case 0x6:
            return("Remote Control");
        break;
        case 0x7:
            return("Combined Interface");
        break;
        case 0x8:
            return("Range Extender");
        break;
        case 0x9:
            return("Mains Power Outlet");
        break;
        case 0x100:
            return("On/Off Light");
        break;
        case 0x101:
            return("Dimmable Light");
        break;
        case 0x102:
            return("Color Dimmable Light");
        break;
        case 0x103:
            return("On/Off Light Switch");
        break;
        case 0x104:
            return("Dimmer Switch");
        break;
        case 0x105:
            return("Color Dimmer Switch");
        break;
        case 0x106:
            return("Light Sensor");
        break;
        case 0x107:
            return("Occupancy Sensor");
        break;
        case 0x200:
            return("Shade");
        break;
        case 0x201:
            return("Shade Controller");
        break;
        case 0x300:
            return("Heating/Cooling Unit");
        break;
        case 0x301:
            return("Thermostat");
        break;
        case 0x302:
            return("Temperature Sensor");
        break;
        case 0x303:
            return("Pump");
        break;
        case 0x304:
            return("Pump Controller");
        break;
        case 0x305:
            return("Pressure Sensor");
        break;
        case 0x306:
            return("Flow Sensor");
        break;
        case 0x400:
            return("IAS Control and Indicating Equipment");
        break;
        case 0x401:
            return("IAS Ancillary Control Equipment");
        break;
        case 0x402:
            return("IAS Zone");
        break;
        case 0x403:
            return("IAS Warning Device");
        break;
        default:
            return("UndefinedDeviceId");
        break;
    }
}

void Device::SetChangeCallback(DeviceCallback_fn aChanged_CB)
{
    mChanged_CB = aChanged_CB;
}

void Device::HandleDeviceChange(Device * device, Device::Changed_t changeMask)
{
    if (mChanged_CB)
    {
        mChanged_CB(this, (Device::Changed_t) changeMask);
    }
}

void Device::DiscoverNode(m2z_device_params_t* zigbee_node){
    
    bool attrs_to_read = this->NodeReadAllAttributes(zigbee_node);
    if (!attrs_to_read)
    {
        // This node has no attribute discovered, move forward
        zigbee_node->dev_state = READ_ATTRS_COMPLETED;
    }
    while(zigbee_node->dev_state == ANNOUNCE_COMPLETED)
    {
        sleep(1);
    }
    std::string manuf_name = this->GetManufacturer(zigbee_node);
    std::string model_name = this->GetModelIdentifier(zigbee_node);
    std::string location = this->GetLocation(zigbee_node);
    std::string device_id = this->GetEndpointDeviceId(zigbee_node);
    this->SetName((manuf_name + "-" + model_name + "@" + std::to_string(zigbee_node->short_addr)).c_str());
    this->SetLocation(location);
    this->SetEndpointDeviceId(device_id);
}

bool Device::HasCluster(uint16_t zcl_cluster_id)
{
    switch(zcl_cluster_id)
    {
        case ZB_ZCL_CLUSTER_ID_BASIC:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasBasic_x0==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_POWER_CONFIG:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPowerCfg_x1==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_DEVICE_TEMP_CONFIG:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDevTempCfg_x2==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_IDENTIFY:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIdentify_x3==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_GROUPS:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasGroups_x4==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_SCENES:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasScenes_x5==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_ON_OFF:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasOnOff_x6==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_ON_OFF_SWITCH_CONFIG:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasOnOffSwitchCfg_x7==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasLvlCtrl_x8==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_ALARMS:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasAlarms_x9==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_TIME:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasTime_xa==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_RSSI_LOCATION:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasRssiLoc_xb==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_ANALOG_INPUT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasAnalogIn_xc==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasAnalogOut_xd==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_ANALOG_VALUE:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasAnalogValue_xe==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_BINARY_INPUT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasBinaryIn_xf==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_BINARY_OUTPUT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasBinaryOut_x10==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_BINARY_VALUE:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasBinaryValue_x11==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_MULTI_INPUT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMultiIn_x12==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_MULTI_OUTPUT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMultiOut_x13==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_MULTI_VALUE:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMultiValue_x14==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_COMMISSIONING:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasCommissioning_x15==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_OTA_UPGRADE:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasOtaUpgrade_x19==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_POLL_CONTROL:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPollCtrl_x20==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_GREEN_POWER:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasGreenPower_x21==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_KEEP_ALIVE:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasKeepAlive_x25==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_SHADE_CONFIG:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasShadeCfg_x100==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_DOOR_LOCK:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDoorLock_x101==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_WINDOW_COVERING:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasWindowCovering_x102==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_PUMP_CONFIG_CONTROL:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPumpCfgCtrl_x200==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_THERMOSTAT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasThermostat_x201==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_FAN_CONTROL:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasFanCtrl_x202==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_DEHUMID_CONTROL:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDehumidCtr_x203==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_THERMOSTAT_UI_CONFIG:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasThermostatUiCfg_x204==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_COLOR_CONTROL:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasColorCtrl_x300==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasBallastCfg_x301==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_ILLUMINANCE_MEASUREMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIlluminanceMeas_x400==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasTempMeas_x402==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_PRESSURE_MEASUREMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPressureMeas_x403==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasRelHumidityMeas_x405==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_OCCUPANCY_SENSING:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasOccupancySensing_x406==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_CARBON_DIOXIDE_MEASUREMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasCarbonDioMeas_x40D==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_PM2_5_MEASUREMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPM2_5Meas_x42a==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_IAS_ZONE:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIasZone_x501==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_IAS_ACE:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIasAce_x501==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_IAS_WD:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIasWd_x502==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_PRICE:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPrice_x700==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_DRLC:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDRLC_x701==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_METERING:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMetering_x702==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_MESSAGING:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMessaging_x703==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_TUNNELING:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasTunelling_x704==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_PREPAYMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPrepayment_x705==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_ENERGY_MANAGEMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasNrjMngt_x706==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_CALENDAR:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasCalendar_x707==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_DEVICE_MANAGEMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDevMngt_x708==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_EVENTS:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasEvents_x709==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_MDU_PAIRING:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMduPairing_x70a==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_SUB_GHZ:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasSubGhz_x70b==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDaySched_x70d==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasKeyEstblmt_x800==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasAplianceEvAl_xb02==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasElecMeas_xb04==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_DIAGNOSTICS:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDiags_xb05==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_WWAH:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasWwah_xfc57==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_TOUCHLINK_COMMISSIONING:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasTouchCom_x1000==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_TUNNEL:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasTunnel_xfc00==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_IR_BLASTER:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIrBlaster_x1fc01==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_CUSTOM_ATTR:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasCustomAttr_xffee==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_METER_IDENTIFICATION:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMeterId_x0b01==1?true:false);
        break;
        case ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION:
            return(this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDirectCfg_x3d==1?true:false);
        break;
    }
    return(false);
}

std::string Device::GetSupportedClusterString()
{
    std::string supported_clusters="";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasBasic_x0==1?"Bas+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPowerCfg_x1==1?"PoCf+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDevTempCfg_x2==1?"DTCf+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIdentify_x3==1?"Idt+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasGroups_x4==1?"Grps+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasScenes_x5==1?"Scen+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasOnOff_x6==1?"OnOf+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasOnOffSwitchCfg_x7==1?"OOSw+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasLvlCtrl_x8==1?"LvC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasAlarms_x9==1?"Alms+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasTime_xa==1?"Time+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasRssiLoc_xb==1?"RsL+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasAnalogIn_xc==1?"AnI+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasAnalogOut_xd==1?"AnO+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasAnalogValue_xe==1?"AnV+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasBinaryIn_xf==1?"BiI+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasBinaryOut_x10==1?"BiO+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasBinaryValue_x11==1?"BiV+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMultiIn_x12==1?"MuI+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMultiOut_x13==1?"MuO+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMultiValue_x14==1?"MuV+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasCommissioning_x15==1?"Com+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasOtaUpgrade_x19==1?"Ota+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPollCtrl_x20==1?"PlC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasGreenPower_x21==1?"GrP+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasKeepAlive_x25==1?"KAl+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasShadeCfg_x100==1?"ShC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDoorLock_x101==1?"DrL+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasWindowCovering_x102==1?"WiC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPumpCfgCtrl_x200==1?"PCC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasThermostat_x201==1?"Thm+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasFanCtrl_x202==1?"FaC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDehumidCtr_x203==1?"DHC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasThermostatUiCfg_x204==1?"TUC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasColorCtrl_x300==1?"CoC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasBallastCfg_x301==1?"BaC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIlluminanceMeas_x400==1?"Ill+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasTempMeas_x402==1?"Tmp+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPressureMeas_x403==1?"Pre+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasRelHumidityMeas_x405==1?"RHu+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasOccupancySensing_x406==1?"OcS+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasCarbonDioMeas_x40D==1?"CO2+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPM2_5Meas_x42a==1?"PM2+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIasZone_x501==1?"IasZ+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIasAce_x501==1?"IasA+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIasWd_x502==1?"IasW+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPrice_x700==1?"Pric+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDRLC_x701==1?"Drlc+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMetering_x702==1?"Mter+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMessaging_x703==1?"Mess+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasTunelling_x704==1?"Tunl+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasPrepayment_x705==1?"PrP+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasNrjMngt_x706==1?"NrjM+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasCalendar_x707==1?"Cal+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDevMngt_x708==1?"DMngt+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasEvents_x709==1?"Evts+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMduPairing_x70a==1?"MduP+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasSubGhz_x70b==1?"SubG+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDaySched_x70d==1?"DSch+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasKeyEstblmt_x800==1?"KEst+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasAplianceEvAl_xb02==1?"AEA+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasElecMeas_xb04==1?"EleM+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDiags_xb05==1?"Diag+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasWwah_xfc57==1?"Wwah+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasTouchCom_x1000==1?"TchC+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasTunnel_xfc00==1?"Tun+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasIrBlaster_x1fc01==1?"IrB+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasCustomAttr_xffee==1?"CusA+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasMeterId_x0b01==1?"MtId+":"";
    supported_clusters+=this->mBridgeZigbeeRcpNode->supported_clusters.bitmap.hasDirectCfg_x3d==1?"DrctC+":"";

    return(supported_clusters);
}

bool Device::IsReachable()
{
    return mReachable;
}

void Device::SetReachable(bool aReachable)
{
    bool changed = (mReachable != aReachable);

    mReachable = aReachable;

    if (aReachable)
    {
        ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : Device[%s]: ONLINE", mName.c_str());
    }
    else
    {
        ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : Device[%s]: OFFLINE", mName.c_str());
    }

    if (changed)
    {
        HandleDeviceChange(this, kChanged_Reachable);
    }
}

void Device::SetName(std::string szName)
{
    bool changed = (mName.compare(szName) != 0);
    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : Device[%s]: New Name=\"%s\"", mName.c_str(), szName.c_str());
    mName = szName;

    if (changed)
    {
        HandleDeviceChange(this, kChanged_Name);
    }
}

void Device::SetLocation(std::string szLocation)
{
    bool changed = (mLocation.compare(szLocation) != 0);

    mLocation = szLocation;

    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : Device[%s]: Location=\"%s\"", mName.c_str(), mLocation.c_str());

    if (changed)
    {
        HandleDeviceChange(this, kChanged_Location);
    }
}

void Device::SetEndpointDeviceId(std::string szDeviceId)
{
    mEpDeviceId = szDeviceId;
    ChipLogProgress(DeviceLayer, " ---> matter-zigbee-bridge : Device[%s]: DeviceId=\"%s\"", mName.c_str(), mEpDeviceId.c_str());
}

void Device::SetClusters()
{
    mClusters = this->GetSupportedClusterString();
    ep_clusters_count = (int)(this->mBridgeZigbeeRcpNode->endpoints[0].num_in_clusters + this->mBridgeZigbeeRcpNode->endpoints[0].num_out_clusters);
}

void Device::InitState(bool aOn)
{
    mOn     = aOn;
}


void SetEndpointDeviceId()
{
}

EndpointListInfo::EndpointListInfo(uint16_t endpointListId, std::string name, EndpointListTypeEnum type)
{
    mEndpointListId = endpointListId;
    mName           = name;
    mType           = type;
}

EndpointListInfo::EndpointListInfo(uint16_t endpointListId, std::string name, EndpointListTypeEnum type,
                                   chip::EndpointId endpointId)
{
    mEndpointListId = endpointListId;
    mName           = name;
    mType           = type;
    mEndpoints.push_back(endpointId);
}

void EndpointListInfo::AddEndpointId(chip::EndpointId endpointId)
{
    mEndpoints.push_back(endpointId);
}

Room::Room(std::string name, uint16_t endpointListId, EndpointListTypeEnum type, bool isVisible)
{
    mName           = name;
    mEndpointListId = endpointListId;
    mType           = type;
    mIsVisible      = isVisible;
}

Action::Action() {}

Action::Action(uint16_t actionId, std::string name, ActionTypeEnum type, uint16_t endpointListId, uint16_t supportedCommands,
               ActionStateEnum status, bool isVisible)
{
    mActionId          = actionId;
    mName              = name;
    mType              = type;
    mEndpointListId    = endpointListId;
    mSupportedCommands = supportedCommands;
    mStatus            = status;
    mIsVisible         = isVisible;
}
