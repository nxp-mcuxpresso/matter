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

#include "ZigbeeRcpImpl.h"
#include "ZigbeeRcpMessage.h"

pthread_t ZigbeeRcpMonitor_thread;


//// define global variable
//ZcbMsg_t ZcbMsg = {
    //.AnnounceStart = false,
    //.HandleMask    = true,
    //.bridge_mutex  = PTHREAD_MUTEX_INITIALIZER,
    //.bridge_cond   = PTHREAD_COND_INITIALIZER,
    //.msg_type      = BRIDGE_UNKNOW,
//};

/**
 * Global variables definitions
 */
/* Global device context */
m2z_device_ctx_t g_device_ctx;
m2z_device_read_attr_t g_read_attr_req[MATTER_ZIGBEERCP_BRIDGE_MAX_ATTRS_PER_CLUSTER];
zb_ieee_addr_t g_zc_addr = MATTER_ZIGBEERCP_BRIDGE_IEEE_ADDR;
zb_uint8_t g_active_cmds = 0;
typedef void (*callback)(int MsgType, void* ZigbeeRcp_dev, void* data);
m2z_callback_t ZigbeeRcp_msg_callback=NULL;

static const zb_uint8_t g_key_nwk[16] = { 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * Declaring attributes for each cluster
 */

/* Basic cluster attributes */
zb_uint8_t attr_zcl_version  = ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE;
zb_uint8_t attr_power_source = ZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE;
ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST(basic_attr_list, &attr_zcl_version, &attr_power_source);

/* Identify cluster attributes */
zb_uint16_t attr_identify_time = 0;
ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(identify_attr_list, &attr_identify_time);

/* Groups cluster attributes */
zb_uint8_t g_attr_name_support = 0;
ZB_ZCL_DECLARE_GROUPS_ATTRIB_LIST(groups_attr_list, &g_attr_name_support);

/* Scenes cluster attributes */
zb_uint8_t g_attr_scenes_scene_count = ZB_ZCL_SCENES_SCENE_COUNT_DEFAULT_VALUE;
zb_uint8_t g_attr_scenes_current_scene = ZB_ZCL_SCENES_CURRENT_SCENE_DEFAULT_VALUE;
zb_uint16_t g_attr_scenes_current_group = ZB_ZCL_SCENES_CURRENT_GROUP_DEFAULT_VALUE;
zb_uint8_t g_attr_scenes_scene_valid = ZB_ZCL_SCENES_SCENE_VALID_DEFAULT_VALUE;
zb_uint16_t g_attr_scenes_name_support = ZB_ZCL_SCENES_NAME_SUPPORT_DEFAULT_VALUE;
ZB_ZCL_DECLARE_SCENES_ATTRIB_LIST(scenes_attr_list, &g_attr_scenes_scene_count,
    &g_attr_scenes_current_scene, &g_attr_scenes_current_group,
    &g_attr_scenes_scene_valid, &g_attr_scenes_name_support);

/* Declare cluster list for a device */
ZB_HA_DECLARE_MATTER_ZIGBEERCP_BRIDGE_CLUSTER_LIST(m2z_clusters,
                                    basic_attr_list,
                                    identify_attr_list,
                                    groups_attr_list,
                                    scenes_attr_list);

/* Declare endpoint */
ZB_HA_DECLARE_MATTER_ZIGBEERCP_BRIDGE_EP(m2z_ep, MATTER_ZIGBEERCP_BRIDGE_ENDPOINT, m2z_clusters);

/* Declare application's device context for single-endpoint device */
ZB_HA_DECLARE_MATTER_ZIGBEERCP_BRIDGE_CTX(m2z_ctx, m2z_ep);

zb_uint16_t m2z_dev_get_index_by_state(zb_uint8_t dev_state)
{
  zb_uint8_t i;
  for (i = 0; i < ZB_ARRAY_SIZE(g_device_ctx.devices); ++i)
  {
    if (g_device_ctx.devices[i].dev_state == dev_state)
    {
      return i;
    }
  }

  return MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX;
}

zb_uint16_t m2z_dev_get_index_by_short_addr(zb_uint16_t short_addr)
{
  zb_uint16_t i;
  for (i = 0; i < ZB_ARRAY_SIZE(g_device_ctx.devices); ++i)
  {
    if (g_device_ctx.devices[i].short_addr == short_addr)
    {
      return i;
    }
  }

  return MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX;
}

zb_uint16_t m2z_dev_get_index_by_ieee_addr(zb_ieee_addr_t ieee_addr)
{
  zb_uint16_t i;
  for (i = 0; i < ZB_ARRAY_SIZE(g_device_ctx.devices); ++i)
  {
    if (ZB_IEEE_ADDR_CMP(g_device_ctx.devices[i].ieee_addr, ieee_addr))
    {
      return i;
    }
  }

  return MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX;
}

zb_uint16_t m2z_dev_get_ep_idx_by_short_addr_and_ep_id(zb_uint16_t short_addr, zb_uint16_t ep_id)
{
  zb_uint16_t i;
  zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(short_addr);

  for ( i = 0; i < g_device_ctx.devices[dev_idx].endpoint; i++)
  {
    if( g_device_ctx.devices[dev_idx].endpoints[i].ep_id == ep_id){
      return i;
    }
  }

  return MATTER_ZIGBEERCP_BRIDGE_INVALID_EP_INDEX;
}

zb_uint16_t m2z_dev_get_ep_id_by_short_addr_and_cluster_id(zb_uint16_t short_addr, zb_uint16_t cluster_id)
{
  zb_uint16_t i;
  zb_uint16_t endpoint_idx = 0;
  zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(short_addr);

  for ( i = 0; i < g_device_ctx.devices[dev_idx].endpoint; i++)
  {
    for ( zb_uint16_t j = 0; j < (g_device_ctx.devices[dev_idx].endpoints[i].num_in_clusters + g_device_ctx.devices[dev_idx].endpoints[i].num_out_clusters); j++)
    {
      /* Break if a cluster with no know attributes is found */
      if(g_device_ctx.devices[dev_idx].endpoints[i].ep_cluster[j].cluster_id == cluster_id)
      {
          endpoint_idx = g_device_ctx.devices[dev_idx].endpoints[i].ep_id;
          return endpoint_idx;
      }
    }
  }
  return MATTER_ZIGBEERCP_BRIDGE_INVALID_EP_INDEX;
}

zb_uint16_t m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(zb_uint16_t short_addr, zb_uint16_t ep_id, zb_uint16_t cluster_id)
{
    zb_uint16_t cluster_idx = 0;
    zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(short_addr);
    zb_uint16_t ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(short_addr, ep_id);

    for ( zb_uint16_t j = 0; j < (g_device_ctx.devices[dev_idx].endpoints[ep_idx].num_in_clusters + g_device_ctx.devices[dev_idx].endpoints[ep_idx].num_out_clusters); j++)
    {
      /* Break if a cluster with no know attributes is found */
      if(g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[j].cluster_id == cluster_id)
      {
          return j;
      }
    }
    return MATTER_ZIGBEERCP_BRIDGE_INVALID_EP_INDEX;
}

zb_uint16_t m2z_dev_get_attr_idx_by_short_addr_and_ep_id_and_cluster_id(zb_uint16_t short_addr, zb_uint16_t ep_id, zb_uint16_t cluster_id, zb_uint16_t attr_id)
{
    zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(short_addr);
    zb_uint16_t ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(short_addr, ep_id);
    zb_uint16_t cluster_idx = m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(short_addr, ep_id, cluster_id);

    for ( zb_uint16_t j = 0; j < g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].num_attrs; j++)
    {
      /* Break if a cluster with no know attributes is found */
      if(g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[j].attr_id == attr_id)
      {
          return j;
      }
    }
    return MATTER_ZIGBEERCP_BRIDGE_INVALID_EP_INDEX;
}

void m2z_schedule_request_bdb_start_commissioning()
{
      zb_buf_get_out_delayed_ext(m2z_request_bdb_start_commissioning, 0, 0);
}
void m2z_schedule_request_open_network()
{
      zb_buf_get_out_delayed_ext(m2z_request_open_network, 0, 0);

}
void m2z_schedule_request_close_network()
{
      zb_buf_get_out_delayed_ext(m2z_request_close_network, 0, 0);
}

void m2z_request_bdb_start_commissioning(zb_bufid_t param)
{
  WCS_TRACE_DBGREL(">> %s param: %d", __FUNCTION__, param);

  bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);

  WCS_TRACE_DBGREL("<< %s param: %d", __FUNCTION__, param);
}

void m2z_request_open_network(zb_bufid_t param)
{
  zb_nlme_permit_joining_request_t *req;

  WCS_TRACE_DBGREL(">> %s param: %d", __FUNCTION__, param);

  req = zb_buf_initial_alloc(param, sizeof(zb_nlme_permit_joining_request_t));
  req->permit_duration = 180;
  zb_nlme_permit_joining_request(param);

  WCS_TRACE_DBGREL("<< %s param: %d", __FUNCTION__, param);
}

void m2z_request_close_network(zb_bufid_t param)
{
  zb_nlme_permit_joining_request_t *req;

  WCS_TRACE_DBGREL(">> %s param: %d", __FUNCTION__, param);

  req = zb_buf_initial_alloc(param, sizeof(zb_nlme_permit_joining_request_t));
  req->permit_duration = 0;
  zb_nlme_permit_joining_request(param);

  WCS_TRACE_DBGREL("<< %s param: %d", __FUNCTION__, param);
}

void m2z_impl_init()
{
/* Global device context initialization */
    ZB_MEMSET(&g_device_ctx, 0, sizeof(g_device_ctx));

    /* Trace enable */
    ZB_SET_TRACE_ON();

    /* Global ZIGBEERCP initialization */
    ZB_INIT("matter-zigbeercp-bridge");

    /* Set up defaults for the commissioning */
    zb_set_long_address(g_zc_addr);

    zb_set_network_coordinator_role(MATTER_ZIGBEERCP_BRIDGE_CHANNEL_MASK);
    zb_set_nvram_erase_at_start(ZB_FALSE);
    zb_nwk_set_max_ed_capacity(MATTER_ZIGBEERCP_BRIDGE_DEV_NUMBER);
    /* Optional step: Setup predefined nwk key - to easily decrypt ZB sniffer logs which does not
    * contain keys exchange. By default nwk key is randomly generated. */
    /* [zb_secur_setup_preconfigured_key] */
    zb_secur_setup_nwk_key((zb_uint8_t *) g_key_nwk, 0);
    /* [zb_secur_setup_preconfigured_key] */

    /* Register NVRAM application callbacks - to be able to store application data to the special
     dataset. */
    #ifdef ZB_USE_NVRAM
    zb_nvram_register_app1_read_cb(m2z_nvram_read_app_data);
    zb_nvram_register_app1_write_cb(m2z_nvram_write_app_data, m2z_get_nvram_data_size);
    #endif

    #ifdef ZB_SUPPORT_LEGACY_DEVICES
    zb_bdb_set_legacy_device_support(ZB_TRUE);
    #endif

    /* Register device ZCL context */
    ZB_AF_REGISTER_DEVICE_CTX(&m2z_ctx);
    /* Register cluster commands handler for a specific endpoint */
    ZB_AF_SET_ENDPOINT_HANDLER(MATTER_ZIGBEERCP_BRIDGE_ENDPOINT, zcl_specific_cluster_cmd_handler);

    #ifdef ZB_ASSERT_SEND_NWK_REPORT
    zb_register_zboss_callback(ZB_ASSERT_INDICATION_CB, SET_ZBOSS_CB(assert_indication_cb));
    #endif

}

