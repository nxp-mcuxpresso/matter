#ifndef MATTER_ZIGBEERCP_BRIDGE_DEVICE_H
#define MATTER_ZIGBEERCP_BRIDGE_DEVICE_H 1

#define ZB_HA_DEVICE_VER_MATTER_ZIGBEERCP_BRIDGE 0  /*!< MATTER ZIGBEERCP BRIDGE device version */
#define ZB_HA_MATTER_ZIGBEERCP_BRIDGE_DEVICE_ID 0

#define ZB_HA_MATTER_ZIGBEERCP_BRIDGE_IN_CLUSTER_NUM 4  /*!< @internal MATTER ZIGBEERCP BRIDGE IN clusters number */
#define ZB_HA_MATTER_ZIGBEERCP_BRIDGE_OUT_CLUSTER_NUM 5 /*!< @internal MATTER ZIGBEERCP BRIDGE OUT clusters number */

#define ZB_HA_MATTER_ZIGBEERCP_BRIDGE_CLUSTER_NUM                                      \
  (ZB_HA_MATTER_ZIGBEERCP_BRIDGE_IN_CLUSTER_NUM + ZB_HA_MATTER_ZIGBEERCP_BRIDGE_OUT_CLUSTER_NUM)

/*! @internal Number of attribute for reporting on MATTER ZIGBEERCP BRIDGE device */
#define ZB_HA_MATTER_ZIGBEERCP_BRIDGE_REPORT_ATTR_COUNT \
  (ZB_ZCL_ON_OFF_SWITCH_CONFIG_REPORT_ATTR_COUNT)

/** @brief Declare cluster list for MATTER ZIGBEERCP BRIDGE device
    @param cluster_list_name - cluster list variable name
    @param on_off_switch_config_attr_list - attribute list for On/off switch configuration cluster
    @param basic_attr_list - attribute list for Basic cluster
    @param identify_attr_list - attribute list for Identify cluster
 */
#define ZB_HA_DECLARE_MATTER_ZIGBEERCP_BRIDGE_CLUSTER_LIST(                           \
      cluster_list_name,                                                    \
      basic_attr_list,                                                      \
      identify_attr_list,                                                   \
      groups_attr_list,                                                   \
      scenes_attr_list)                                                   \
      zb_zcl_cluster_desc_t cluster_list_name[] =                           \
      {                                                                     \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_BASIC,                                          \
          ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),                \
          (basic_attr_list),                                                \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                                       \
          ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),             \
          (identify_attr_list),                                             \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                        \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                    \
          ZB_ZCL_CLUSTER_ID_GROUPS,                             \
          ZB_ZCL_ARRAY_SIZE(groups_attr_list, zb_zcl_attr_t),   \
          (groups_attr_list),                                   \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                           \
          ZB_ZCL_MANUF_CODE_INVALID                             \
        ),                                                      \
        ZB_ZCL_CLUSTER_DESC(                                    \
          ZB_ZCL_CLUSTER_ID_SCENES,                             \
          ZB_ZCL_ARRAY_SIZE(scenes_attr_list, zb_zcl_attr_t),   \
          (scenes_attr_list),                                   \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                           \
          ZB_ZCL_MANUF_CODE_INVALID                             \
        ),                                                       \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_SCENES,                                         \
          0,                                                                \
          NULL,                                                             \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_GROUPS,                                         \
          0,                                                                \
          NULL,                                                             \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                            \
          ZB_ZCL_CLUSTER_ID_IAS_ZONE,                                   \
          0,                                                            \
          NULL,                                                         \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                   \
          ZB_ZCL_MANUF_CODE_INVALID                                     \
          ),                                                            \
        ZB_ZCL_CLUSTER_DESC(                                            \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                                   \
          0,                                                            \
          NULL,                                                         \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                   \
          ZB_ZCL_MANUF_CODE_INVALID                                     \
        )                                                               \
    }


/** @internal @brief Declare simple descriptor for MATTER ZIGBEERCP BRIDGE device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param in_clust_num - number of supported input clusters
    @param out_clust_num - number of supported output clusters
    @note in_clust_num, out_clust_num should be defined by numeric constants, not variables or any
    definitions, because these values are used to form simple descriptor type name
*/
#define ZB_ZCL_DECLARE_MATTER_ZIGBEERCP_BRIDGE_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                                        \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) simple_desc_##ep_name =                 \
  {                                                                                           \
    ep_id,                                                                                    \
    ZB_AF_HA_PROFILE_ID,                                                                      \
    ZB_HA_MATTER_ZIGBEERCP_BRIDGE_DEVICE_ID,                                                            \
    ZB_HA_DEVICE_VER_MATTER_ZIGBEERCP_BRIDGE,                                                           \
    0,                                                                                        \
    in_clust_num,                                                                             \
    out_clust_num,                                                                            \
    {                                                                                         \
      ZB_ZCL_CLUSTER_ID_BASIC,                                                                \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                             \
      ZB_ZCL_CLUSTER_ID_ON_OFF_SWITCH_CONFIG,                                                 \
      ZB_ZCL_CLUSTER_ID_ON_OFF,                                                               \
      ZB_ZCL_CLUSTER_ID_SCENES,                                                               \
      ZB_ZCL_CLUSTER_ID_GROUPS,                                         \
      ZB_ZCL_CLUSTER_ID_IAS_ZONE,                                       \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                       \
    }                                                                                         \
  }

/** @brief Declare endpoint for MATTER ZIGBEERCP BRIDGE device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param cluster_list - endpoint cluster list
 */
#define ZB_HA_DECLARE_MATTER_ZIGBEERCP_BRIDGE_EP(ep_name, ep_id, cluster_list) \
  ZB_ZCL_DECLARE_MATTER_ZIGBEERCP_BRIDGE_SIMPLE_DESC(                          \
      ep_name,                                                       \
      ep_id,                                                         \
      ZB_HA_MATTER_ZIGBEERCP_BRIDGE_IN_CLUSTER_NUM,                            \
      ZB_HA_MATTER_ZIGBEERCP_BRIDGE_OUT_CLUSTER_NUM);                          \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name,                                                \
                              ep_id,                                                  \
      ZB_AF_HA_PROFILE_ID,                                           \
      0,                                                             \
      NULL,                                                          \
      ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t),        \
      cluster_list,                                                  \
                              (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name,       \
                              0, NULL, /* No reporting ctx */                         \
                              0, NULL)

/** @brief Declare MATTER ZIGBEERCP BRIDGE device context.
    @param device_ctx - device context variable name.
    @param ep_name - endpoint variable name.
*/
#define ZB_HA_DECLARE_MATTER_ZIGBEERCP_BRIDGE_CTX(device_ctx, ep_name)                \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx, ep_name)
 /* No CVC ctx */

/*! @} */

#endif /* MATTER_ZIGBEERCP_BRIDGE_DEVICE_H */
