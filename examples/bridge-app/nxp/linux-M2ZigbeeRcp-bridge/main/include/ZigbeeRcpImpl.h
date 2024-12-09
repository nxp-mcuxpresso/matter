#ifndef MATTER_ZIGBEERCP_BRIDGE_H
#define MATTER_ZIGBEERCP_BRIDGE_H 1

#include "ZigbeeRcpImplDevice.h"
#include "zboss_api.h"
#include "zb_led_button.h"

#define ZB_TRACE_FILE_ID 24

/* Used endpoint number */
#define MATTER_ZIGBEERCP_BRIDGE_ENDPOINT 1

#undef ZB_USE_BUTTONS
/* IAS zone IEEE address */
#define MATTER_ZIGBEERCP_BRIDGE_IEEE_ADDR {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}
/* Default channel */
#define MATTER_ZIGBEERCP_BRIDGE_CHANNEL_MASK (1l<<21)

/* Device settings definitions */
#define MATTER_ZIGBEERCP_BRIDGE_INVALID_DEV_INDEX 0xff
#define MATTER_ZIGBEERCP_BRIDGE_INVALID_EP_INDEX  0xff
#define MATTER_ZIGBEERCP_BRIDGE_DEV_NUMBER 255
#define MATTER_ZIGBEERCP_BRIDGE_MAX_EP_PER_DEV 8
#define MATTER_ZIGBEERCP_BRIDGE_MAX_CLUSTERS_PER_EP 32
#define MATTER_ZIGBEERCP_BRIDGE_MAX_ATTRS_PER_CLUSTER 64
#define MATTER_ZIGBEERCP_BRIDGE_TOGGLE_ITER_TIMEOUT (5*ZB_TIME_ONE_SECOND)
#define MATTER_ZIGBEERCP_BRIDGE_RANDOM_TIMEOUT_VAL (15)
#define MATTER_ZIGBEERCP_BRIDGE_COMMUNICATION_PROBLEMS_TIMEOUT (30) /* 300 sec */
#define MATTER_ZIGBEERCP_BRIDGE_TOGGLE_TIMEOUT (ZB_RANDOM_VALUE(MATTER_ZIGBEERCP_BRIDGE_RANDOM_TIMEOUT_VAL)) /* * ZB_TIME_ONE_SECOND */

/* Reporting settings */
#define MATTER_ZIGBEERCP_BRIDGE_REPORTING_MIN_INTERVAL 30
#define MATTER_ZIGBEERCP_BRIDGE_REPORTING_MAX_INTERVAL(dev_idx) ((90 * ((dev_idx) + 1)))

// time to wait in ms that Zigbee commands are completed
#define COMMAND_COMPLETED_TIME_MS (300)

/* Device states enumeration */
enum m2z_device_state_e
{
    NO_DEVICE,
    IEEE_ADDR_DISCOVERY,
    SHORT_ADDR_DISCOVERY,
    AUTHORIZATION_DISCOVERY,
    ANNOUNCE_COMPLETED,
    READ_ATTRS_COMPLETED,
    CONFIGURE_BINDING,
    CONFIGURE_REPORTING,
    COMPLETED,
    COMPLETED_NO_TOGGLE,
};

/* Cluster state */
enum m2z_cluster_state_e
{
    NO_CLUSTER_INFO,
    REQUESTED_CLUSTER_INFO,
    KNOWN_CLUSTER,
};


/* Attrs state */
enum m2z_discovery_attrs_state_e
{
    NO_DISC_ATTRS_INFO,
    REQUESTED_DISC_ATTRS_INFO,
    KNOWN_DISC_ATTRS,
};

/* Attrs state */
enum m2z_attrs_state_e
{
    NO_ATTR_INFO,
    DISCOVERED_ATTR,
    SCHEDULED_VALUE_READ_ATTR,
    REQUESTED_VALUE_READ_ATTR,
    GOT_VALUE_ATTR,
};

/* Declare read_attribute structure */
typedef ZB_PACKED_PRE struct m2z_device_read_attr_s {
    zb_uint16_t dev_idx;
    zb_uint16_t zc_ep;
    zb_uint16_t zed_ep;
    zb_uint16_t cluster_id;
    zb_uint16_t attr_id;
} m2z_device_read_attr_t;