void * m2z_impl_ZigbeeRcpMonitor(void * context)
{
    WCS_TRACE_DBGREL(" ---> matter-zigbee-bridge : BridgeDevMgr::ZigbeeRcpMonitor is running !!!\n");
    if (zboss_start_no_autostart() != RET_OK)
    {
        WCS_TRACE_DBGREL("zboss_start failed");
    }
    else
    {
        /* Call the main loop */
        zboss_main_loop();
    }
    return NULL;
}

int m2z_impl_start_threads()
{
    int res = pthread_create(&ZigbeeRcpMonitor_thread, NULL, m2z_impl_ZigbeeRcpMonitor, NULL);
    if (res)
    {
        WCS_TRACE_DBGREL(" ---> matter-zigbee-bridge : Error creating polling thread: %d\n", res);
        return -1;
    }

    return 0;
}

int m2z_register_message_callback(m2z_callback_t callback_function)
{
    ZigbeeRcp_msg_callback = callback_function;
    return 0;
}

/* Callback which will be called on incoming ZCL packet. */
zb_uint8_t zcl_specific_cluster_cmd_handler(zb_uint8_t param)
{
    zb_bufid_t zcl_cmd_buf = param;
    zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(zcl_cmd_buf, zb_zcl_parsed_hdr_t);
    zb_uint8_t cmd_processed = 0;
    zb_uint16_t dst_addr = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr;
    zb_uint8_t dst_ep = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint;

    WCS_TRACE_DBGREL(">>  %s ", __FUNCTION__);

    ///* Uncomment to use destination address and destination endpoint of the incoming ZCL packet. */
    ///* g_dst_addr = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr; */
    ///* g_endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint; */

    if( cmd_info -> cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV )
    {
        WCS_TRACE_DBGREL("%s: Got ZB_ZCL_FRAME_DIRECTION_TO_SRV from 0x%x -> Endpoint: %d Cluster 0x%04x:(%s) Command %d:(%s)"
            , __FUNCTION__, dst_addr, dst_ep, cmd_info->cluster_id, get_cluster_id_str(cmd_info->cluster_id), cmd_info->cmd_id, get_cmd_id_str(cmd_info->is_common_command, cmd_info->cluster_id,cmd_info->cmd_id));
        switch( cmd_info->cmd_id)
        {
            case ZB_ZCL_CMD_READ_ATTRIB:
            break;

            case ZB_ZCL_CMD_READ_ATTRIB_RESP:
            break;

            case ZB_ZCL_CMD_WRITE_ATTRIB:
            break;

            case ZB_ZCL_CMD_WRITE_ATTRIB_UNDIV:
            break;

            case ZB_ZCL_CMD_WRITE_ATTRIB_NO_RESP:
            break;

            case ZB_ZCL_CMD_CONFIG_REPORT:
            break;

            case ZB_ZCL_CMD_READ_REPORT_CFG:
            break;

            case ZB_ZCL_CMD_REPORT_ATTRIB:
            break;

            case ZB_ZCL_CMD_DISC_ATTRIB:
            break;

            case ZB_ZCL_CMD_DEFAULT_RESP:
                //m2z_handler_disc_attr_resp(zcl_cmd_buf, cmd_info);
            break;

            default:
                WCS_TRACE_DBGREL("ZB_ZCL_FRAME_DIRECTION_TO_SRV: Skip general command %hd", cmd_info->cmd_id);
            break;
            }
    }
    else if (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_CLI)
    {
        WCS_TRACE_DBGREL("%s: Got ZB_ZCL_FRAME_DIRECTION_TO_CLI from 0x%x -> Endpoint: %d Cluster 0x%04x:(%s) Command %hd:(%s)"
            , __FUNCTION__, dst_addr, dst_ep, cmd_info->cluster_id, get_cluster_id_str(cmd_info->cluster_id), cmd_info->cmd_id, get_cmd_id_str(cmd_info->is_common_command, cmd_info->cluster_id, cmd_info->cmd_id));
        switch( cmd_info->cmd_id)
        {
            case ZB_ZCL_CMD_READ_ATTRIB_RESP:
                zb_zcl_read_attr_res_t * read_attr_resp = NULL;
                zb_uint16_t dev_idx = 0;
                zb_uint16_t ep_idx = 0;
                zb_uint16_t cluster_idx = 0;
                zb_uint16_t attr_idx = 0;
                ZB_ZCL_GENERAL_GET_NEXT_READ_ATTR_RES(param, read_attr_resp);
                TRACE_MSG(TRACE_APP3, "read_attr_resp %p", (FMT__P, read_attr_resp));
                if (read_attr_resp)
                {
                  if (ZB_ZCL_STATUS_SUCCESS == read_attr_resp->status)
                  {
                      zb_uint8_t attr_size = zb_zcl_get_attribute_size(read_attr_resp->attr_type, &(read_attr_resp->attr_value[0]));
                      zb_uint8_t attr_array[32];
                      WCS_TRACE_DBGREL("%s: Got %s -> Cluster 0x%04x:(%s) attr_id: 0x%04x status: %d attr_type: 0x%04x attr_size: %d attr_value[0]: 0x%x"
                        , __FUNCTION__, get_cmd_id_str(cmd_info->is_common_command, cmd_info->cluster_id, cmd_info->cmd_id), cmd_info->cluster_id, get_cluster_id_str(cmd_info->cluster_id), read_attr_resp->attr_id, read_attr_resp->status, read_attr_resp->attr_type, attr_size, read_attr_resp->attr_value[0]);
                      dev_idx = m2z_dev_get_index_by_short_addr(dst_addr);
                      ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(dst_addr, dst_ep);
                      cluster_idx = m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(dst_addr, dst_ep, cmd_info->cluster_id);
                      attr_idx = m2z_dev_get_attr_idx_by_short_addr_and_ep_id_and_cluster_id(dst_addr, dst_ep, cmd_info->cluster_id, read_attr_resp->attr_id);
                      
                      g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].disc_attrs_state = KNOWN_CLUSTER;
                      g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].attr_id = read_attr_resp->attr_id;
                      g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].attr_type = read_attr_resp->attr_type;
                      g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].attr_state = GOT_VALUE_ATTR;
                      ZB_MEMCPY(g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].attr_value_array, &(read_attr_resp->attr_value[0]), attr_size);
                  }
                }
                else
                {
                  WCS_TRACE_DBGREL("ERROR, No info on attribute(s) read");
                }
                
                if(g_device_ctx.devices[dev_idx].dev_state == ANNOUNCE_COMPLETED)
                {
                    m2z_device_ep_t *endpoint = NULL;
                    m2z_device_cluster_t *cluster = NULL;
                    m2z_device_cluster_attr_t *attribute = NULL;
                    uint8_t read_attrs_done=1;
                    // check if all the attributes were successfully read
                    for(ep_idx=0; ep_idx < g_device_ctx.devices[dev_idx].endpoint; ep_idx++)
                    {
                        endpoint=(m2z_device_ep_t *)(&(g_device_ctx.devices[dev_idx].endpoints[ep_idx]));
                        for(cluster_idx=0; cluster_idx < (endpoint->num_in_clusters + endpoint->num_out_clusters); cluster_idx++)
                        {
                            cluster = &(endpoint->ep_cluster[cluster_idx]);
                            for (attr_idx=0; attr_idx < cluster->num_attrs; attr_idx++)
                            {
                                attribute = &(cluster->attribute[attr_idx]);
                                if(attribute->attr_state == REQUESTED_VALUE_READ_ATTR)
                                {
                                    read_attrs_done=0;
                                    //WCS_TRACE_DBGREL("%s: Endpoint: %d Cluster: %d attr_id: %d attr_state: %d", __FUNCTION__, ep_idx, cluster_idx, attr_idx, attribute->attr_state);
                                    break;
                                }
                            }
                            if(read_attrs_done==0)
                            {
                                break;
                            }
                        }
                        if(read_attrs_done==0)
                        {
                            break;
                        }
                    }
                    if(read_attrs_done==1)
                    {
                        g_device_ctx.devices[dev_idx].dev_state = READ_ATTRS_COMPLETED;
                    }
                }
            break;

            case ZB_ZCL_CMD_WRITE_ATTRIB_RESP:
                zb_zcl_write_attr_res_t * write_attr_resp = NULL;
                //zb_uint16_t dev_idx = 0;
                //zb_uint16_t ep_idx = 0;
                //zb_uint16_t cluster_idx = 0;
                //zb_uint16_t attr_idx = 0;
                ZB_ZCL_GET_NEXT_WRITE_ATTR_RES(param, write_attr_resp);
                TRACE_MSG(TRACE_APP3, "write_attr_resp %p", (FMT__P, write_attr_resp));
                if (write_attr_resp)
                {
                    WCS_TRACE_DBGREL("%s: Got %s -> Cluster 0x%04x:(%s) attr_id: 0x%04x status: 0x%x "
                    , __FUNCTION__, get_cmd_id_str(cmd_info->is_common_command, cmd_info->cluster_id, cmd_info->cmd_id), cmd_info->cluster_id, get_cluster_id_str(cmd_info->cluster_id), write_attr_resp->attr_id, write_attr_resp->status);
                    dev_idx = m2z_dev_get_index_by_short_addr(dst_addr);
                    ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(dst_addr, dst_ep);
                    cluster_idx = m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(dst_addr, dst_ep, cmd_info->cluster_id);
                    attr_idx = m2z_dev_get_attr_idx_by_short_addr_and_ep_id_and_cluster_id(dst_addr, dst_ep, cmd_info->cluster_id, write_attr_resp->attr_id);

                    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].attr_zb_status = write_attr_resp->status;

                    if (ZB_ZCL_STATUS_SUCCESS != write_attr_resp->status)
                    {
                        WCS_TRACE_DBGREL("%s: -> Dev: %d Endpoint: %d Cluster 0x%04x:(%s) attr_id: 0x%04x status: 0x%x -> IMPOSSIBLE TO BE WRITTEN !!!"
                            , __FUNCTION__, dev_idx, ep_idx, cmd_info->cluster_id, get_cluster_id_str(cmd_info->cluster_id), write_attr_resp->attr_id, write_attr_resp->status);
                    }
                }
                else
                {
                  WCS_TRACE_DBGREL("ERROR, No info on attribute(s) write");
                }

            case ZB_ZCL_CMD_WRITE_ATTRIB_NO_RESP:
            break;

            case ZB_ZCL_CMD_CONFIG_REPORT_RESP:
            break;

            case ZB_ZCL_CMD_READ_REPORT_CFG_RESP:
            break;

            case ZB_ZCL_CMD_REPORT_ATTRIB:
            break;

            case ZB_ZCL_CMD_DEFAULT_RESP:
                zb_zcl_default_resp_payload_t *default_resp;
                default_resp = ZB_ZCL_READ_DEFAULT_RESP(param);
                dev_idx = m2z_dev_get_index_by_short_addr(dst_addr);
                ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(dst_addr, dst_ep);
                cluster_idx = m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(dst_addr, dst_ep, cmd_info->cluster_id);
                g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].pending_command = false;

                WCS_TRACE_DBGREL("%s: Got %s -> Dev: %d Endpoint: %d Cluster 0x%04x:(%s) command_id 0x%x status: 0x%x"
                    , __FUNCTION__, get_cmd_id_str(cmd_info->is_common_command, cmd_info->cluster_id, cmd_info->cmd_id), dev_idx, ep_idx, cmd_info->cluster_id, get_cluster_id_str(cmd_info->cluster_id), default_resp->command_id, default_resp->status);
                
            break;

            case ZB_ZCL_CMD_DISC_ATTRIB_RESP:
                m2z_handler_disc_attr_resp(zcl_cmd_buf, cmd_info);
            break;

            default:
                if(cmd_info->is_common_command == false)
                {
                    //Cluster-specific Command response received, no data structure to parse it...
                    // just set to false the pending_command flag for this Cluster...
                    dev_idx = m2z_dev_get_index_by_short_addr(dst_addr);
                    ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(dst_addr, dst_ep);
                    cluster_idx = m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(dst_addr, dst_ep, cmd_info->cluster_id);
                    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].pending_command = false;

                    WCS_TRACE_DBGREL("%s: Got CLUSTER-SPECIFIC Command Response -> Dev: %d Endpoint: %d Cluster 0x%04x:(%s) command_id 0x%x"
                        , __FUNCTION__, dev_idx, ep_idx, cmd_info->cluster_id, get_cluster_id_str(cmd_info->cluster_id), cmd_info->cmd_id);
                }
                else
                {
                    WCS_TRACE_DBGREL("ZB_ZCL_FRAME_DIRECTION_TO_CLI: Skip general command %hd", cmd_info->cmd_id);
                }
            break;
            }
    }

    WCS_TRACE_DBGREL("< zcl_specific_cluster_cmd_handler");
    return cmd_processed;
}

/* Callback which will be called on startup procedure complete (successful or not). */
void zboss_signal_handler(zb_uint8_t param)
{
    zb_zdo_app_signal_hdr_t * p_sg_p = NULL;
    zb_zdo_signal_leave_params_t * p_leave_params = NULL;
    zb_zdo_app_signal_type_t sig = zb_get_app_signal(param, &p_sg_p);
    zb_ret_t status = ZB_GET_APP_SIGNAL_STATUS(param);
  if (status == 0)
  {
    WCS_TRACE_DBGREL(">> %s param %hd ", __FUNCTION__, param);
    switch(sig)
    {
#ifdef DEBUG
      case ZB_DEBUG_SIGNAL_TCLK_READY:
      {
        zb_debug_signal_tclk_ready_params_t *params = ZB_ZDO_SIGNAL_GET_PARAMS(p_sg_p, zb_debug_signal_tclk_ready_params_t);
        zb_debug_broadcast_aps_key(params->long_addr);
      }
      break;
#endif
      case ZB_ZDO_SIGNAL_SKIP_STARTUP:
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_ZDO_SIGNAL_SKIP_STARTUP", __FUNCTION__, sig);
#ifndef ZB_MACSPLIT_HOST
        zboss_start_continue();
#endif /* ZB_MACSPLIT_HOST */
        break;

#ifdef ZB_MACSPLIT_HOST
      case ZB_MACSPLIT_DEVICE_BOOT:
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_MACSPLIT_DEVICE_BOOT", __FUNCTION__, sig);
          zb_zdo_signal_macsplit_dev_boot_params_t *boot_params = ZB_ZDO_SIGNAL_GET_PARAMS(p_sg_p, zb_zdo_signal_macsplit_dev_boot_params_t);
          WCS_TRACE_DBGREL("%s: ZB_MACSPLIT_DEVICE_BOOT dev version %d", __FUNCTION__, boot_params->dev_version);
        break;
#endif /* ZB_MACSPLIT_HOST */

      case ZB_ZDO_SIGNAL_DEFAULT_START:
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_ZDO_SIGNAL_DEFAULT_START", __FUNCTION__, sig);
      case ZB_BDB_SIGNAL_DEVICE_FIRST_START:
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_BDB_SIGNAL_DEVICE_FIRST_START", __FUNCTION__, sig);
      case ZB_BDB_SIGNAL_DEVICE_REBOOT:
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_BDB_SIGNAL_DEVICE_REBOOT", __FUNCTION__, sig);
        break;

      case ZB_BDB_SIGNAL_STEERING:
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_BDB_SIGNAL_STEERING: Successful steering", __FUNCTION__, sig);
        break;

      case ZB_BDB_SIGNAL_FORMATION:
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_BDB_SIGNAL_FORMATION: Successful formation", __FUNCTION__, sig);
        break;

      case ZB_TCSWAP_DB_BACKUP_REQUIRED_SIGNAL:
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_TCSWAP_DB_BACKUP_REQUIRED_SIGNAL", __FUNCTION__, sig);
        break;

      case ZB_NWK_SIGNAL_PERMIT_JOIN_STATUS:
      {
        zb_uint8_t *permit_duration = ZB_ZDO_SIGNAL_GET_PARAMS(p_sg_p, zb_uint8_t);
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_NWK_SIGNAL_PERMIT_JOIN_STATUS: Duration: %hd", __FUNCTION__, sig, (*permit_duration));
        WCS_TRACE_NOTICE("Permit Join Received. Duration: %hd", (*permit_duration));
        //zb_uint8_t *permit_duration = ZB_ZDO_SIGNAL_GET_PARAMS((zb_zdo_app_signal_hdr_t *)p_sg_p, zb_uint8_t);
        //WCS_TRACE_NOTICE("NWK PERMIT_JOIN_STATUS, duration: %d",  *permit_duration);
        break; /* ZB_NWK_SIGNAL_PERMIT_JOIN_STATUS */
      }

/* [signal_leave_ind] */
      case ZB_ZDO_SIGNAL_LEAVE_INDICATION:
      {
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_ZDO_SIGNAL_LEAVE_INDICATION", __FUNCTION__, sig);
        zb_zdo_signal_leave_indication_params_t *leave_ind_params = ZB_ZDO_SIGNAL_GET_PARAMS(p_sg_p, zb_zdo_signal_leave_indication_params_t);
        if (!leave_ind_params->rejoin)
        {
          m2z_callback_dev_leave_indication(leave_ind_params->device_addr);
        }
      }
      break;
/* [signal_device_annce] */
      case ZB_ZDO_SIGNAL_DEVICE_ANNCE:
      {
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_ZDO_SIGNAL_DEVICE_ANNCE", __FUNCTION__, sig);
        zb_zdo_signal_device_annce_params_t *dev_annce_params = ZB_ZDO_SIGNAL_GET_PARAMS(p_sg_p, zb_zdo_signal_device_annce_params_t);
        m2z_callback_dev_annce(dev_annce_params);
      }
      break;
/* [signal_device_authorized] */
      case ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED:
      {
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED", __FUNCTION__, sig);
        zb_zdo_signal_device_authorized_params_t *dev_authorized_params = ZB_ZDO_SIGNAL_GET_PARAMS(p_sg_p, zb_zdo_signal_device_authorized_params_t);
        m2z_callback_dev_authorized(dev_authorized_params);
      }
      break;
/* [signal_device_associated] */
      case ZB_NWK_SIGNAL_DEVICE_ASSOCIATED:
      {
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_NWK_SIGNAL_DEVICE_ASSOCIATED", __FUNCTION__, sig);
        zb_nwk_signal_device_associated_params_t *dev_associated_params = ZB_ZDO_SIGNAL_GET_PARAMS(p_sg_p, zb_nwk_signal_device_associated_params_t);
        m2z_callback_dev_associated(dev_associated_params);
      }
      break;
/* [signal_device_update] */
      case ZB_ZDO_SIGNAL_DEVICE_UPDATE:
      {
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_ZDO_SIGNAL_DEVICE_UPDATE", __FUNCTION__, sig);
        zb_zdo_signal_device_update_params_t *dev_update_params = ZB_ZDO_SIGNAL_GET_PARAMS(p_sg_p, zb_zdo_signal_device_update_params_t);
        m2z_callback_dev_updated(dev_update_params);
      }
      break;
/* [signal_device_update] */
      case ZB_NLME_STATUS_INDICATION:
      {
        WCS_TRACE_DBGREL("%s: signal %02u -> ZB_NLME_STATUS_INDICATION", __FUNCTION__, sig);
        zb_zdo_signal_nlme_status_indication_params_t *dev_status_indication_params = ZB_ZDO_SIGNAL_GET_PARAMS(p_sg_p, zb_zdo_signal_nlme_status_indication_params_t);
        //m2z_dev_status_indication_cb(dev_status_indication_params);
      }
      break;
/* [not implemented] */
      default:
        WCS_TRACE_DBGREL("Not Implemented signal: %02u", sig);
    }
  }
  else if (sig == ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY)
  {
    WCS_TRACE_DBGREL("Production config is not present or invalid");
  }
  else
  {
    WCS_TRACE_DBGREL("Device started FAILED status %d", ZB_GET_APP_SIGNAL_STATUS(param));
  }

  if (param)
  {
    zb_buf_free(param);
  }
  WCS_TRACE_DBGREL("<< %s param %hd ", __FUNCTION__, param);
}