/* Declare write_attribute structure */
typedef ZB_PACKED_PRE struct m2z_device_write_attr_s {
    zb_uint16_t dev_idx;
    zb_uint16_t zc_ep;
    zb_uint16_t zed_ep;
    zb_uint16_t cluster_id;
    zb_uint16_t attr_id;
    zb_uint8_t attr_type;
    zb_uint8_t write_value[32];
} m2z_device_write_attr_t;

/* Declare command structure */
typedef ZB_PACKED_PRE struct m2z_device_command_s {
    zb_uint16_t dev_idx;
    zb_uint16_t zc_ep;
    zb_uint16_t zed_ep;
    zb_uint16_t cluster_id;
    zb_uint16_t cmd_id;
    zb_uint8_t cmd_data[32];
    zb_uint8_t cmd_size;
    
} m2z_device_command_t;

typedef ZB_PACKED_PRE struct m2z_device_cluster_attr_s {
    zb_uint16_t attr_id;
    zb_uint8_t attr_zb_status;
    zb_uint8_t attr_state;
    zb_uint8_t attr_type;
    zb_uint8_t attr_value_array[32];
    m2z_device_read_attr_t read_req;
    bool pending_write;
    m2z_device_write_attr_t write_req;
} m2z_device_cluster_attr_t;

/* Attributes of a Cluster of a joined device */
typedef ZB_PACKED_PRE struct m2z_device_cluster_s {
    zb_uint16_t cluster_id;
    bool in_cluster;
    zb_uint8_t disc_attrs_state;
    zb_uint8_t num_attrs;
    m2z_device_cluster_attr_t attribute[MATTER_ZIGBEERCP_BRIDGE_MAX_ATTRS_PER_CLUSTER];
    bool pending_command;
    m2z_device_command_t command_req;
} m2z_device_cluster_t;

/* Description of a EP of a joined device */
typedef ZB_PACKED_PRE struct m2z_device_ep_s {
    zb_uint16_t ep_id;
    zb_uint16_t profile_id;
    zb_uint16_t device_id;
    zb_uint8_t clusters_state;
    zb_uint8_t num_in_clusters;
    zb_uint8_t num_out_clusters;
    m2z_device_cluster_t ep_cluster[MATTER_ZIGBEERCP_BRIDGE_MAX_CLUSTERS_PER_EP];
} m2z_device_ep_t;