void m2z_remove_device(zb_uint8_t idx)
{
  ZB_BZERO(&g_device_ctx.devices[idx], sizeof(m2z_device_params_t));
#ifdef ZB_USE_NVRAM
  /* If we fail, trace is given and assertion is triggered */
  (void)zb_nvram_write_dataset(ZB_NVRAM_APP_DATA1);
#endif
}

/* [address_by_short] */
void m2z_request_active_ep_leave(zb_bufid_t param, zb_uint16_t short_addr, zb_bool_t rejoin_flag)
{
    zb_bufid_t buf = param;
    zb_zdo_mgmt_leave_param_t *req_param;
    zb_address_ieee_ref_t addr_ref;

    if (zb_address_by_short(short_addr, ZB_FALSE, ZB_FALSE, &addr_ref) == RET_OK)
    {
      req_param = ZB_BUF_GET_PARAM(buf, zb_zdo_mgmt_leave_param_t);
      ZB_BZERO(req_param, sizeof(zb_zdo_mgmt_leave_param_t));

      req_param->dst_addr = short_addr;
      req_param->rejoin = (rejoin_flag ? 1 : 0);
      zdo_mgmt_leave_req(param, NULL);
    }
    else
    {
      WCS_TRACE_DBGREL("tried to remove 0x%xd, but device is already left", short_addr);
      zb_buf_free(buf);
    }
  }
/* [address_by_short] */


void m2z_leave_and_rejoin_device(zb_uint8_t param, zb_uint16_t short_addr)
{
  WCS_TRACE_DBGREL(">> %s param %hd short_addr %d",__FUNCTION__, param, short_addr);

  if (!param)
  {
    zb_buf_get_out_delayed_ext(m2z_leave_and_rejoin_device, short_addr, 0);
  }
  else
  {
    m2z_request_active_ep_leave(param, short_addr, ZB_TRUE);
  }

  WCS_TRACE_DBGREL("<< %s",__FUNCTION__);
}


void m2z_remove_and_rejoin_device_delayed(zb_uint8_t idx)
{
  WCS_TRACE_DBGREL("%s: short_addr 0x%x",__FUNCTION__,  g_device_ctx.devices[idx].short_addr);

  zb_buf_get_out_delayed_ext(m2z_leave_and_rejoin_device, g_device_ctx.devices[idx].short_addr, 0);
  m2z_remove_device(idx);
}

/* Callback which will be called on Device Associated packet. */
void m2z_callback_dev_associated(zb_nwk_signal_device_associated_params_t *dev_associated_params)
{
    WCS_TRACE_DBGREL(">> %s ", __FUNCTION__);
    WCS_TRACE_DBGREL("<<  %s ", __FUNCTION__);
}

/* Callback which will be called on Device Updated packet. */
void m2z_callback_dev_updated(zb_zdo_signal_device_update_params_t *dev_updated_params)
{
    WCS_TRACE_DBGREL(">> %s ", __FUNCTION__);
    WCS_TRACE_DBGREL("Short Addr: %d Status: %d tc_action: %d parent_short: %d", dev_updated_params->short_addr \
                                    , dev_updated_params->status \
                                    , dev_updated_params->tc_action \
                                    , dev_updated_params->parent_short \
                                    );
    zb_uint16_t idx = m2z_dev_get_index_by_ieee_addr(dev_updated_params->long_addr);
    if (idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX)
    {
        WCS_TRACE_DBGREL("It is known device, §,,,,");
    }
    WCS_TRACE_DBGREL("<<  %s ", __FUNCTION__);
}

/* Callback which will be called on incoming Device Announce packet. */
void m2z_callback_dev_annce(zb_zdo_signal_device_annce_params_t *device_annce_params)
{
    WCS_TRACE_DBGREL(">> %s ", __FUNCTION__);

    zb_uint16_t idx = m2z_dev_get_index_by_ieee_addr(device_annce_params->ieee_addr);
    if (idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX)
    {
        WCS_TRACE_DBGREL("It is known device, but it attempts to associate, strange...");
    }
    else
    {
        /* It is a new device, save its ieee_addr */
        /* Ok, device is unknown, add to dev list. */
        idx = m2z_dev_get_index_by_state(NO_DEVICE);
        if (idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX)
        {
            g_device_ctx.devices[idx].short_addr = device_annce_params->device_short_addr;
            ZB_IEEE_ADDR_COPY(g_device_ctx.devices[idx].ieee_addr, device_annce_params->ieee_addr);
            ZB_SCHEDULE_APP_ALARM(m2z_schedule_request_active_ep, (uint8_t)idx, 1 * ZB_TIME_ONE_SECOND);
            g_device_ctx.devices[idx].dev_state = COMPLETED;
            g_device_ctx.devices[idx].announced_to_bridge = 0;
        }
    }
    WCS_TRACE_DBGREL("<<  %s ", __FUNCTION__);
}

/* Callback which will be called on Device Authorized packet. */
void m2z_callback_dev_authorized(zb_zdo_signal_device_authorized_params_t *dev_authorized_params)
{
    WCS_TRACE_DBGREL(">> %s ", __FUNCTION__);
    zb_uint16_t idx = m2z_dev_get_index_by_ieee_addr(dev_authorized_params->long_addr);

    if (idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX)
    {
        g_device_ctx.devices[idx].authorization_type = dev_authorized_params->authorization_type;
        g_device_ctx.devices[idx].authorization_status = dev_authorized_params->authorization_status;
        g_device_ctx.devices[idx].announced_to_bridge = 0;
    }
    WCS_TRACE_DBGREL("<<  %s ", __FUNCTION__);
}

void m2z_callback_dev_ieee_addr(zb_uint8_t param)
{
  zb_bufid_t buf = param;
  zb_zdo_nwk_addr_resp_head_t *resp;
  zb_ieee_addr_t ieee_addr;
  zb_uint16_t nwk_addr;
  zb_uint16_t dev_idx;

  WCS_TRACE_DBGREL(">> %s param %hd", __FUNCTION__, param);

  resp = (zb_zdo_nwk_addr_resp_head_t*)zb_buf_begin(buf);
  WCS_TRACE_DBGREL("resp status %hd, nwk addr %d", resp->status, resp->nwk_addr);

  if (resp->status == ZB_ZDP_STATUS_SUCCESS)
  {
    ZB_LETOH64(ieee_addr, resp->ieee_addr);
    ZB_LETOH16(&nwk_addr, &resp->nwk_addr);

    dev_idx = m2z_dev_get_index_by_short_addr(nwk_addr);

    if (dev_idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX)
    {
      ZB_MEMCPY(g_device_ctx.devices[dev_idx].ieee_addr, ieee_addr, sizeof(zb_ieee_addr_t));
      g_device_ctx.devices[dev_idx].dev_state = COMPLETED;

      ZB_SCHEDULE_APP_ALARM(m2z_schedule_request_active_ep, (uint8_t)dev_idx, 1 * ZB_TIME_ONE_SECOND);

    }
    else
    {
      WCS_TRACE_DBGREL("This resp is not for our device");
    }
  }

  if (param)
  {
    zb_buf_free(buf);
  }

  WCS_TRACE_DBGREL("<< %s", __FUNCTION__);
}

void m2z_schedule_request_ieee_addr(zb_uint16_t dev_idx) {
    zb_buf_get_out_delayed_ext(m2z_request_ieee_addr, dev_idx, 0);
}
void m2z_request_ieee_addr(zb_bufid_t param, zb_uint16_t dev_idx)
{
  zb_bufid_t  buf = param;
  zb_zdo_ieee_addr_req_param_t *req_param;
  WCS_TRACE_DBGREL(">> %s param %hd dev_idx %hd", __FUNCTION__, param, dev_idx);

  if (!param){
    zb_buf_get_out_delayed_ext((zb_callback2_t)m2z_request_ieee_addr, dev_idx, 0);
  }else
  {
    if (dev_idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX)
    {
      req_param = ZB_BUF_GET_PARAM(buf, zb_zdo_ieee_addr_req_param_t);

      req_param->nwk_addr = g_device_ctx.devices[dev_idx].short_addr;
      req_param->dst_addr = req_param->nwk_addr;
      req_param->start_index = 0;
      req_param->request_type = 0;
      zb_zdo_ieee_addr_req(buf, m2z_callback_dev_ieee_addr);
    }
    else
    {
      WCS_TRACE_DBGREL("No devices in discovery state were found!");
      zb_buf_free(buf);
    }
  }
  WCS_TRACE_DBGREL("<< %s param %hd dev_idx %hd", __FUNCTION__, param, dev_idx);
}

void m2z_handler_disc_attr_resp(zb_bufid_t param, zb_zcl_parsed_hdr_t *cmd_info)
{
    zb_zcl_disc_attr_info_t *disc_attr_info;
    zb_uint8_t complete;
    m2z_device_cluster_t *ep_cluster = NULL;
    m2z_device_ep_t *ep;
    zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr);
    zb_uint16_t ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
                                                                   ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint);

    WCS_TRACE_DBGREL(">> %s", __FUNCTION__);
    if (dev_idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX && ep_idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_EP_INDEX)
    {
        ep = (m2z_device_ep_t *)(&(g_device_ctx.devices[dev_idx].endpoints[ep_idx]));

        for (zb_uint16_t i = 0; i < (ep->num_in_clusters + ep->num_out_clusters); i++)
        {
          if( ep->ep_cluster[i].cluster_id == cmd_info->cluster_id)
          {
            ep_cluster = &ep->ep_cluster[i];
            break;
          }
        }

        if(ep_cluster)
        {
            ZB_ZCL_GENERAL_GET_COMPLETE_DISC_RES(param, complete);
            WCS_TRACE_DBGREL(">> %s complete: %d", __FUNCTION__, complete);

            ZB_ZCL_GENERAL_GET_NEXT_DISC_ATTR_RES(param, disc_attr_info);
            while(disc_attr_info)
            {
                WCS_TRACE_DBGREL("-> Id: 0x%x - Data Type: 0x%x", disc_attr_info->attr_id, disc_attr_info->data_type);

                if (ep_cluster->num_attrs < ZB_ARRAY_SIZE(ep_cluster->attribute))
                {
                    ep_cluster->attribute[ep_cluster->num_attrs].attr_state = DISCOVERED_ATTR;
                    ep_cluster->attribute[ep_cluster->num_attrs].attr_id = disc_attr_info->attr_id;
                    ep_cluster->attribute[ep_cluster->num_attrs].attr_type = disc_attr_info->data_type;
                    
                    ep_cluster->attribute[ep_cluster->num_attrs].read_req.dev_idx = dev_idx;
                    ep_cluster->attribute[ep_cluster->num_attrs].read_req.zed_ep = ep_idx;
                    ep_cluster->attribute[ep_cluster->num_attrs].read_req.zc_ep = MATTER_ZIGBEERCP_BRIDGE_ENDPOINT;
                    ep_cluster->attribute[ep_cluster->num_attrs].read_req.cluster_id = cmd_info->cluster_id;
                    ep_cluster->attribute[ep_cluster->num_attrs].read_req.attr_id = disc_attr_info->attr_id;
                    
                    ep_cluster->attribute[ep_cluster->num_attrs].write_req.dev_idx = dev_idx;
                    ep_cluster->attribute[ep_cluster->num_attrs].write_req.zed_ep = ep_idx;
                    ep_cluster->attribute[ep_cluster->num_attrs].write_req.zc_ep = MATTER_ZIGBEERCP_BRIDGE_ENDPOINT;
                    ep_cluster->attribute[ep_cluster->num_attrs].write_req.cluster_id = cmd_info->cluster_id;
                    ep_cluster->attribute[ep_cluster->num_attrs].write_req.attr_id = disc_attr_info->attr_id;
                    ep_cluster->attribute[ep_cluster->num_attrs].write_req.attr_id = disc_attr_info->attr_id;
                    ep_cluster->attribute[ep_cluster->num_attrs].write_req.attr_type = disc_attr_info->data_type;
                    ep_cluster->num_attrs++;
                    
                }            
                ZB_ZCL_GENERAL_GET_NEXT_DISC_ATTR_RES(param, disc_attr_info);
            }

        }
    }
    WCS_TRACE_DBGREL("<< %s", __FUNCTION__);
}

void m2z_schedule_read_attribute(m2z_device_params_t *dev, zb_uint16_t cluster_id, zb_uint16_t attr_id)
{
    zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(dev->short_addr);
    zb_uint16_t ep_id = m2z_dev_get_ep_id_by_short_addr_and_cluster_id(dev->short_addr, cluster_id);
    zb_uint16_t ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(dev->short_addr, ep_id);
    zb_uint16_t cluster_idx = m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(dev->short_addr, ep_id, cluster_id);
    zb_uint16_t attr_idx = m2z_dev_get_attr_idx_by_short_addr_and_ep_id_and_cluster_id(dev->short_addr, ep_id, cluster_id, attr_id);
    
    if(attr_idx == MATTER_ZIGBEERCP_BRIDGE_INVALID_EP_INDEX)
    {
        // it is the first time this attribute is requested to be read
        attr_idx = g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].num_attrs;
        g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].num_attrs += 1;
    }
        
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].read_req.dev_idx = dev_idx;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].read_req.zed_ep = ep_id;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].read_req.zc_ep = MATTER_ZIGBEERCP_BRIDGE_ENDPOINT;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].read_req.cluster_id = cluster_id;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].read_req.attr_id = attr_id;
    
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].attr_state = SCHEDULED_VALUE_READ_ATTR;
    zb_buf_get_out_delayed_ext((zb_callback2_t)m2z_request_read_attribute, dev_idx, 0);
}

void m2z_schedule_write_attribute(m2z_device_params_t *dev, zb_uint16_t cluster_id, zb_uint16_t attr_id, zb_uint8_t * p_attr_value)
{
    zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(dev->short_addr);
    zb_uint16_t ep_id = m2z_dev_get_ep_id_by_short_addr_and_cluster_id(dev->short_addr, cluster_id);
    zb_uint16_t ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(dev->short_addr, ep_id);
    zb_uint16_t cluster_idx = m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(dev->short_addr, ep_id, cluster_id);
    zb_uint16_t attr_idx = m2z_dev_get_attr_idx_by_short_addr_and_ep_id_and_cluster_id(dev->short_addr, ep_id, cluster_id, attr_id);
    
    if(attr_idx == MATTER_ZIGBEERCP_BRIDGE_INVALID_EP_INDEX)
    {
        // it is the first time this attribute is requested to be read
        attr_idx = g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].num_attrs;
        g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].num_attrs += 1;
    }
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].write_req.dev_idx = dev_idx;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].write_req.zed_ep = ep_id;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].write_req.zc_ep = MATTER_ZIGBEERCP_BRIDGE_ENDPOINT;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].write_req.cluster_id = cluster_id;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].write_req.attr_id = attr_id;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].write_req.attr_type = g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].attr_type;
    ZB_MEMCPY(g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].write_req.write_value, p_attr_value,sizeof(zb_uint8_t));
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].pending_write = true;
    zb_buf_get_out_delayed_ext((zb_callback2_t)m2z_request_write_attribute, dev_idx, 0);
}

void m2z_schedule_send_command(m2z_device_params_t *dev, zb_uint16_t cluster_id, zb_uint16_t cmd_id, zb_uint8_t *cmd_data, zb_uint8_t cmd_size)
{
    WCS_TRACE_DBGREL(">> %s ", __FUNCTION__);
    zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(dev->short_addr);
    zb_uint16_t ep_id = m2z_dev_get_ep_id_by_short_addr_and_cluster_id(dev->short_addr, cluster_id);
    zb_uint16_t ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(dev->short_addr, ep_id);
    zb_uint16_t cluster_idx = m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(dev->short_addr, ep_id, cluster_id);
    
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].command_req.dev_idx = dev_idx;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].command_req.zed_ep = ep_id;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].command_req.zc_ep = MATTER_ZIGBEERCP_BRIDGE_ENDPOINT;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].command_req.cluster_id = cluster_id;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].command_req.cmd_id = cmd_id;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].pending_command = true;
    g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].command_req.cmd_size = cmd_size;
    memcpy(g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].command_req.cmd_data, cmd_data, cmd_size);
    
    zb_buf_get_out_delayed_ext((zb_callback2_t)m2z_request_send_command, dev_idx, 0);
    while(g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].pending_command)
    {
        usleep(100);
    };
    usleep(COMMAND_COMPLETED_TIME_MS * 1000);
    WCS_TRACE_DBGREL("<< %s ", __FUNCTION__);

}

/* Send_read_attribute_req */
void m2z_request_read_attribute(zb_bufid_t param, zb_uint16_t dev_index)
{
  WCS_TRACE_DBGREL(">> %s param %hd dev_idx %hd", __FUNCTION__, param, dev_index);
    
    m2z_device_ep_t *endpoint = NULL;
    m2z_device_cluster_t *cluster = NULL;
    m2z_device_cluster_attr_t *attribute = NULL;
    
    for(zb_uint16_t ep_idx=0; ep_idx < g_device_ctx.devices[dev_index].endpoint; ep_idx++)
    {
        endpoint=(m2z_device_ep_t *)(&(g_device_ctx.devices[dev_index].endpoints[ep_idx]));
        for(zb_uint16_t cluster_idx=0; cluster_idx < (endpoint->num_in_clusters + endpoint->num_out_clusters); cluster_idx++)
        {
            cluster = &(endpoint->ep_cluster[cluster_idx]);
            for (zb_uint16_t attr_idx=0; attr_idx < cluster->num_attrs; attr_idx++)
            {
                attribute = &(cluster->attribute[attr_idx]);
                if(attribute->attr_state == SCHEDULED_VALUE_READ_ATTR)
                {
                    WCS_TRACE_DBGREL(">> %s -> dev_idx: %d zc_ep: %d zed_ep: %d cluster 0x%04x:(%s) attr_id: %d ", __FUNCTION__, dev_index, attribute->read_req.zc_ep
                    , attribute->read_req.zed_ep, attribute->read_req.cluster_id, get_cluster_id_str(attribute->read_req.cluster_id), attribute->read_req.attr_id);
                    attribute->attr_state = REQUESTED_VALUE_READ_ATTR;
                    
                    zb_uint8_t *cmd_ptr;
                    ZB_ZCL_GENERAL_INIT_READ_ATTR_REQ((param), cmd_ptr, ZB_ZCL_DISABLE_DEFAULT_RESPONSE);
                    ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, attribute->read_req.attr_id);
                    ZB_ZCL_GENERAL_SEND_READ_ATTR_REQ((param), cmd_ptr, g_device_ctx.devices[dev_index].short_addr, \
                                                ZB_APS_ADDR_MODE_16_ENDP_PRESENT, \
                                                (uint8_t)attribute->read_req.zed_ep, (uint8_t)attribute->read_req.zc_ep, get_profile_id_by_endpoint((uint8_t)attribute->read_req.zc_ep), attribute->read_req.cluster_id, NULL);
                    return;
                }
            }
        }
    }

    
  WCS_TRACE_DBGREL("<< %s param %hd dev_idx %hd", __FUNCTION__, param, dev_index);
}

void m2z_request_write_attribute(zb_bufid_t param, zb_uint16_t dev_index)
{
  WCS_TRACE_DBGREL(">> %s param %hd dev_idx %hd", __FUNCTION__, param, dev_index);
    
    m2z_device_ep_t *endpoint = NULL;
    m2z_device_cluster_t *cluster = NULL;
    m2z_device_cluster_attr_t *attribute = NULL;
    
    for(zb_uint16_t ep_idx=0; ep_idx < g_device_ctx.devices[dev_index].endpoint; ep_idx++)
    {
        endpoint=(m2z_device_ep_t *)(&(g_device_ctx.devices[dev_index].endpoints[ep_idx]));
        for(zb_uint16_t cluster_idx=0; cluster_idx < (endpoint->num_in_clusters + endpoint->num_out_clusters); cluster_idx++)
        {
            cluster = &(endpoint->ep_cluster[cluster_idx]);
            for (zb_uint16_t attr_idx=0; attr_idx < cluster->num_attrs; attr_idx++)
            {
                attribute = &(cluster->attribute[attr_idx]);
                if(attribute->pending_write)
                {
                    WCS_TRACE_DBGREL(">> %s -> dev_idx: %d zc_ep: %d zed_ep: %d cluster 0x%04x:(%s) attr_id: %d ", __FUNCTION__, dev_index, attribute->read_req.zc_ep
                    , attribute->read_req.zed_ep, attribute->read_req.cluster_id, get_cluster_id_str(attribute->read_req.cluster_id), attribute->read_req.attr_id);
                    zb_uint8_t *cmd_ptr;
                    ZB_ZCL_GENERAL_INIT_WRITE_ATTR_REQ((param), cmd_ptr, ZB_ZCL_DISABLE_DEFAULT_RESPONSE);
                    ZB_ZCL_GENERAL_ADD_VALUE_WRITE_ATTR_REQ(cmd_ptr, attribute->write_req.attr_id, attribute->write_req.attr_type, attribute->write_req.write_value);
                    ZB_ZCL_GENERAL_SEND_WRITE_ATTR_REQ((param), cmd_ptr, g_device_ctx.devices[dev_index].short_addr, \
                                                ZB_APS_ADDR_MODE_16_ENDP_PRESENT, \
                                                (uint8_t)attribute->write_req.zed_ep, (uint8_t)attribute->write_req.zc_ep, get_profile_id_by_endpoint((uint8_t)attribute->write_req.zc_ep), attribute->write_req.cluster_id, NULL);
                    attribute->pending_write = false;
                    return;
                }
            }
        }
    }
  WCS_TRACE_DBGREL("<< %s param %hd dev_idx %hd", __FUNCTION__, param, dev_index);
}