/* Joined devices information context */
typedef ZB_PACKED_PRE struct m2z_device_params_s
{
    zb_uint8_t dev_state;
    zb_uint16_t dev_index;
    zb_uint8_t endpoint;
    m2z_device_ep_t endpoints[MATTER_ZIGBEERCP_BRIDGE_MAX_EP_PER_DEV];
    union
    {
        uint64_t u64;
        struct
        {
            uint8_t hasBasic_x0:1;
            uint8_t hasPowerCfg_x1:1;
            uint8_t hasDevTempCfg_x2:1;
            uint8_t hasIdentify_x3:1;
            uint8_t hasGroups_x4:1;
            uint8_t hasScenes_x5:1;
            uint8_t hasOnOff_x6:1;
            uint8_t hasOnOffSwitchCfg_x7:1;
            uint8_t hasLvlCtrl_x8:1;
            uint8_t hasAlarms_x9:1;
            uint8_t hasTime_xa:1;
            uint8_t hasRssiLoc_xb:1;
            uint8_t hasAnalogIn_xc:1;
            uint8_t hasAnalogOut_xd:1;
            uint8_t hasAnalogValue_xe:1;
            uint8_t hasBinaryIn_xf:1;
            uint8_t hasBinaryOut_x10:1;
            uint8_t hasBinaryValue_x11:1;
            uint8_t hasMultiIn_x12:1;
            uint8_t hasMultiOut_x13:1;
            uint8_t hasMultiValue_x14:1;
            uint8_t hasCommissioning_x15:1;
            uint8_t hasOtaUpgrade_x19:1;
            uint8_t hasPollCtrl_x20:1;
            uint8_t hasGreenPower_x21:1;
            uint8_t hasKeepAlive_x25:1;
            uint8_t hasShadeCfg_x100:1;
            uint8_t hasDoorLock_x101:1;
            uint8_t hasWindowCovering_x102:1;
            uint8_t hasPumpCfgCtrl_x200:1;
            uint8_t hasThermostat_x201:1;
            uint8_t hasFanCtrl_x202:1;
            uint8_t hasDehumidCtr_x203:1;
            uint8_t hasThermostatUiCfg_x204:1;
            uint8_t hasColorCtrl_x300:1;
            uint8_t hasBallastCfg_x301:1;
            uint8_t hasIlluminanceMeas_x400:1;
            uint8_t hasTempMeas_x402:1;
            uint8_t hasPressureMeas_x403:1;
            uint8_t hasRelHumidityMeas_x405:1;
            uint8_t hasOccupancySensing_x406:1;
            uint8_t hasCarbonDioMeas_x40D:1;
            uint8_t hasPM2_5Meas_x42a:1;
            uint8_t hasIasZone_x501:1;
            uint8_t hasIasAce_x501:1;
            uint8_t hasIasWd_x502:1;
            uint8_t hasPrice_x700:1;
            uint8_t hasDRLC_x701:1;
            uint8_t hasMetering_x702:1;
            uint8_t hasMessaging_x703:1;
            uint8_t hasTunelling_x704:1;
            uint8_t hasPrepayment_x705:1;
            uint8_t hasNrjMngt_x706:1;
            uint8_t hasCalendar_x707:1;
            uint8_t hasDevMngt_x708:1;
            uint8_t hasEvents_x709:1;
            uint8_t hasMduPairing_x70a:1;
            uint8_t hasSubGhz_x70b:1;
            uint8_t hasDaySched_x70d:1;
            uint8_t hasKeyEstblmt_x800:1;
            uint8_t hasAplianceEvAl_xb02:1;
            uint8_t hasElecMeas_xb04:1;
            uint8_t hasDiags_xb05:1;
            uint8_t hasWwah_xfc57:1;
            uint8_t hasTouchCom_x1000:1;
            uint8_t hasTunnel_xfc00:1;
            uint8_t hasIrBlaster_x1fc01:1;
            uint8_t hasCustomAttr_xffee:1;
            uint8_t hasMeterId_x0b01:1;
            uint8_t hasDirectCfg_x3d:1;
        } bitmap;
    } supported_clusters;

    zb_uint16_t short_addr;
    zb_ieee_addr_t ieee_addr;
    zb_uint8_t pending_toggle;
    zb_uint8_t authorization_type;
    zb_uint8_t authorization_status;
    zb_uint8_t announced_to_bridge;
} m2z_device_params_t;

/* Global device context */
typedef ZB_PACKED_PRE struct m2z_device_ctx_s
{
    m2z_device_params_t devices[MATTER_ZIGBEERCP_BRIDGE_DEV_NUMBER];
} ZB_PACKED_STRUCT m2z_device_ctx_t;

/* Declare type for persisting into nvram context */
typedef m2z_device_ctx_t m2z_device_nvram_dataset_t;

typedef void (* m2z_callback_t)(int MsgType, m2z_device_params_t* ZigbeeRcp_dev, void* data);

void m2z_impl_init();
int m2z_impl_start_threads();
int m2z_register_message_callback(m2z_callback_t callback_function);

void m2z_schedule_request_bdb_start_commissioning();
void m2z_schedule_request_open_network();
void m2z_schedule_request_close_network();
void m2z_request_bdb_start_commissioning(zb_bufid_t param);
void m2z_request_open_network(zb_bufid_t param);
void m2z_request_close_network(zb_bufid_t param);


void m2z_request_ieee_addr(zb_bufid_t param, zb_uint16_t dev_idx);
void m2z_request_attr_disc(zb_bufid_t param, zb_uint16_t dev_idx);
void m2z_request_simple_desc(zb_bufid_t param, zb_uint16_t dev_idx);
void m2z_request_active_ep(zb_bufid_t param, zb_uint16_t dev_idx);