void m2z_get_attribute_zb_status(m2z_device_params_t *dev, zb_uint16_t cluster_id, zb_uint16_t attr_id, zb_uint8_t *attr_zb_status)
{
    zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(dev->short_addr);
    zb_uint16_t ep_id = m2z_dev_get_ep_id_by_short_addr_and_cluster_id(dev->short_addr, cluster_id);
    zb_uint16_t ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(dev->short_addr, ep_id);
    zb_uint16_t cluster_idx = m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(dev->short_addr, ep_id, cluster_id);
    zb_uint16_t attr_idx = m2z_dev_get_attr_idx_by_short_addr_and_ep_id_and_cluster_id(dev->short_addr, ep_id, cluster_id, attr_id);
    
    if(attr_idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_EP_INDEX)
    {
        memcpy(attr_zb_status, &(g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].attr_zb_status), sizeof(zb_uint8_t));
    }
    WCS_TRACE_DBGREL("<< %s attribute_zb_status: %d attribute[attr_idx].attr_zb_status: %d", __FUNCTION__, *attr_zb_status, g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[cluster_idx].attribute[attr_idx].attr_zb_status);
    
}

void m2z_request_send_command(zb_bufid_t param, zb_uint16_t dev_index)
{
  WCS_TRACE_DBGREL(">> %s param %hd dev_idx %hd", __FUNCTION__, param, dev_index);
    
    m2z_device_ep_t *endpoint = NULL;
    m2z_device_cluster_t *cluster = NULL;
    m2z_device_cluster_attr_t *attribute = NULL;
    
    for(zb_uint16_t ep_idx=0; ep_idx < g_device_ctx.devices[dev_index].endpoint; ep_idx++)
    {
        endpoint=(m2z_device_ep_t *)(&(g_device_ctx.devices[dev_index].endpoints[ep_idx]));
        for(zb_uint16_t cluster_idx=0; cluster_idx < (endpoint->num_in_clusters + endpoint->num_out_clusters); cluster_idx++)
        {
            cluster = &(endpoint->ep_cluster[cluster_idx]);
            if(cluster->pending_command)
            {
                WCS_TRACE_DBGREL(">> %s -> dev_idx: %d zc_ep: %d zed_ep: %d cluster 0x%04x:(%s) cmd_id 0x%x", __FUNCTION__, dev_index, cluster->command_req.zc_ep
                , cluster->command_req.zed_ep, cluster->command_req.cluster_id, get_cluster_id_str(cluster->command_req.cluster_id), cluster->command_req.cmd_id);                
                // send a generic command
                zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(param)
                ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, ZB_FALSE)
                ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), (uint8_t)cluster->command_req.cmd_id);
                for(uint8_t cmd_byte = 0; cmd_byte < cluster->command_req.cmd_size;cmd_byte++)
                {
                    ZB_ZCL_PACKET_PUT_DATA8(ptr, cluster->command_req.cmd_data[cmd_byte]);
                }
                ZB_ZCL_FINISH_PACKET(param, ptr)
                ZB_ZCL_SEND_COMMAND_SHORT(param, g_device_ctx.devices[dev_index].short_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT, (uint8_t)cluster->command_req.zed_ep,
                                            (uint8_t)cluster->command_req.zc_ep, get_profile_id_by_endpoint((uint8_t)cluster->command_req.zc_ep), (uint8_t)cluster->command_req.cluster_id, NULL);

            }
        }
    }

  WCS_TRACE_DBGREL("<< %s param %hd dev_idx %hd", __FUNCTION__, param, dev_index);
    return;
}

void m2z_schedule_request_attr_disc(zb_uint16_t dev_idx)
{
  zb_buf_get_out_delayed_ext((zb_callback2_t)m2z_request_attr_disc, dev_idx, 0);
}

void m2z_callback_dev_attr_disc(zb_uint8_t param)
{
  zb_zcl_command_send_status_t *cmd_send_status = ZB_BUF_GET_PARAM(param, zb_zcl_command_send_status_t);
  zb_uint16_t dev_idx = MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX;

  WCS_TRACE_DBGREL(">> %s: param %hd status %hd", __FUNCTION__, param, cmd_send_status->status);

  if (cmd_send_status->dst_addr.addr_type == ZB_ZCL_ADDR_TYPE_SHORT)
  {
    dev_idx = m2z_dev_get_index_by_short_addr(cmd_send_status->dst_addr.u.short_addr);
  }

  if (dev_idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX)
  {
    ZB_SCHEDULE_APP_ALARM((zb_callback_t)m2z_schedule_request_attr_disc, (uint8_t)dev_idx, ZB_TIME_ONE_SECOND);
  }

  zb_buf_free(param);

  WCS_TRACE_DBGREL("<< %s", __FUNCTION__);
}

void m2z_request_attr_disc(zb_bufid_t param, zb_uint16_t dev_idx)
{
  zb_uint16_t ep_idx;
  zb_uint16_t ep_cluster_idx;
  zb_bool_t found_cluster_ep = 0;
  m2z_device_params_t *dev = (m2z_device_params_t *)(&g_device_ctx.devices[dev_idx]);

  WCS_TRACE_DBGREL(">> %s param %hd dev_idx %hd", __FUNCTION__, param, dev_idx);

  /* Cluster is not known, search in all EPs of the device for clusters without known attributes */
  for (zb_uint16_t i = 0; i < dev->endpoint; i++ )
  {
    for ( zb_uint16_t j = 0; j < (dev->endpoints[i].num_in_clusters + dev->endpoints[i].num_out_clusters); j++)
    {
      /* Break if a cluster with no know attributes is found */
      if(dev->endpoints[i].ep_cluster[j].disc_attrs_state == NO_DISC_ATTRS_INFO)
      {
        ep_idx = i;
        ep_cluster_idx = j;
        found_cluster_ep = 1;
        break;
      }
      if(j == (dev->endpoints[i].num_in_clusters + dev->endpoints[i].num_out_clusters - 1) )
      {
          // this cluster has no more attributes to be discovered
          dev->endpoints[i].ep_cluster[j].disc_attrs_state = KNOWN_DISC_ATTRS;
      }

    }
    if(found_cluster_ep)
      break;
  }

  if(found_cluster_ep)
  {
    WCS_TRACE_DBGREL("%s: Request Discover Attributes to 0x%x (Zb dev index: %d) -> Endpoint: %d Cluster 0x%04x:(%s)"
        , __FUNCTION__,dev->short_addr, dev_idx, dev->endpoints[ep_idx].ep_id, dev->endpoints[ep_idx].ep_cluster[ep_cluster_idx].cluster_id, get_cluster_id_str(dev->endpoints[ep_idx].ep_cluster[ep_cluster_idx].cluster_id));

    dev->endpoints[ep_idx].ep_cluster[ep_cluster_idx].disc_attrs_state = REQUESTED_DISC_ATTRS_INFO;

    ZB_ZCL_GENERAL_DISC_READ_ATTR_REQ(param, ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                                      0, /* start attribute id */
                                      0xff, /* maximum attribute id-s */
                                      dev->short_addr,
                                      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                      (uint8_t)dev->endpoints[ep_idx].ep_id,
                                      MATTER_ZIGBEERCP_BRIDGE_ENDPOINT, /*source EP*/
                                      dev->endpoints[ep_idx].profile_id,
                                      dev->endpoints[ep_idx].ep_cluster[ep_cluster_idx].cluster_id,
                                      m2z_callback_dev_attr_disc);
  }
  else
  {
        WCS_TRACE_DBGREL("%s: No more endpoints without known attributes from 0x%x (Zb dev index: %d) -> dev->announced_to_bridge = %d"
        , __FUNCTION__,dev->short_addr, dev_idx, dev->announced_to_bridge);
        if(dev->announced_to_bridge == 0)
        {
            dev->announced_to_bridge = 1;
            WCS_TRACE_DBGREL("%s: All Discover Attributes of 0x%x were queried for Discover Attributes, Announce this device to BridgeMgr "
                , __FUNCTION__,dev->short_addr);
            // all endpoint Simple Descriptors were received for that device
            // Announce it to BridgeMgr
            int MsgType = BRIDGE_ADD_DEV;

            (*ZigbeeRcp_msg_callback)(MsgType, (m2z_device_params_t *)(&(g_device_ctx.devices[dev_idx])), &dev_idx);
        }
  }
  WCS_TRACE_DBGREL("<< %s param %hd dev_idx %hd", __FUNCTION__, param, dev_idx);
}

void m2z_update_endpoint_supported_clusters(m2z_device_params_t *device, uint16_t zcl_cluster_id)
{
    switch(zcl_cluster_id)
    {
        case ZB_ZCL_CLUSTER_ID_BASIC:
            device->supported_clusters.bitmap.hasBasic_x0=1;
        break;
        case ZB_ZCL_CLUSTER_ID_POWER_CONFIG:
            device->supported_clusters.bitmap.hasPowerCfg_x1=1;
        break;
        case ZB_ZCL_CLUSTER_ID_DEVICE_TEMP_CONFIG:
            device->supported_clusters.bitmap.hasDevTempCfg_x2=1;
        break;
        case ZB_ZCL_CLUSTER_ID_IDENTIFY:
            device->supported_clusters.bitmap.hasIdentify_x3=1;
        break;
        case ZB_ZCL_CLUSTER_ID_GROUPS:
            device->supported_clusters.bitmap.hasGroups_x4=1;
        break;
        case ZB_ZCL_CLUSTER_ID_SCENES:
            device->supported_clusters.bitmap.hasScenes_x5=1;
        break;
        case ZB_ZCL_CLUSTER_ID_ON_OFF:
            device->supported_clusters.bitmap.hasOnOff_x6=1;
        break;
        case ZB_ZCL_CLUSTER_ID_ON_OFF_SWITCH_CONFIG:
            device->supported_clusters.bitmap.hasOnOffSwitchCfg_x7=1;
        break;
        case ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL:
            device->supported_clusters.bitmap.hasLvlCtrl_x8=1;
        break;
        case ZB_ZCL_CLUSTER_ID_ALARMS:
            device->supported_clusters.bitmap.hasAlarms_x9=1;
        break;
        case ZB_ZCL_CLUSTER_ID_TIME:
            device->supported_clusters.bitmap.hasTime_xa=1;
        break;
        case ZB_ZCL_CLUSTER_ID_RSSI_LOCATION:
            device->supported_clusters.bitmap.hasRssiLoc_xb=1;
        break;
        case ZB_ZCL_CLUSTER_ID_ANALOG_INPUT:
            device->supported_clusters.bitmap.hasAnalogIn_xc=1;
        break;
        case ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT:
            device->supported_clusters.bitmap.hasAnalogOut_xd=1;
        break;
        case ZB_ZCL_CLUSTER_ID_ANALOG_VALUE:
            device->supported_clusters.bitmap.hasAnalogValue_xe=1;
        break;
        case ZB_ZCL_CLUSTER_ID_BINARY_INPUT:
            device->supported_clusters.bitmap.hasBinaryIn_xf=1;
        break;
        case ZB_ZCL_CLUSTER_ID_BINARY_OUTPUT:
            device->supported_clusters.bitmap.hasBinaryOut_x10=1;
        break;
        case ZB_ZCL_CLUSTER_ID_BINARY_VALUE:
            device->supported_clusters.bitmap.hasBinaryValue_x11=1;
        break;
        case ZB_ZCL_CLUSTER_ID_MULTI_INPUT:
            device->supported_clusters.bitmap.hasMultiIn_x12=1;
        break;
        case ZB_ZCL_CLUSTER_ID_MULTI_OUTPUT:
            device->supported_clusters.bitmap.hasMultiOut_x13=1;
        break;
        case ZB_ZCL_CLUSTER_ID_MULTI_VALUE:
            device->supported_clusters.bitmap.hasMultiValue_x14=1;
        break;
        case ZB_ZCL_CLUSTER_ID_COMMISSIONING:
            device->supported_clusters.bitmap.hasCommissioning_x15=1;
        break;
        case ZB_ZCL_CLUSTER_ID_OTA_UPGRADE:
            device->supported_clusters.bitmap.hasOtaUpgrade_x19=1;
        break;
        case ZB_ZCL_CLUSTER_ID_POLL_CONTROL:
            device->supported_clusters.bitmap.hasPollCtrl_x20=1;
        break;
        case ZB_ZCL_CLUSTER_ID_GREEN_POWER:
            device->supported_clusters.bitmap.hasGreenPower_x21=1;
        break;
        case ZB_ZCL_CLUSTER_ID_KEEP_ALIVE:
            device->supported_clusters.bitmap.hasKeepAlive_x25=1;
        break;
        case ZB_ZCL_CLUSTER_ID_SHADE_CONFIG:
            device->supported_clusters.bitmap.hasShadeCfg_x100=1;
        break;
        case ZB_ZCL_CLUSTER_ID_DOOR_LOCK:
            device->supported_clusters.bitmap.hasDoorLock_x101=1;
        break;
        case ZB_ZCL_CLUSTER_ID_WINDOW_COVERING:
            device->supported_clusters.bitmap.hasWindowCovering_x102=1;
        break;
        case ZB_ZCL_CLUSTER_ID_PUMP_CONFIG_CONTROL:
            device->supported_clusters.bitmap.hasPumpCfgCtrl_x200=1;
        break;
        case ZB_ZCL_CLUSTER_ID_THERMOSTAT:
            device->supported_clusters.bitmap.hasThermostat_x201=1;
        break;
        case ZB_ZCL_CLUSTER_ID_FAN_CONTROL:
            device->supported_clusters.bitmap.hasFanCtrl_x202=1;
        break;
        case ZB_ZCL_CLUSTER_ID_DEHUMID_CONTROL:
            device->supported_clusters.bitmap.hasDehumidCtr_x203=1;
        break;
        case ZB_ZCL_CLUSTER_ID_THERMOSTAT_UI_CONFIG:
            device->supported_clusters.bitmap.hasThermostatUiCfg_x204=1;
        break;
        case ZB_ZCL_CLUSTER_ID_COLOR_CONTROL:
            device->supported_clusters.bitmap.hasColorCtrl_x300=1;
        break;
        case ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG:
            device->supported_clusters.bitmap.hasBallastCfg_x301=1;
        break;
        case ZB_ZCL_CLUSTER_ID_ILLUMINANCE_MEASUREMENT:
            device->supported_clusters.bitmap.hasIlluminanceMeas_x400=1;
        break;
        case ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT:
            device->supported_clusters.bitmap.hasTempMeas_x402=1;
        break;
        case ZB_ZCL_CLUSTER_ID_PRESSURE_MEASUREMENT:
            device->supported_clusters.bitmap.hasPressureMeas_x403=1;
        break;
        case ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT:
            device->supported_clusters.bitmap.hasRelHumidityMeas_x405=1;
        break;
        case ZB_ZCL_CLUSTER_ID_OCCUPANCY_SENSING:
            device->supported_clusters.bitmap.hasOccupancySensing_x406=1;
        break;
        case ZB_ZCL_CLUSTER_ID_CARBON_DIOXIDE_MEASUREMENT:
            device->supported_clusters.bitmap.hasCarbonDioMeas_x40D=1;
        break;
        case ZB_ZCL_CLUSTER_ID_PM2_5_MEASUREMENT:
            device->supported_clusters.bitmap.hasPM2_5Meas_x42a=1;
        break;
        case ZB_ZCL_CLUSTER_ID_IAS_ZONE:
            device->supported_clusters.bitmap.hasIasZone_x501=1;
        break;
        case ZB_ZCL_CLUSTER_ID_IAS_ACE:
            device->supported_clusters.bitmap.hasIasAce_x501=1;
        break;
        case ZB_ZCL_CLUSTER_ID_IAS_WD:
            device->supported_clusters.bitmap.hasIasWd_x502=1;
        break;
        case ZB_ZCL_CLUSTER_ID_PRICE:
            device->supported_clusters.bitmap.hasPrice_x700=1;
        break;
        case ZB_ZCL_CLUSTER_ID_DRLC:
            device->supported_clusters.bitmap.hasDRLC_x701=1;
        break;
        case ZB_ZCL_CLUSTER_ID_METERING:
            device->supported_clusters.bitmap.hasMetering_x702=1;
        break;
        case ZB_ZCL_CLUSTER_ID_MESSAGING:
            device->supported_clusters.bitmap.hasMessaging_x703=1;
        break;
        case ZB_ZCL_CLUSTER_ID_TUNNELING:
            device->supported_clusters.bitmap.hasTunelling_x704=1;
        break;
        case ZB_ZCL_CLUSTER_ID_PREPAYMENT:
            device->supported_clusters.bitmap.hasPrepayment_x705=1;
        break;
        case ZB_ZCL_CLUSTER_ID_ENERGY_MANAGEMENT:
            device->supported_clusters.bitmap.hasNrjMngt_x706=1;
        break;
        case ZB_ZCL_CLUSTER_ID_CALENDAR:
            device->supported_clusters.bitmap.hasCalendar_x707=1;
        break;
        case ZB_ZCL_CLUSTER_ID_DEVICE_MANAGEMENT:
            device->supported_clusters.bitmap.hasDevMngt_x708=1;
        break;
        case ZB_ZCL_CLUSTER_ID_EVENTS:
            device->supported_clusters.bitmap.hasEvents_x709=1;
        break;
        case ZB_ZCL_CLUSTER_ID_MDU_PAIRING:
            device->supported_clusters.bitmap.hasMduPairing_x70a=1;
        break;
        case ZB_ZCL_CLUSTER_ID_SUB_GHZ:
            device->supported_clusters.bitmap.hasSubGhz_x70b=1;
        break;
        case ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE:
            device->supported_clusters.bitmap.hasDaySched_x70d=1;
        break;
        case ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT:
            device->supported_clusters.bitmap.hasKeyEstblmt_x800=1;
        break;
        case ZB_ZCL_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS:
            device->supported_clusters.bitmap.hasAplianceEvAl_xb02=1;
        break;
        case ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT:
            device->supported_clusters.bitmap.hasElecMeas_xb04=1;
        break;
        case ZB_ZCL_CLUSTER_ID_DIAGNOSTICS:
            device->supported_clusters.bitmap.hasDiags_xb05=1;
        break;
        case ZB_ZCL_CLUSTER_ID_WWAH:
            device->supported_clusters.bitmap.hasWwah_xfc57=1;
        break;
        case ZB_ZCL_CLUSTER_ID_TOUCHLINK_COMMISSIONING:
            device->supported_clusters.bitmap.hasTouchCom_x1000=1;
        break;
        case ZB_ZCL_CLUSTER_ID_TUNNEL:
            device->supported_clusters.bitmap.hasTunnel_xfc00=1;
        break;
        case ZB_ZCL_CLUSTER_ID_IR_BLASTER:
            device->supported_clusters.bitmap.hasIrBlaster_x1fc01=1;
        break;
        case ZB_ZCL_CLUSTER_ID_CUSTOM_ATTR:
            device->supported_clusters.bitmap.hasCustomAttr_xffee=1;
        break;
        case ZB_ZCL_CLUSTER_ID_METER_IDENTIFICATION:
            device->supported_clusters.bitmap.hasMeterId_x0b01=1;
        break;
        case ZB_ZCL_CLUSTER_ID_DIRECT_CONFIGURATION:
            device->supported_clusters.bitmap.hasDirectCfg_x3d=1;
        break;
    }
}