void m2z_schedule_request_ieee_addr(zb_uint16_t dev_idx);
void m2z_schedule_request_attr_disc(zb_uint16_t dev_idx);
void m2z_schedule_request_simple_desc(zb_uint16_t dev_idx);
void m2z_schedule_request_active_ep(zb_uint16_t dev_idx);
void m2z_schedule_read_attribute(m2z_device_params_t *dev, zb_uint16_t cluster_id, zb_uint16_t attr_id);
void m2z_schedule_write_attribute(m2z_device_params_t *dev, zb_uint16_t cluster_id, zb_uint16_t attr_id, zb_uint8_t* p_attr_value);
void m2z_schedule_send_command(m2z_device_params_t *dev, zb_uint16_t cluster_id, zb_uint16_t cmd_id, zb_uint8_t *cmd_data, zb_uint8_t cmd_size);
void m2z_request_send_command(zb_bufid_t param, zb_uint16_t dev_index);
void m2z_request_read_attribute(zb_bufid_t param, zb_uint16_t dev_index);
void m2z_request_write_attribute(zb_bufid_t param, zb_uint16_t dev_index);

void m2z_get_attribute_zb_status(m2z_device_params_t *dev, zb_uint16_t cluster_id, zb_uint16_t attr_id, zb_uint8_t *attr_zb_status);

void m2z_handler_disc_attr_resp(zb_bufid_t param, zb_zcl_parsed_hdr_t *cmd_info);
void m2z_callback_dev_attr_disc(zb_uint8_t param);
void m2z_callback_dev_simple_desc(zb_bufid_t param);
void m2z_callback_dev_active_ep(zb_bufid_t param);
void m2z_callback_dev_ieee_addr(zb_uint8_t param);
void m2z_update_endpoint_supported_clusters(m2z_device_params_t *device, uint16_t zcl_cluster_id);

/* Startup sequence routines */
void m2z_callback_dev_annce(zb_zdo_signal_device_annce_params_t *);
void m2z_callback_dev_authorized(zb_zdo_signal_device_authorized_params_t *);
void m2z_callback_dev_updated(zb_zdo_signal_device_update_params_t *);
void m2z_callback_dev_associated(zb_nwk_signal_device_associated_params_t *);
void m2z_callback_dev_leave_indication(zb_ieee_addr_t dev_addr);

/* Devices discovery routines */
void find_onoff_device(zb_uint8_t param, zb_uint16_t dev_idx);
void find_onoff_device_delayed(zb_uint8_t param);
void find_onoff_device_cb(zb_uint8_t param);
void find_onoff_device_tmo(zb_uint8_t param);

void m2z_request_dev_ieee_addr(zb_uint8_t param, zb_uint16_t dev_idx);

void bind_device(zb_uint8_t param, zb_uint16_t dev_idx);
void bind_device_cb(zb_uint8_t param);

void configure_reporting(zb_uint8_t param, zb_uint16_t dev_idx);
void configure_reporting_cb(zb_uint8_t param);

/* Devices management routines */
zb_uint16_t m2z_dev_get_index_by_state(zb_uint8_t dev_state);
zb_uint16_t m2z_dev_get_index_by_short_addr(zb_uint16_t short_addr);
zb_uint16_t m2z_dev_get_index_by_ieee_addr(zb_ieee_addr_t ieee_addr);

zb_uint16_t m2z_dev_get_ep_idx_by_short_addr_and_ep_id(zb_uint16_t short_addr, zb_uint16_t ep_id);
zb_uint16_t m2z_dev_get_ep_id_by_short_addr_and_cluster_id(zb_uint16_t short_addr, zb_uint16_t cluster_id);
zb_uint16_t m2z_dev_get_cluster_idx_by_short_addr_and_ep_id_and_cluster_id(zb_uint16_t short_addr, zb_uint16_t ep_id, zb_uint16_t cluster_id);
zb_uint16_t m2z_dev_get_attr_idx_by_short_addr_and_ep_id_and_cluster_id(zb_uint16_t short_addr, zb_uint16_t ep_id, zb_uint16_t cluster_id, zb_uint16_t attr_id);

void m2z_remove_and_rejoin_device_delayed(zb_uint8_t idx);
void m2z_remove_device_delayed(zb_uint8_t idx);

/* Persisting data into NVRAM routines */
zb_uint16_t m2z_get_nvram_data_size();
void m2z_nvram_read_app_data(zb_uint8_t page, zb_uint32_t pos, zb_uint16_t payload_length);
zb_ret_t m2z_nvram_write_app_data(zb_uint8_t page, zb_uint32_t pos);

/* Handler for specific zcl commands */
zb_uint8_t zcl_specific_cluster_cmd_handler(zb_uint8_t param);


/* Macros used in test loop */

#endif /* MATTER_ZIGBEERCP_BRIDGE_H */