void m2z_callback_dev_simple_desc(zb_bufid_t param)
{
    zb_uint8_t *zdp_cmd = zb_buf_begin(param);
    zb_zdo_simple_desc_resp_t *resp = (zb_zdo_simple_desc_resp_t*)(zdp_cmd);
    zb_uint_t i;

    zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(resp->hdr.nwk_addr);
    zb_uint16_t ep_idx = m2z_dev_get_ep_idx_by_short_addr_and_ep_id(resp->hdr.nwk_addr, resp->simple_desc.endpoint);

    WCS_TRACE_DBGREL(">> %s: status %hd, addr 0x%x",
            __FUNCTION__, resp->hdr.status, resp->hdr.nwk_addr);

    if (resp->hdr.status != ZB_ZDP_STATUS_SUCCESS)
    {
        WCS_TRACE_DBGREL("Error incorrect status");
    }
    else
    {
        /* Check if the cluster array has space for the received cluster information */
        if ( ZB_ARRAY_SIZE(g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster) <
              (resp->simple_desc.app_input_cluster_count + resp->simple_desc.app_output_cluster_count))
        {
            WCS_TRACE_DBGREL("No memory to store all Clusters of this EP");
        }
        else
        {
            WCS_TRACE_DBGREL("%s: ep %hd, profile_id %d, dev id %d, dev ver %hd, input count 0x%hx, output count 0x%hx",
                    __FUNCTION__, resp->simple_desc.endpoint, resp->simple_desc.app_profile_id,
                    resp->simple_desc.app_device_id, resp->simple_desc.app_device_version,
                  resp->simple_desc.app_input_cluster_count, resp->simple_desc.app_output_cluster_count);

            WCS_TRACE_DBGREL("%s: clusters:", __FUNCTION__);

            /* Fill EP information relative to clusters*/
            g_device_ctx.devices[dev_idx].endpoints[ep_idx].clusters_state = KNOWN_CLUSTER;
            g_device_ctx.devices[dev_idx].endpoints[ep_idx].profile_id = resp->simple_desc.app_profile_id;
            g_device_ctx.devices[dev_idx].endpoints[ep_idx].device_id = resp->simple_desc.app_device_id;
            g_device_ctx.devices[dev_idx].endpoints[ep_idx].num_in_clusters = resp->simple_desc.app_input_cluster_count;
            g_device_ctx.devices[dev_idx].endpoints[ep_idx].num_out_clusters = resp->simple_desc.app_output_cluster_count;

            /* Add the received clusters to cluster array*/
            for(i = 0; i < (resp->simple_desc.app_input_cluster_count + resp->simple_desc.app_output_cluster_count); i++)
            {
              g_device_ctx.devices[dev_idx].endpoints[ep_idx].ep_cluster[i].cluster_id = *(resp->simple_desc.app_cluster_list + i);
              m2z_update_endpoint_supported_clusters((m2z_device_params_t *)(&(g_device_ctx.devices[dev_idx])), *(resp->simple_desc.app_cluster_list + i));
              WCS_TRACE_DBGREL("%s: 0x%hx -> %s",__FUNCTION__,  *(resp->simple_desc.app_cluster_list + i), get_cluster_id_str(*(resp->simple_desc.app_cluster_list + i)));
            }

            /* For each cluster request info of their attributes */
            ZB_SCHEDULE_APP_ALARM(m2z_schedule_request_attr_disc, (uint8_t)dev_idx, 2 * ZB_TIME_ONE_SECOND);
        }
    }
    zb_buf_free(param);
    WCS_TRACE_DBGREL("<< %s", __FUNCTION__);
}

/* Send_simple_desc_req */
void m2z_request_simple_desc(zb_bufid_t param, zb_uint16_t dev_idx)
{
  zb_zdo_simple_desc_req_t *req;
  WCS_TRACE_DBGREL(">> %s param %hd dev_idx %hd", __FUNCTION__, param, dev_idx);

  if (!param)
  {
      zb_buf_get_out_delayed_ext(m2z_request_simple_desc, dev_idx, 0);
  }
  else
  {
      WCS_TRACE_DBGREL(">> %s", __FUNCTION__);

      req = zb_buf_initial_alloc(param, sizeof(zb_zdo_simple_desc_req_t));
      req->nwk_addr = g_device_ctx.devices[dev_idx].short_addr;

      for (int i = 0; i < g_device_ctx.devices[dev_idx].endpoint; i++)
      {
        if(g_device_ctx.devices[dev_idx].endpoints[i].clusters_state ==  NO_CLUSTER_INFO)
        {
          g_device_ctx.devices[dev_idx].endpoints[i].clusters_state = REQUESTED_CLUSTER_INFO;
          req->endpoint = (uint8_t)g_device_ctx.devices[dev_idx].endpoints[i].ep_id;
          break;
        }
      }
      zb_zdo_simple_desc_req(param, m2z_callback_dev_simple_desc);

  }
  WCS_TRACE_DBGREL("<< %s param %hd dev_idx %hd", __FUNCTION__, param, dev_idx);
}

void m2z_schedule_request_simple_desc(zb_uint16_t dev_idx) {
    zb_buf_get_out_delayed_ext((zb_callback2_t)m2z_request_simple_desc, dev_idx, 0);
}

/* active_ep_req callback */
void m2z_callback_dev_active_ep(zb_bufid_t param)
{
  zb_uint8_t *zdp_cmd = zb_buf_begin(param);
  zb_zdo_ep_resp_t *resp = (zb_zdo_ep_resp_t*)zdp_cmd;
  zb_uint8_t *ep_list = zdp_cmd + sizeof(zb_zdo_ep_resp_t);

  zb_uint16_t dev_idx = m2z_dev_get_index_by_short_addr(resp->nwk_addr);

  WCS_TRACE_DBGREL("%s: status %hd, addr 0x%x",
            __FUNCTION__, resp->status, resp->nwk_addr);

  if (dev_idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX)
  {
    if (resp->status != ZB_ZDP_STATUS_SUCCESS)
    {
        WCS_TRACE_DBGREL("active_ep_resp: Error incorrect status");
    } else
    {
      /*Ensure the endpoints array has space for the Eps received */
      if (ZB_ARRAY_SIZE(g_device_ctx.devices[dev_idx].endpoints) < resp->ep_count)
      {
        WCS_TRACE_DBGREL("No memory to store all EndPoints");
      } else
      {
        g_device_ctx.devices[dev_idx].endpoint = resp->ep_count;
        WCS_TRACE_DBGREL("%s: ep count %hd, ep numbers:", __FUNCTION__, resp->ep_count);
        /* Add all received EPs to the EP array */
        for (int i = 0; i < resp->ep_count; i++)
        {
          g_device_ctx.devices[dev_idx].endpoints[i].ep_id = *(ep_list + i);
          WCS_TRACE_DBGREL("%s: ep %hd", __FUNCTION__, *(ep_list + i));
          /*Send a simple_dec_req per active EP received*/
          ZB_SCHEDULE_APP_ALARM(m2z_schedule_request_simple_desc, (uint8_t)dev_idx, 2 * ZB_TIME_ONE_SECOND);
        }
      }
    }
      }
  else
  {
    /* Received active_ep_rsp from unknown device */
  }

  zb_buf_free(param);

}

/* Send active_ep_req */
void m2z_request_active_ep(zb_bufid_t param, zb_uint16_t dev_idx)
{
  WCS_TRACE_DBGREL(">> %s param %hd dev_idx %hd", __FUNCTION__, param, dev_idx);
  if (!param)
  {
    zb_buf_get_out_delayed_ext(m2z_request_active_ep, dev_idx, 0);
  }
  else
  {
      zb_zdo_active_ep_req_t *req;
      req = zb_buf_initial_alloc(param, sizeof(zb_zdo_active_ep_req_t));

      req->nwk_addr = g_device_ctx.devices[dev_idx].short_addr;
      zb_zdo_active_ep_req(param, m2z_callback_dev_active_ep);
  }
  WCS_TRACE_DBGREL("<< %s param %hd dev_idx %hd", __FUNCTION__, param, dev_idx);
}

void m2z_schedule_request_active_ep(zb_uint16_t dev_idx) {
    zb_buf_get_out_delayed_ext((zb_callback2_t)m2z_request_active_ep, dev_idx, 0);
}

/* Callback which will be called on incoming nwk Leave packet. */
void m2z_callback_dev_leave_indication(zb_ieee_addr_t dev_addr)
{
  zb_uint16_t dev_idx;

  WCS_TRACE_DBGREL("> m2z_leave_indication device_addr ieee", TRACE_ARG_64(dev_addr));

  dev_idx = m2z_dev_get_index_by_ieee_addr(dev_addr);

  if (dev_idx != MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX)
  {
    /* It is the device which we controlled before. Remove it from the dev list. */
    //ZB_SCHEDULE_APP_ALARM_CANCEL(m2z_remove_device_delayed, dev_idx);
    //ZB_SCHEDULE_APP_ALARM_CANCEL(find_onoff_device_tmo, dev_idx);
    g_device_ctx.devices[dev_idx].dev_state = NO_DEVICE;
    ZB_MEMSET(&g_device_ctx.devices[dev_idx], 0, sizeof(m2z_device_params_t));
  }

  WCS_TRACE_DBGREL("< m2z_leave_indication");
}

#ifdef ZB_USE_NVRAM
zb_uint16_t m2z_get_nvram_data_size(void)
{
  WCS_TRACE_DBGREL("m2z_get_nvram_data_size, ret %hd", sizeof(m2z_device_nvram_dataset_t));
  return ((zb_uint16_t)sizeof(m2z_device_nvram_dataset_t));
}

void m2z_nvram_read_app_data(zb_uint8_t page, zb_uint32_t pos, zb_uint16_t payload_length)
{
  m2z_device_nvram_dataset_t ds;
  zb_ret_t ret;
  zb_uint8_t i;

  WCS_TRACE_DBGREL(">> m2z_nvram_read_app_data page %hd pos %d", page, pos);

  ret = zb_nvram_read_data(page, pos, (zb_uint8_t*)&ds, (zb_uint16_t)sizeof(ds));

  if (ret == RET_OK)
  {
    ZB_MEMCPY(g_device_ctx.devices, &ds, sizeof(m2z_device_nvram_dataset_t));

    /* Reset timeouts for all bulbs */
    for (i = 0; i < ZB_ARRAY_SIZE(g_device_ctx.devices); ++i)
    {
      if (g_device_ctx.devices[i].dev_state == COMPLETED)
      {
        g_device_ctx.devices[i].pending_toggle = i + 1;
      }
    }
  }

  WCS_TRACE_DBGREL("<< m2z_nvram_read_app_data ret %d", ret);
}

zb_ret_t m2z_nvram_write_app_data(zb_uint8_t page, zb_uint32_t pos)
{
  zb_ret_t ret;
  m2z_device_nvram_dataset_t ds;

  WCS_TRACE_DBGREL(">> m2z_nvram_write_app_data, page %hd, pos %d", page, pos);

  ZB_MEMCPY(&ds, g_device_ctx.devices, sizeof(m2z_device_nvram_dataset_t));

  ret = zb_nvram_write_data(page, pos, (zb_uint8_t*)&ds, (zb_uint16_t)sizeof(ds));

  WCS_TRACE_DBGREL("<< m2z_nvram_write_app_data, ret %d", ret);

  return ret;
}

#endif  /* ZB_USE_NVRAM */

