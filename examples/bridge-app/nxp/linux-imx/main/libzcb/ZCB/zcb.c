// ------------------------------------------------------------------
// ZCB
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

/** \addtogroup zb
 * \file
 * \brief ZCB functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "zcb.h"
#include "nibbles.h"
#include "systemtable.h"
#include "newDb.h"
#include "dump.h"
#include "ZigbeeConstant.h"
#include "SerialLink.h"
#include "ZigbeeDevices.h"

#include "zcb_main.h"
#include "ZcbMessage.h"

// ---------------------------------------------------------------
// Macros
// ---------------------------------------------------------------

// #define ZCB_DEBUG

#ifdef ZCB_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* ZCB_DEBUG */

// ---------------------------------------------------------------------
// Send message to Matter
// ---------------------------------------------------------------------
teZcbStatus eZCB_SendMsg(int MsgType, newdb_zcb_t *Zcb, void* data)
{
    pthread_mutex_lock(&ZcbMsg.bridge_mutex);

    ZcbMsg.HandleMask = false;
    pthread_cond_signal(&ZcbMsg.bridge_cond);

    if( Zcb != NULL ) {
        ZcbMsg.zcb = *Zcb;
        ZcbMsg.msg_type = MsgType;
        ZcbMsg.msg_data = data;
    } else {
        ZcbMsg.msg_type = MsgType;
    }

    pthread_mutex_unlock(&ZcbMsg.bridge_mutex);
}

// ---------------------------------------------------------------------
// Send announce message to IoT
// ---------------------------------------------------------------------

#define DEVICE_ENTRY(id, profile, mappedId) \
{id, CZD_PROFILE_ID_##profile,CZD_DEVICE_VERSION_##profile, mappedId}

/* Only for devices for which there is a conflicting ID. */
tsDeviceMapEntry saDeviceMap[] =
    {
        DEVICE_ENTRY(0x0000, ZLL, SIMPLE_DESCR_SWITCH_ONOFF),
        DEVICE_ENTRY(0x0000, ZHA, SIMPLE_DESCR_SWITCH_ONOFF),
        DEVICE_ENTRY(0x0000, ZLO, SIMPLE_DESCR_SWITCH_ONOFF),
        DEVICE_ENTRY(0x0100, ZLL, SIMPLE_DESCR_LAMP_DIMM),
        DEVICE_ENTRY(0x0100, ZHA, SIMPLE_DESCR_LAMP_DIMM),
        DEVICE_ENTRY(0x010C, ZLO, SIMPLE_DESCR_LAMP_COLOUR_TEMP),
        DEVICE_ENTRY(0x010D, ZLO, SIMPLE_DESCR_LAMP_COLOUR),
        DEVICE_ENTRY(0x01ff, ZLL, SIMPLE_DESCR_LAMP_COLOUR_TEMP), // private ID for prototype
        DEVICE_ENTRY(0x01ff, ZHA, SIMPLE_DESCR_LAMP_COLOUR_TEMP), // private ID for prototype
        DEVICE_ENTRY(0x0200, ZLL, SIMPLE_DESCR_LAMP_COLOUR),
        DEVICE_ENTRY(0x0200, ZHA, SIMPLE_DESCR_LAMP_COLOUR),
        DEVICE_ENTRY(0x0210, ZLL, SIMPLE_DESCR_LAMP_COLOUR),
        DEVICE_ENTRY(0x0210, ZHA, SIMPLE_DESCR_LAMP_COLOUR),
    };

/**
 * \brief Handle device announces depending on the Simple Descriptor device ID.
 * In case of a plug meter, also the update interval is set to a few seconds.
 * \param u64IEEEAddress Mac address of the device
 * \param u16DeviceID Simple descriptor device ID
 */

void announceDevice(
    uint64_t u64IEEEAddress,
    uint16_t u16DeviceId,
    uint16_t u16ProfileId,
    uint8_t  u8DeviceVersion)
{
    DEBUG_PRINTF( "Announce --------- Device 0x%llx, DeviceId = 0x%04x\n", u64IEEEAddress, u16DeviceId );

    char  mac[LEN_MAC_NIBBLE+2];
    char  caType[LEN_TY+2];
    int i;
    tsDeviceMapEntry * psDevice;
    newdb_zcb_t        sZcb;

    psDevice = &saDeviceMap[0];
    u642nibblestr( u64IEEEAddress, mac );
    newDbGetZcb(mac, &sZcb);

    /* If case of Id overlap, look up for device Id to use */
    for (i = 0;
        i < (sizeof(saDeviceMap)/sizeof(tsDeviceMapEntry));
        i++)
    {
      if (psDevice->u16Id == u16DeviceId &&
          psDevice->u16ProfileId == u16ProfileId &&
          psDevice->u8DeviceVersion == u8DeviceVersion)
      {
        u16DeviceId = psDevice->u16ResultingId;
      }
      psDevice++;
    }

    switch ( u16DeviceId ) {
    case SIMPLE_DESCR_CONTROL_BRIDGE:
        break;

    case SIMPLE_DESCR_SWITCH_ONOFF:
        zcbHandleAnnounce( mac, "swi", "onoff" );
        break;

    case SIMPLE_DESCR_SWITCH_DIMM:
        zcbHandleAnnounce( mac, "swi", "dim" );
        break;

    case SIMPLE_DESCR_SWITCH_COLL_DIMM:
        zcbHandleAnnounce( mac, "swi", "col" );
        break;

    case SIMPLE_DESCR_LAMP_ONOFF_ZLL:     // 0x0000
    case SIMPLE_DESCR_LAMP_ONOFF:         // 0x0100
        zcbHandleAnnounce( mac, "lmp", "onoff" );
        eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_ONOFF );
        break;

    case SIMPLE_DESCR_LAMP_DIMM:              // 0x0101
    case SIMPLE_DESCR_LAMP_DIMM_OFF:          // 0x0061
        zcbHandleAnnounce( mac, "lmp", "dim" );
        eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_ONOFF );
        // eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_LEVEL_CONTROL );
        break;

    case SIMPLE_DESCR_LAMP_COLOUR:       // 0x0102
    case SIMPLE_DESCR_LAMP_COLOUR_DIMM:  // 0x0200 ZLL
    case SIMPLE_DESCR_LAMP_COLOUR_EXT:   // 0x0210 ZLL
        zcbHandleAnnounce( mac, "lmp", "col" );
        eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_ONOFF );
        // eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_LEVEL_CONTROL );
        // eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_COLOR_CONTROL );
        break;

    case SIMPLE_DESCR_LAMP_COLOUR_TEMP:
//    case SIMPLE_DESCR_LAMP_CCTW:
        zcbHandleAnnounce( mac, "lmp", "tw" );
        eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_ONOFF );
        // eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_LEVEL_CONTROL );
        // eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_COLOR_CONTROL );
        break;

    case SIMPLE_DESCR_HVAC_HC_UNIT:
        zcbHandleAnnounce( mac, "man", NULL );
        break;

    case SIMPLE_DESCR_THERMOSTAT:
        zcbHandleAnnounce( mac, "ui", NULL );
        break;

    case SIMPLE_DESCR_SIMPLE_SENSOR:
        zcbHandleAnnounce( mac, "sen", NULL );
        break;

    case SIMPLE_DESCR_HVAC_PUMP:
        zcbHandleAnnounce( mac, "pmp", NULL );
        break;

    case SIMPLE_DESCR_SMART_PLUG:
        zcbHandleAnnounce( mac, "plg", NULL );
        eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_ONOFF );
        eZCB_SendBindCommand( u64IEEEAddress, E_ZB_CLUSTERID_SIMPLE_METERING );
        SmartPlugUpdateIntervalMsg( u64IEEEAddress, 4 );	// was 2
        break;

    case SIMPLE_DESCR_OCCUPANCY_SENSOR:
        eZCB_SendBindCommand(u64IEEEAddress, E_ZB_CLUSTERID_OCCUPANCYSENSING);
        strcpy(caType,"occ");
        if (sZcb.uSupportedClusters.sClusterBitmap.hasIlluminanceSensing)
        {
          strcat(caType, "+L");
          eZCB_SendBindCommand( u64IEEEAddress,
              E_ZB_CLUSTERID_MEASUREMENTSENSING_ILLUM);
        }
        if (sZcb.uSupportedClusters.sClusterBitmap.hasTemperatureSensing)
        {
          strcat(caType, "+T");
          eZCB_SendBindCommand( u64IEEEAddress,
              E_ZB_CLUSTERID_MEASUREMENTSENSING_TEMP);
        }
        zcbHandleAnnounce(mac, "sen", caType);
        break;

    case SIMPLE_DESCR_LIGHT_SENSOR:
        // eZCB_SendBindCommand(u64IEEEAddress, E_ZB_CLUSTERID_MEASUREMENTSENSING_ILLUM);
        strcpy(caType,"Ill");
        if (sZcb.uSupportedClusters.sClusterBitmap.hasOccupancySensing)
        {
          strcat(caType, "+O");
        //   eZCB_SendBindCommand( u64IEEEAddress,
        //       E_ZB_CLUSTERID_OCCUPANCYSENSING);
        }
        if (sZcb.uSupportedClusters.sClusterBitmap.hasTemperatureSensing)
        {
          strcat(caType, "+T");
          eZCB_SendBindCommand( u64IEEEAddress,
              E_ZB_CLUSTERID_MEASUREMENTSENSING_TEMP);
        }
        zcbHandleAnnounce(mac, "sen", caType);
        break;

    default:
        printf( "error: Illegal simple descriptor (0x%02x)\n", u16DeviceId );
        break;
    }
// #endif

}

// ------------------------------------------------------------------
// Handle incoming attributes
// ------------------------------------------------------------------

/**
 * \brief Handles an incoming attribute message
 * \param u16ShortAddress Short address of the device
 * \param u16ClusterID Cluster ID of the attribute
 * \param u16AttributeID Attribute ID
 * \param u64Data Data containes
 * \param u8Endpoint Originating endpoint
 */
void handleAttribute( uint16_t u16ShortAddress,
                      uint16_t u16ClusterID,
                      uint16_t u16AttributeID,
                      uint64_t u64Data,
                      uint8_t  u8Endpoint ) {

    uint64_t u64IEEEAddress = 0;
    newdb_zcb_t zcb;
    ZcbAttribute_t *msg_data = (ZcbAttribute_t *)malloc(sizeof(ZcbAttribute_t));

    if ( newDbGetZcbSaddr( u16ShortAddress, &zcb ) ) {
        u64IEEEAddress = nibblestr2u64( zcb.mac );
    }

    if ( u64IEEEAddress ) {
        switch ( u16ClusterID ) {

        case E_ZB_CLUSTERID_ONOFF:
            switch ( u16AttributeID ) {
            case E_ZB_ATTRIBUTEID_ONOFF_ONOFF:
                {
                    sendActuatorOnOff( u64IEEEAddress, u8Endpoint, (int)u64Data );
                    printf( "get onoff attribute report with u16AttributeID : %x, u64Data : %d\n", u16AttributeID, u64Data);
                    strncpy(zcb.info, (u64Data ? "on" : "off"), LEN_CMD);
                    newDbSetZcb(&zcb);
                }
                break;

            default:
                printf( "Received attribute 0x%04x in ONOFF cluster\n", u16AttributeID );
                break;
            }
            break;

	    case E_ZB_CLUSTERID_MEASUREMENTSENSING_TEMP:
            switch ( u16AttributeID ) {
            case E_ZB_ATTRIBUTEID_MS_TEMP_MEASURED:
                {
                    printf( "got measurement temp attribute report with gDeviceID : %d, u64Data : %ld\n", zcb.matterIndex, u64Data);
                    ZcbAttribute_t *msg_data = (ZcbAttribute_t *)malloc(sizeof(ZcbAttribute_t));
                    msg_data->u16ClusterID = u16ClusterID;
                    msg_data->u16AttributeID = u16AttributeID;
                    msg_data->u64Data = u64Data;
                    eZCB_SendMsg(BRIDGE_WRITE_ATTRIBUTE, &zcb, msg_data);
                }
                break;

            default:
                printf( "Received attribute 0x%04x in MEASUREMENTSENSING_TEMP cluster\n", u16AttributeID );
                break;
            }
            break;

        default:
            printf( "Received attribute 0x%04x in cluster 0x%04x\n",
                    u16AttributeID, u16ClusterID );
            break;
        }
    }
}

// ---------------------------------------------------------------
// Local Function Prototypes
// ---------------------------------------------------------------
static void ZCB_HandleVersionResponse           (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleNodeClusterList           (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleNodeClusterAttributeList  (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleNodeCommandIDList         (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleNetworkJoined             (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleDeviceAnnounce            (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleDeviceLeave               (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleMatchDescriptorResponse   (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleAttributeReport           (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleSimpleDescriptorResponse  (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleAddGroupResponse          (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleDefaultResponse           (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleRemoveGroupMembershipResponse (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleRemoveSceneResponse       (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleStoreSceneResponse        (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleReadAttrResp              (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleActiveEndPointResp        (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleZcbStatusResponse         (void *pvUser, uint16_t u16Length, void *pvMessage);
static void ZCB_HandleLog                       (void *pvUser, uint16_t u16Length, void *pvMessage);

// ---------------------------------------------------------------
// Helper Functions
// ---------------------------------------------------------------

// --------------------------------------------------------------------
// Add node
// --------------------------------------------------------------------
/*!
 * Updates database when a new node has joined the network.
 * Creates the node in the database if required.
 * @param shortAddress
 * @param extendedAddress
 * @return 1 if a new node is created, 0 else
 */
int zcbAddNode( uint16_t  shortAddress, uint64_t extendedAddress ) {
    DEBUG_PRINTF( "Add node 0x%04x, 0x%016llx\n",
            (int)shortAddress, (long long unsigned int)extendedAddress );
    int iReturn = 1;
    char  mac[LEN_MAC_NIBBLE+2];
    u642nibblestr( extendedAddress, mac );

    newdb_zcb_t zcb;
    if ( newDbGetZcbSaddr( shortAddress, &zcb ) ) {
        zcb.status = ZCB_STATUS_JOINED;
        strcpy( zcb.mac, mac );
        newDbSetZcb( &zcb );

    } else if ( newDbGetZcb( mac, &zcb ) ) {
        zcb.status = ZCB_STATUS_JOINED;
        zcb.saddr  = shortAddress;
        newDbSetZcb( &zcb );

    } else if ( newDbGetNewZcb( mac, &zcb ) ) {
        zcb.status = ZCB_STATUS_JOINED;
        zcb.saddr  = shortAddress;
        zcb.type   = SIMPLE_DESCR_UNKNOWN;
        newDbSetZcb( &zcb );
        iReturn = 0;
    }

    return iReturn;
}

uint16_t zcbNodeGetShortAddress( char * mac ) {
    DEBUG_PRINTF( "Get node short address for 0x%s\n", mac );

    newdb_zcb_t zcb;
    if ( newDbGetZcb( mac, &zcb ) ) {
        return zcb.saddr;
    }
    return 0xFFFF;
}

uint64_t zcbNodeGetExtendedAddress( uint16_t shortAddress ) {
    DEBUG_PRINTF( "Get node extended address for 0x%04x\n",
            (int)shortAddress );

    newdb_zcb_t zcb;
    if ( newDbGetZcbSaddr( shortAddress, &zcb ) ) {

        uint64_t u64mac = nibblestr2u64( zcb.mac );
        return u64mac;
    }

    return 0;
}


int zcbNodeLeft( uint64_t extendedAddress, int keep ) {
    DEBUG_PRINTF( "Left node 0x%016llx, %d\n",
            (long long unsigned int)extendedAddress, keep );

    char  mac[LEN_MAC_NIBBLE+2];
    u642nibblestr( extendedAddress, mac );

    newdb_zcb_t zcb;
    if ( newDbGetZcb( mac, &zcb ) ) {
        eZCB_SendMsg(BRIDGE_REMOVE_DEV, &zcb, NULL);
        zcb.status = (keep) ? ZCB_STATUS_LEFT : ZCB_STATUS_FREE;
        newDbSetZcb( &zcb );
#ifdef PERSIST
        newDbSave();
#endif
        return 1;
    }
    return 0;
}

int zcbNodeSetDeviceID( uint16_t shortAddress, uint16_t deviceID ) {
    DEBUG_PRINTF( "Set DeviceID 0x%04x to node 0x%04x\n",
            (int)deviceID, (int)shortAddress );

    newdb_zcb_t zcb;
    if ( newDbGetZcbSaddr( shortAddress, &zcb ) ) {
        if ( zcb.type != deviceID ) {
            // Only act when a change is necessary
            zcb.type = deviceID;
            newDbSetZcb( &zcb );
        }
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------
// Exported Functions
// ---------------------------------------------------------------

teZcbStatus eZCB_Init(char *cpSerialDevice, uint32_t u32BaudRate) {

    if (eSL_Init(cpSerialDevice, u32BaudRate) != E_SL_OK) {
        return E_ZCB_COMMS_FAILED;
    }

    /* Register listeners */
    eSL_AddListener(E_SL_MSG_VERSION_LIST,               ZCB_HandleVersionResponse,          NULL);
    eSL_AddListener(E_SL_MSG_NODE_CLUSTER_LIST,          ZCB_HandleNodeClusterList,          NULL);
    eSL_AddListener(E_SL_MSG_NODE_ATTRIBUTE_LIST,        ZCB_HandleNodeClusterAttributeList, NULL);
    eSL_AddListener(E_SL_MSG_NODE_COMMAND_ID_LIST,       ZCB_HandleNodeCommandIDList,        NULL);
    eSL_AddListener(E_SL_MSG_NETWORK_JOINED_FORMED,      ZCB_HandleNetworkJoined,            NULL);
    eSL_AddListener(E_SL_MSG_DEVICE_ANNOUNCE,            ZCB_HandleDeviceAnnounce,           NULL);
    eSL_AddListener(E_SL_MSG_LEAVE_INDICATION,           ZCB_HandleDeviceLeave,              NULL);
    eSL_AddListener(E_SL_MSG_MATCH_DESCRIPTOR_RESPONSE,  ZCB_HandleMatchDescriptorResponse,  NULL);
    eSL_AddListener(E_SL_MSG_ATTRIBUTE_REPORT,           ZCB_HandleAttributeReport,          NULL);
    eSL_AddListener(E_SL_MSG_SIMPLE_DESCRIPTOR_RESPONSE, ZCB_HandleSimpleDescriptorResponse, NULL);
    eSL_AddListener(E_SL_MSG_ADD_GROUP_RESPONSE,         ZCB_HandleAddGroupResponse,         NULL);
    eSL_AddListener(E_SL_MSG_DEFAULT_RESPONSE,           ZCB_HandleDefaultResponse,          NULL);
    eSL_AddListener(E_SL_MSG_REMOVE_GROUP_RESPONSE,      ZCB_HandleRemoveGroupMembershipResponse, NULL);
    eSL_AddListener(E_SL_MSG_REMOVE_SCENE_RESPONSE,      ZCB_HandleRemoveSceneResponse,      NULL);
    eSL_AddListener(E_SL_MSG_STORE_SCENE_RESPONSE,       ZCB_HandleStoreSceneResponse,       NULL);
    eSL_AddListener(E_SL_MSG_READ_ATTRIBUTE_RESPONSE,    ZCB_HandleReadAttrResp,             NULL);
    eSL_AddListener(E_SL_MSG_ACTIVE_ENDPOINT_RESPONSE,   ZDActiveEndPointResp,               NULL);
    eSL_AddListener(E_SL_MSG_GET_ZCB_STATUS_RESPONSE,    ZCB_HandleZcbStatusResponse,        NULL);
    eSL_AddListener(E_SL_MSG_LOG,                        ZCB_HandleLog,                      NULL);

    return E_ZCB_OK;
}


teZcbStatus eZCB_Finish(void) {
    eSL_Destroy();

    return E_ZCB_OK;
}


teZcbStatus eZCB_EstablishComms(void) {
#if 0
    if (eSL_SendMessage(E_SL_MSG_GET_VERSION, 0, NULL, NULL) == E_SL_OK) {
        uint16_t u16Length;
        uint32_t  *u32Version;

        /* Wait 300ms for the version message to arrive */
        if (eSL_MessageWait(E_SL_MSG_VERSION_LIST, 300, &u16Length, (void**)&u32Version) == E_SL_OK) {
            uint32_t version = ntohl(*u32Version);
            newDbSystemSaveIntval( "zcb_version", version );

            DEBUG_PRINTF( "Connected to control bridge version 0x%08x\n", version );
            free(u32Version);

            DEBUG_PRINTF( "Reset control bridge\n");
            if (eSL_SendMessage(E_SL_MSG_RESET, 0, NULL, NULL) != E_SL_OK) {
                return E_ZCB_COMMS_FAILED;
            }
            return E_ZCB_OK;
        }
        else
        {
            DEBUG_PRINTF( "Missing answer: Version List\n");
        }
    }

    return E_ZCB_COMMS_FAILED;
#endif
    if (eSL_SendMessage(E_SL_MSG_GET_VERSION, 0, NULL, NULL) == E_SL_OK)
	    return E_ZCB_OK;
    else
    {
        DEBUG_PRINTF( "serial port error\n");
	    usleep(1000*1000);
	    return E_ZCB_COMMS_FAILED;
    }
}


teZcbStatus eOnOff( uint16_t u16ShortAddress, uint8_t u8Mode ) {
    uint8_t             u8SequenceNo;
    teSL_Status         eStatus;

    struct {
        uint8_t     u8TargetAddressMode;
        uint16_t    u16TargetAddress;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
        uint8_t     u8Mode;
    } __attribute__((__packed__)) sOnOffMessage;

    DEBUG_PRINTF( "%s :On/Off (Set Mode=%d)\n", __FUNCTION__, u8Mode);

    if (u8Mode > 2) {
        /* Illegal value */
        return E_ZCB_ERROR;
    }

    sOnOffMessage.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_SHORT_NO_ACK;
    sOnOffMessage.u16TargetAddress      = htons(u16ShortAddress);
    sOnOffMessage.u8SourceEndpoint      = ZB_ENDPOINT_ONOFF;
    sOnOffMessage.u8DestinationEndpoint = ZB_ENDPOINT_ONOFF;

    sOnOffMessage.u8Mode = u8Mode;
    eStatus = eSL_SendMessage(E_SL_MSG_ONOFF, sizeof(sOnOffMessage),
        &sOnOffMessage, &u8SequenceNo);

    if (eStatus != E_SL_OK)
    {
      DEBUG_PRINTF( "Sending of Command '%s' failed (0x%02x)\n",
          (u8Mode? "On" : "Off"),
          eStatus);
      return E_ZCB_COMMS_FAILED;
    }

    return E_ZCB_OK;
}


teZcbStatus SimpleDescriptorRequest(uint16_t u16ShortAddress) {

    struct _SimpleDescriptorRequest {
        uint16_t    u16TargetAddress;
        uint8_t     u8Endpoint;
    } __attribute__((__packed__)) sSimpleDescriptorRequest;

    uint8_t u8SequenceNo;
    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;
    teSL_Status eSlStatus = E_SL_NOMESSAGE;
    uint8_t u8Retry = 3;

    DEBUG_PRINTF( "Send Simple Descriptor request to 0x%04X\n", u16ShortAddress);

    sSimpleDescriptorRequest.u16TargetAddress       = htons(u16ShortAddress);
    sSimpleDescriptorRequest.u8Endpoint             = ZB_ENDPOINT_SIMPLE;
    /*
     * Special processing in case there is noanswer from control bridge
     */
    do
    {
      eSlStatus = eSL_SendMessage(E_SL_MSG_SIMPLE_DESCRIPTOR_REQUEST,
          sizeof(struct _SimpleDescriptorRequest),
          &sSimpleDescriptorRequest, &u8SequenceNo);
      if (eSlStatus == E_SL_OK)
      {
          eStatus = E_ZCB_OK;
          u8Retry = 0;

      }
      else
      {
        DEBUG_PRINTF("eSL_SendMessage : error %i\n", eSlStatus);
        if (eSlStatus == E_SL_NOMESSAGE)
        {
          /* wait a bit */
          usleep( 500 * 1000 );
          /* Try sending again */
          DEBUG_PRINTF("eSL_SendMessage : trying again...\n");
          u8Retry--;
        }
      }
    }
    while (u8Retry);
    return eStatus;
}


// ------------------------------------------------------------------
// Check Neighbours
// ------------------------------------------------------------------

int eZCB_NeighbourTableRequest( int start ) {
    struct _ManagementLQIRequest
    {
        uint16_t    u16TargetAddress;
        uint8_t     u8StartIndex;
    } __attribute__((__packed__)) sManagementLQIRequest;

    struct _ManagementLQIResponse
    {
        uint8_t     u8SequenceNo;
        uint8_t     u8Status;
        uint8_t     u8NeighbourTableSize;
        uint8_t     u8TableEntries;
        uint8_t     u8StartIndex;
        struct
        {
            uint16_t    u16ShortAddress;
            uint64_t    u64PanID;
            uint64_t    u64IEEEAddress;
            uint8_t     u8Depth;
            uint8_t     u8LQI;
            struct
            {
                unsigned    uDeviceType : 2;
                unsigned    uPermitJoining: 2;
                unsigned    uRelationship : 2;
                unsigned    uMacCapability : 2;
            } __attribute__((__packed__)) sBitmap;
        } __attribute__((__packed__)) asNeighbours[2];
    } __attribute__((__packed__)) *psManagementLQIResponse = NULL;

    uint16_t u16ShortAddress;
    uint16_t u16Length;
    uint8_t u8SequenceNo;
    int i, ret = -1;
    teSL_Status eStatus;

    u16ShortAddress = 0;    // GW

    sManagementLQIRequest.u16TargetAddress = htons(u16ShortAddress);
    sManagementLQIRequest.u8StartIndex     = start;

    time_t t = time(NULL);
    char * szTimeStamp = ctime(&t);

    DEBUG_PRINTF( "%s:Send management LQI request to 0x%04X for entries starting at %d\n",
        szTimeStamp, u16ShortAddress, sManagementLQIRequest.u8StartIndex);
    eStatus = eSL_SendMessage(E_SL_MSG_MANAGEMENT_LQI_REQUEST,
        sizeof(struct _ManagementLQIRequest), &sManagementLQIRequest, &u8SequenceNo);

    if ( eStatus != E_SL_OK)
    {
      DEBUG_PRINTF( "Error sending management LQI request : %d (%s)\n",
          eStatus,
          eSL_PrintError(eStatus));
        goto done;
    }

    while (1)
    {
        /* Wait 1 second for the message to arrive */
        if (eSL_MessageWait(E_SL_MSG_MANAGEMENT_LQI_RESPONSE, 1000, &u16Length,
            (void**)&psManagementLQIResponse) != E_SL_OK)
        {
            DEBUG_PRINTF( "No response to management LQI request");
            goto done;
        }
        else if (u8SequenceNo == psManagementLQIResponse->u8SequenceNo)
        {
            break;
        }
        else
        {
            DEBUG_PRINTF( "IEEE Address sequence number received 0x%02X does not match that sent 0x%02X\n",
                          psManagementLQIResponse->u8SequenceNo, u8SequenceNo);
            free(psManagementLQIResponse);
            psManagementLQIResponse = NULL;
        }
    }
    if (psManagementLQIResponse->u8Status == CZD_NW_STATUS_SUCCESS)
    {
      DEBUG_PRINTF( "Received management LQI response. Table size: %d, Entry count: %d, start index: %d\n",
          psManagementLQIResponse->u8NeighbourTableSize,
          psManagementLQIResponse->u8TableEntries,
          psManagementLQIResponse->u8StartIndex);

      ret = 0;
      for (i = 0; i < psManagementLQIResponse->u8TableEntries; i++)
      {
        psManagementLQIResponse->asNeighbours[i].u16ShortAddress    = ntohs(psManagementLQIResponse->asNeighbours[i].u16ShortAddress);
        psManagementLQIResponse->asNeighbours[i].u64PanID           = be64toh(psManagementLQIResponse->asNeighbours[i].u64PanID);
        psManagementLQIResponse->asNeighbours[i].u64IEEEAddress     = be64toh(psManagementLQIResponse->asNeighbours[i].u64IEEEAddress);

        if ((psManagementLQIResponse->asNeighbours[i].u16ShortAddress >= 0xFFFA) ||
            (psManagementLQIResponse->asNeighbours[i].u64IEEEAddress  == 0))
        {
          /* Illegal short / IEEE address */
          continue;
        }

        DEBUG_PRINTF( "  Entry %02d: Short Address 0x%04X, PAN ID: 0x%016llX, IEEE Address: 0x%016llX\n", i,
            psManagementLQIResponse->asNeighbours[i].u16ShortAddress,
            (unsigned long long int)psManagementLQIResponse->asNeighbours[i].u64PanID,
            (unsigned long long int)psManagementLQIResponse->asNeighbours[i].u64IEEEAddress);

        DEBUG_PRINTF( "    Type: %d, Permit Joining: %d, Relationship: %d, RxOnWhenIdle: %d\n",
            psManagementLQIResponse->asNeighbours[i].sBitmap.uDeviceType,
            psManagementLQIResponse->asNeighbours[i].sBitmap.uPermitJoining,
            psManagementLQIResponse->asNeighbours[i].sBitmap.uRelationship,
            psManagementLQIResponse->asNeighbours[i].sBitmap.uMacCapability);

        DEBUG_PRINTF( "    Depth: %d, LQI: %d\n",
            psManagementLQIResponse->asNeighbours[i].u8Depth,
            psManagementLQIResponse->asNeighbours[i].u8LQI);

        newdb_zcb_t zcb;
        if ( newDbGetZcbSaddr( psManagementLQIResponse->asNeighbours[i].u16ShortAddress, &zcb ) ) {
          // Existing node
        } else {
          // New node
          DEBUG_PRINTF( "New Node 0x%04X in neighbour table\n", psManagementLQIResponse->asNeighbours[i].u16ShortAddress);

          // Handle as the device announced itself
          zcbAddNode( psManagementLQIResponse->asNeighbours[i].u16ShortAddress,
              psManagementLQIResponse->asNeighbours[i].u64IEEEAddress );
          if ( SimpleDescriptorRequest( psManagementLQIResponse->asNeighbours[i].u16ShortAddress ) != E_ZCB_OK) {
            DEBUG_PRINTF( "Error sending simple descriptor request\n");
            ret = -1;
          }
        }
      }

      if (psManagementLQIResponse->u8TableEntries > 0)
      {
        // We got some entries, so next time request the entries after these.
        start += psManagementLQIResponse->u8TableEntries;
        /* Make sure we're still in table boundaries */
        if (start >= psManagementLQIResponse->u8NeighbourTableSize)
        {
          start = 0;
        }
      }
      else
      {
        // No more valid entries.
        start = 0;
      }
      if ( ret == 0 ) ret = start;
    } // if (psManagementLQIResponse->u8Status == CZD_NW_STATUS_SUCCESS)
    else
    {
      DEBUG_PRINTF( "Received error status in management LQI response : 0x%02x\n",
          psManagementLQIResponse->u8Status);
    }
done:
    free(psManagementLQIResponse);
    return( ret );
}

// ------------------------------------------------------------------
// Write attribute
// ------------------------------------------------------------------

teZcbStatus eZCB_WriteAttributeRequest(uint16_t u16ShortAddress,
                                    uint16_t u16ClusterID,
                                    uint8_t u8Direction,
                                    uint8_t u8ManufacturerSpecific,
                                    uint16_t u16ManufacturerID,
                                    uint16_t u16AttributeID,
                                    teZCL_ZCLAttributeType eType,
                                    void *pvData)
{
    struct _WriteAttributeRequest
    {
        uint8_t     u8TargetAddressMode;
        uint16_t    u16TargetAddress;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
        uint16_t    u16ClusterID;
        uint8_t     u8Direction;
        uint8_t     u8ManufacturerSpecific;
        uint16_t    u16ManufacturerID;
        uint8_t     u8NumAttributes;
        uint16_t    u16AttributeID;
        uint8_t     u8Type;
        union
        {
            uint8_t     u8Data;
            uint16_t    u16Data;
            uint32_t    u32Data;
            uint64_t    u64Data;
        } uData;
    } __attribute__((__packed__)) sWriteAttributeRequest;

#if 0
    struct _WriteAttributeResponse
    {
        /**\todo ZCB-Sheffield: handle default response properly */
        uint8_t     au8ZCLHeader[3];
        uint16_t    u16MessageType;

        uint8_t     u8SequenceNo;
        uint16_t    u16ShortAddress;
        uint8_t     u8Endpoint;
        uint16_t    u16ClusterID;
        uint16_t    u16AttributeID;
        uint8_t     u8Status;
        uint8_t     u8Type;
        union
        {
            uint8_t     u8Data;
            uint16_t    u16Data;
            uint32_t    u32Data;
            uint64_t    u64Data;
        } uData;
    } __attribute__((__packed__)) *psWriteAttributeResponse = NULL;
#endif

    struct _DataIndication
    {
        /**\todo ZCB-Sheffield: handle data indication properly */
        uint8_t     u8ZCBStatus;
        uint16_t    u16ProfileID;
        uint16_t    u16ClusterID;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
        uint8_t     u8SourceAddressMode;
        uint16_t    u16SourceShortAddress; /* OR uint64_t u64IEEEAddress */
        uint8_t     u8DestinationAddressMode;
        uint16_t    u16DestinationShortAddress; /* OR uint64_t u64IEEEAddress */

        uint8_t     u8FrameControl;
        uint8_t     u8SequenceNo;
        uint8_t     u8Command;
        uint8_t     u8Status;
        uint16_t    u16AttributeID;
    } __attribute__((__packed__)) *psDataIndication = NULL;


    uint16_t u16Length = sizeof(struct _WriteAttributeRequest) - sizeof(sWriteAttributeRequest.uData);
    uint8_t u8SequenceNo;
    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;

    DEBUG_PRINTF("Send Write Attribute request to 0x%04X\n", u16ShortAddress);

    sWriteAttributeRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_SHORT_NO_ACK;
    sWriteAttributeRequest.u16TargetAddress      = htons(u16ShortAddress);
    sWriteAttributeRequest.u8SourceEndpoint      = ZB_ENDPOINT_ATTR;
    sWriteAttributeRequest.u8DestinationEndpoint = ZB_ENDPOINT_ATTR;

    sWriteAttributeRequest.u16ClusterID             = htons(u16ClusterID);
    sWriteAttributeRequest.u8Direction              = u8Direction;
    sWriteAttributeRequest.u8ManufacturerSpecific   = u8ManufacturerSpecific;
    sWriteAttributeRequest.u16ManufacturerID        = htons(u16ManufacturerID);
    sWriteAttributeRequest.u8NumAttributes          = 1;
    sWriteAttributeRequest.u16AttributeID           = htons(u16AttributeID);
    sWriteAttributeRequest.u8Type                   = (uint8_t)eType;

    switch(eType)
    {
        case(E_ZCL_GINT8):
        case(E_ZCL_UINT8):
        case(E_ZCL_INT8):
        case(E_ZCL_ENUM8):
        case(E_ZCL_BMAP8):
        case(E_ZCL_BOOL):
        case(E_ZCL_OSTRING):
        case(E_ZCL_CSTRING):
            memcpy(&sWriteAttributeRequest.uData.u8Data, pvData, sizeof(uint8_t));
                        u16Length += sizeof(uint8_t);
            break;

        case(E_ZCL_LOSTRING):
        case(E_ZCL_LCSTRING):
        case(E_ZCL_STRUCT):
        case(E_ZCL_INT16):
        case(E_ZCL_UINT16):
        case(E_ZCL_ENUM16):
        case(E_ZCL_CLUSTER_ID):
        case(E_ZCL_ATTRIBUTE_ID):
            memcpy(&sWriteAttributeRequest.uData.u16Data, pvData, sizeof(uint16_t));
            sWriteAttributeRequest.uData.u16Data = ntohs(sWriteAttributeRequest.uData.u16Data);
                        u16Length += sizeof(uint16_t);
            break;

        case(E_ZCL_UINT24):
        case(E_ZCL_UINT32):
        case(E_ZCL_TOD):
        case(E_ZCL_DATE):
        case(E_ZCL_UTCT):
        case(E_ZCL_BACNET_OID):
            memcpy(&sWriteAttributeRequest.uData.u32Data, pvData, sizeof(uint32_t));
            sWriteAttributeRequest.uData.u32Data = ntohl(sWriteAttributeRequest.uData.u32Data);
                        u16Length += sizeof(uint32_t);
            break;

        case(E_ZCL_UINT40):
        case(E_ZCL_UINT48):
        case(E_ZCL_UINT56):
        case(E_ZCL_UINT64):
        case(E_ZCL_IEEE_ADDR):
            memcpy(&sWriteAttributeRequest.uData.u64Data, pvData, sizeof(uint64_t));
            sWriteAttributeRequest.uData.u64Data = be64toh(sWriteAttributeRequest.uData.u64Data);
                        u16Length += sizeof(uint64_t);
            break;

        default:
            printf( "Unknown attribute data type (%d)", eType);
            return E_ZCB_ERROR;
    }

//    printf( "sWriteAttributeRequest:\n" );
//    dump( (char *)&sWriteAttributeRequest, u16Length );

    if (eSL_SendMessage(E_SL_MSG_WRITE_ATTRIBUTE_REQUEST, u16Length, &sWriteAttributeRequest, &u8SequenceNo) != E_SL_OK)
    {
// printf( "kok1\n" );
        goto done;
    }

    while (1)
    {
// printf( "kok2\n" );
        /* Wait 1 second for the message to arrive */
        /**\todo ZCB-Sheffield: handle data indication here for now - BAD Idea! Implement a general case handler in future! */
        if (eSL_MessageWait(E_SL_MSG_DATA_INDICATION, 1000, &u16Length, (void**)&psDataIndication) != E_SL_OK)
        {
            DEBUG_PRINTF( "No response to write attribute request\n");
            eStatus = E_ZCB_COMMS_FAILED;
            goto done;
        }

        DEBUG_PRINTF( "Got data indication\n");

        if (u8SequenceNo == psDataIndication->u8SequenceNo)
        {
            break;
        }
        else
        {
            printf( "Write Attribute sequence number received 0x%02X does not match that sent 0x%02X\n", psDataIndication->u8SequenceNo, u8SequenceNo);
            goto done;
        }
    }

    DEBUG_PRINTF( "Got write attribute response\n");

    eStatus = psDataIndication->u8Status;

done:
    free(psDataIndication);
// printf( "kok3\n" );
    return eStatus;
}

// ------------------------------------------------------------------
// Bind
// ------------------------------------------------------------------

teZcbStatus eZCB_SendBindCommand(uint64_t u64IEEEAddress,
                                 uint16_t u16ClusterId )
{

    struct _BindUnbindReq {
        uint64_t u64SrcAddress;
        uint8_t u8SrcEndpoint;
        uint16_t u16ClusterId;
        uint8_t u8DstAddrMode;
        union {
            struct {
                uint16_t u16DstAddress;
            } __attribute__((__packed__)) sShort;
            struct {
                uint64_t u64DstAddress;
                uint8_t u8DstEndPoint;
            } __attribute__((__packed__)) sExtended;
        } __attribute__((__packed__)) uAddressField;
    } __attribute__((__packed__)) sBindUnbindReq;

    struct _BindUnbindResp {
        uint8_t     u8SequenceNo;
        uint8_t     u8Status;
    } __attribute__((__packed__)) *psBindUnbindResp = NULL;

    uint16_t u16Length = sizeof(struct _BindUnbindReq);
    uint8_t u8SequenceNo;
    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;

    memset( (char *)&sBindUnbindReq, 0, u16Length );

    sBindUnbindReq.u64SrcAddress = htobe64( u64IEEEAddress );
    sBindUnbindReq.u8SrcEndpoint = ZB_ENDPOINT_ATTR;
    sBindUnbindReq.u16ClusterId  = htons( u16ClusterId );
    sBindUnbindReq.u8DstAddrMode = 0x03;
    sBindUnbindReq.uAddressField.sExtended.u64DstAddress = htobe64( zcbNodeGetExtendedAddress( 0 ) );
    sBindUnbindReq.uAddressField.sExtended.u8DstEndPoint = ZB_ENDPOINT_ATTR;

    DEBUG_PRINTF("0x%016llX Send (Un-)Binding request to 0x%016llX (%d)\n", sBindUnbindReq.uAddressField.sExtended.u64DstAddress, u64IEEEAddress, u16Length);
    dump( (char *)&sBindUnbindReq, u16Length );

    if (eSL_SendMessage( E_SL_MSG_BIND,
                         u16Length,
                         &sBindUnbindReq,
                         &u8SequenceNo) != E_SL_OK)
    {
 printf( "bind 1\n" );
        goto done;
    }

    while (1)
    {
 printf( "bind 2\n" );
        /* Wait 1 second for the message to arrive */
        if (eSL_MessageWait(E_SL_MSG_BIND_RESPONSE, 1000, &u16Length,
            (void**)&psBindUnbindResp) != E_SL_OK)
        {
            DEBUG_PRINTF( "No response to Binding request\n");
            eStatus = E_ZCB_COMMS_FAILED;
            goto done;
        }

        DEBUG_PRINTF( "Got Binding response\n");

        if (u8SequenceNo == psBindUnbindResp->u8SequenceNo)
        {
            break;
        }
        else
        {
            printf( "Binding request sequence number received 0x%02X does not match that sent 0x%02X\n",
                    psBindUnbindResp->u8SequenceNo, u8SequenceNo);
            goto done;
        }
    }

    eStatus = psBindUnbindResp->u8Status;

done:
    free(psBindUnbindResp);
 printf( "bind 3\n" );
    return eStatus;
}

// ------------------------------------------------------------------
// Configure Reporting
// ------------------------------------------------------------------

teZcbStatus eZCB_SendConfigureReportingCommand(uint16_t u16ShortAddress,
                                    uint16_t u16ClusterID,
                                    uint16_t u16AttributeID,
                                    teZCL_ZCLAttributeType eType )
{
    struct _AttributeReportingConfigurationRequest
    {
        uint8_t     u8TargetAddressMode;
        uint16_t    u16TargetAddress;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
        uint16_t    u16ClusterID;

        uint8_t     u8DirectionIsReceived;
        teZCL_ZCLAttributeType eAttributeDataType;
        uint16_t    u16AttributeEnum;
        uint16_t    u16MinimumReportingInterval;
        uint16_t    u16MaximumReportingInterval;
        uint16_t    u16TimeoutPeriodField;
        uint64_t    uAttributeReportableChange;
    } __attribute__((__packed__)) sAttributeReportingConfigurationRequest;

    struct _AttributeReportingConfigurationResponse
    {
        uint8_t     u8SequenceNo;
        uint8_t     u8Status;
    } __attribute__((__packed__)) *psAttributeReportingConfigurationResponse = NULL;

    uint16_t u16Length = sizeof(struct _AttributeReportingConfigurationRequest);
    uint8_t u8SequenceNo;
    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;

    DEBUG_PRINTF("Send Reporting Configuration request to 0x%04X\n", u16ShortAddress);

    sAttributeReportingConfigurationRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_SHORT_NO_ACK;
    sAttributeReportingConfigurationRequest.u16TargetAddress      = htons(u16ShortAddress);
    sAttributeReportingConfigurationRequest.u8SourceEndpoint      = ZB_ENDPOINT_ATTR;
    sAttributeReportingConfigurationRequest.u8DestinationEndpoint = ZB_ENDPOINT_ATTR;

    sAttributeReportingConfigurationRequest.u16ClusterID             = htons(u16ClusterID);

    sAttributeReportingConfigurationRequest.u8DirectionIsReceived       = 0;
    sAttributeReportingConfigurationRequest.eAttributeDataType          = (uint8_t)eType;
    sAttributeReportingConfigurationRequest.u16AttributeEnum            = htons(u16AttributeID);
    sAttributeReportingConfigurationRequest.u16MinimumReportingInterval = 0;
    sAttributeReportingConfigurationRequest.u16MaximumReportingInterval = 0;
    sAttributeReportingConfigurationRequest.u16TimeoutPeriodField       = 0;
    sAttributeReportingConfigurationRequest.uAttributeReportableChange  = 0;

    if (eSL_SendMessage( E_SL_MSG_CONFIG_REPORTING_REQUEST,
                         u16Length,
                         &sAttributeReportingConfigurationRequest,
                         &u8SequenceNo) != E_SL_OK)
    {
// printf( "kok1\n" );
        goto done;
    }

    while (1)
    {
// printf( "kok2\n" );
        /* Wait 1 second for the message to arrive */
        if (eSL_MessageWait(E_SL_MSG_CONFIG_REPORTING_RESPONSE, 1000, &u16Length,
            (void**)&psAttributeReportingConfigurationResponse) != E_SL_OK)
        {
            DEBUG_PRINTF( "No response to Reporting Configuration request\n");
            eStatus = E_ZCB_COMMS_FAILED;
            goto done;
        }

        DEBUG_PRINTF( "Got Reporting Configuration response\n");

        if (u8SequenceNo == psAttributeReportingConfigurationResponse->u8SequenceNo)
        {
            break;
        }
        else
        {
            printf( "Reporting Configuration request sequence number received 0x%02X does not match that sent 0x%02X\n",
                    psAttributeReportingConfigurationResponse->u8SequenceNo, u8SequenceNo);
            goto done;
        }
    }

    eStatus = psAttributeReportingConfigurationResponse->u8Status;

done:
    free(psAttributeReportingConfigurationResponse);
// printf( "kok3\n" );
    return eStatus;
}

// ------------------------------------------------------------------
// Groups
// ------------------------------------------------------------------

teZcbStatus eZCB_AddGroupMembership( uint16_t u16ShortAddress, uint16_t u16GroupAddress ) {

    struct _AddGroupMembershipRequest {
        uint8_t     u8TargetAddressMode;
        uint16_t    u16TargetAddress;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
        uint16_t    u16GroupAddress;
    } __attribute__((__packed__)) sAddGroupMembershipRequest;

    uint8_t u8SequenceNo;
    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;

    DEBUG_PRINTF( "Send add group membership 0x%04X request to 0x%04X\n", u16GroupAddress, u16ShortAddress);

    sAddGroupMembershipRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_SHORT_NO_ACK;
    sAddGroupMembershipRequest.u16TargetAddress      = htons(u16ShortAddress);
    sAddGroupMembershipRequest.u8SourceEndpoint      = ZB_ENDPOINT_GROUP;
    sAddGroupMembershipRequest.u8DestinationEndpoint = ZB_ENDPOINT_GROUP;

    sAddGroupMembershipRequest.u16GroupAddress = htons(u16GroupAddress);

    if (eSL_SendMessage(E_SL_MSG_ADD_GROUP_REQUEST, sizeof(struct _AddGroupMembershipRequest),
                        &sAddGroupMembershipRequest, &u8SequenceNo) != E_SL_OK) {
        eStatus = E_ZCB_OK;
    }

    return eStatus;
}


teZcbStatus eZCB_RemoveGroupMembership( uint16_t u16ShortAddress, uint16_t u16GroupAddress) {

    struct _RemoveGroupMembershipRequest {
        uint8_t     u8TargetAddressMode;
        uint16_t    u16TargetAddress;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
        uint16_t    u16GroupAddress;
    } __attribute__((__packed__)) sRemoveGroupMembershipRequest;

    uint8_t u8SequenceNo;
    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;

    DEBUG_PRINTF( "Send remove group membership 0x%04X request to 0x%04X\n", u16GroupAddress, u16ShortAddress);

    sRemoveGroupMembershipRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_SHORT_NO_ACK;
    sRemoveGroupMembershipRequest.u16TargetAddress      = htons(u16ShortAddress);
    sRemoveGroupMembershipRequest.u8SourceEndpoint      = ZB_ENDPOINT_GROUP;
    sRemoveGroupMembershipRequest.u8DestinationEndpoint = ZB_ENDPOINT_GROUP;

    sRemoveGroupMembershipRequest.u16GroupAddress = htons(u16GroupAddress);

    if (eSL_SendMessage(E_SL_MSG_REMOVE_GROUP_REQUEST, sizeof(struct _RemoveGroupMembershipRequest),
            &sRemoveGroupMembershipRequest, &u8SequenceNo) == E_SL_OK) {
        eStatus = E_ZCB_OK;
    }

    return eStatus;
}


teZcbStatus eZCB_ClearGroupMemberships( uint16_t u16ShortAddress ) {

    struct _ClearGroupMembershipRequest {
        uint8_t     u8TargetAddressMode;
        uint16_t    u16TargetAddress;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
    } __attribute__((__packed__)) sClearGroupMembershipRequest;

    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;

    DEBUG_PRINTF( "Send clear group membership request to 0x%04X\n", u16ShortAddress);

    sClearGroupMembershipRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_SHORT_NO_ACK;
    sClearGroupMembershipRequest.u16TargetAddress      = htons(u16ShortAddress);
    sClearGroupMembershipRequest.u8SourceEndpoint      = ZB_ENDPOINT_GROUP;
    sClearGroupMembershipRequest.u8DestinationEndpoint = ZB_ENDPOINT_GROUP;

    if (eSL_SendMessage(E_SL_MSG_REMOVE_ALL_GROUPS, sizeof(struct _ClearGroupMembershipRequest),
        &sClearGroupMembershipRequest, NULL) == E_SL_OK) {
        eStatus = E_ZCB_OK;
    }

    return eStatus;
}

// ------------------------------------------------------------------
// Scenes
// ------------------------------------------------------------------

teZcbStatus eZCB_RemoveScene( uint16_t u16ShortAddress, uint16_t u16GroupAddress, uint8_t u8SceneID) {

    struct _RemoveSceneRequest {
        uint8_t     u8TargetAddressMode;
        uint16_t    u16TargetAddress;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
        uint16_t    u16GroupAddress;
        uint8_t     u8SceneID;
    } __attribute__((__packed__)) sRemoveSceneRequest;

    uint8_t u8SequenceNo;
    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;

    DEBUG_PRINTF( "Send remove scene %d (Group 0x%04X) for Endpoint %d to 0x%04X\n",
                u8SceneID, u16GroupAddress, sRemoveSceneRequest.u8DestinationEndpoint, u16ShortAddress);

    if (u16ShortAddress != 0xFFFF) {
        sRemoveSceneRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_SHORT_NO_ACK;
        sRemoveSceneRequest.u16TargetAddress     = htons(u16ShortAddress);
    } else {
        sRemoveSceneRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_GROUP;
        sRemoveSceneRequest.u16TargetAddress      = htons(u16GroupAddress);
        sRemoveSceneRequest.u8DestinationEndpoint = ZB_DEFAULT_ENDPOINT_ZLL;
    }

    sRemoveSceneRequest.u8SourceEndpoint      = ZB_ENDPOINT_SCENE;
    sRemoveSceneRequest.u8DestinationEndpoint = ZB_ENDPOINT_SCENE;
    sRemoveSceneRequest.u16GroupAddress  = htons(u16GroupAddress);
    sRemoveSceneRequest.u8SceneID        = u8SceneID;

    if (eSL_SendMessage(E_SL_MSG_REMOVE_SCENE, sizeof(struct _RemoveSceneRequest),
        &sRemoveSceneRequest, &u8SequenceNo) == E_SL_OK) {
        eStatus = E_ZCB_OK;
    }

    return eStatus;
}

teZcbStatus eZCB_RemoveAllScenes( uint16_t u16ShortAddress, uint16_t u16GroupAddress) {

    struct _RemoveAllScenesRequest {
        uint8_t     u8TargetAddressMode;
        uint16_t    u16TargetAddress;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
        uint16_t    u16GroupAddress;
    } __attribute__((__packed__)) sRemoveAllScenesRequest;

    uint8_t u8SequenceNo;
    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;

    DEBUG_PRINTF( "Send remove all scenes (Group 0x%04X) to 0x%04X\n",
                u16GroupAddress, u16ShortAddress);

    if (u16ShortAddress != 0xFFFF) {
        sRemoveAllScenesRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_SHORT_NO_ACK;
        sRemoveAllScenesRequest.u16TargetAddress     = htons(u16ShortAddress);
    } else {
        sRemoveAllScenesRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_GROUP;
        sRemoveAllScenesRequest.u16TargetAddress      = htons(u16GroupAddress);
        sRemoveAllScenesRequest.u8DestinationEndpoint = ZB_DEFAULT_ENDPOINT_ZLL;
    }

    sRemoveAllScenesRequest.u8SourceEndpoint      = ZB_ENDPOINT_SCENE;
    sRemoveAllScenesRequest.u8DestinationEndpoint = ZB_ENDPOINT_SCENE;
    sRemoveAllScenesRequest.u16GroupAddress  = htons(u16GroupAddress);

    if (eSL_SendMessage(E_SL_MSG_REMOVE_ALL_SCENES, sizeof(struct _RemoveAllScenesRequest),
        &sRemoveAllScenesRequest, &u8SequenceNo) == E_SL_OK) {
        eStatus = E_ZCB_OK;
    }

    return eStatus;
}


teZcbStatus eZCB_StoreScene( uint16_t u16ShortAddress, uint16_t u16GroupAddress, uint8_t u8SceneID) {

    struct _StoreSceneRequest {
        uint8_t     u8TargetAddressMode;
        uint16_t    u16TargetAddress;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
        uint16_t    u16GroupAddress;
        uint8_t     u8SceneID;
    } __attribute__((__packed__)) sStoreSceneRequest;

    uint8_t u8SequenceNo;
    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;

    DEBUG_PRINTF( "Send store scene %d (Group 0x%04X)\n", u8SceneID, u16GroupAddress);

    if (u16ShortAddress != 0xFFFF) {
        sStoreSceneRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_SHORT_NO_ACK;
        sStoreSceneRequest.u16TargetAddress     = htons(u16ShortAddress);
    } else {
        sStoreSceneRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_GROUP;
        sStoreSceneRequest.u16TargetAddress      = htons(u16GroupAddress);
        sStoreSceneRequest.u8DestinationEndpoint = ZB_DEFAULT_ENDPOINT_ZLL;
    }

    sStoreSceneRequest.u8SourceEndpoint      = ZB_ENDPOINT_SCENE;
    sStoreSceneRequest.u8DestinationEndpoint = ZB_ENDPOINT_SCENE;
    sStoreSceneRequest.u16GroupAddress  = htons(u16GroupAddress);
    sStoreSceneRequest.u8SceneID        = u8SceneID;

    if (eSL_SendMessage(E_SL_MSG_STORE_SCENE, sizeof(struct _StoreSceneRequest),
        &sStoreSceneRequest, &u8SequenceNo) == E_SL_OK) {
        eStatus = E_ZCB_OK;
    }

    return eStatus;
}


teZcbStatus eZCB_RecallScene( uint16_t u16ShortAddress, uint16_t u16GroupAddress, uint8_t u8SceneID) {

    struct _RecallSceneRequest {
        uint8_t     u8TargetAddressMode;
        uint16_t    u16TargetAddress;
        uint8_t     u8SourceEndpoint;
        uint8_t     u8DestinationEndpoint;
        uint16_t    u16GroupAddress;
        uint8_t     u8SceneID;
    } __attribute__((__packed__)) sRecallSceneRequest;

    uint8_t         u8SequenceNo;
    teZcbStatus eStatus = E_ZCB_COMMS_FAILED;

    if (u16ShortAddress != 0xFFFF) {
        DEBUG_PRINTF( "Send recall scene %d (Group 0x%04X) to 0x%04X\n",
                u8SceneID, u16GroupAddress, u16ShortAddress);

        sRecallSceneRequest.u8TargetAddressMode   = E_ZB_ADDRESS_MODE_SHORT_NO_ACK;
        sRecallSceneRequest.u16TargetAddress     = htons(u16ShortAddress);
    } else {
        sRecallSceneRequest.u8TargetAddressMode  = E_ZB_ADDRESS_MODE_GROUP;
        sRecallSceneRequest.u16TargetAddress     = htons(u16GroupAddress);
        sRecallSceneRequest.u8DestinationEndpoint= ZB_DEFAULT_ENDPOINT_ZLL;

        DEBUG_PRINTF( "Send recall scene %d (Group 0x%04X) to 0x%04X\n",
                u8SceneID, u16GroupAddress, u16GroupAddress);
    }

    sRecallSceneRequest.u8SourceEndpoint      = ZB_ENDPOINT_SCENE;
    sRecallSceneRequest.u8DestinationEndpoint = ZB_ENDPOINT_SCENE;
    sRecallSceneRequest.u16GroupAddress  = htons(u16GroupAddress);
    sRecallSceneRequest.u8SceneID        = u8SceneID;

    if (eSL_SendMessage(E_SL_MSG_RECALL_SCENE, sizeof(struct _RecallSceneRequest),
        &sRecallSceneRequest, &u8SequenceNo) == E_SL_OK) {
        eStatus = E_ZCB_OK;
    }

    return eStatus;
}

// ------------------------------------------------------------------
// Handlers
// ------------------------------------------------------------------

static void ZCB_HandleVersionResponse(void *pvUser, uint16_t u16Length, void *pvMessage) {
    static int counter = 0;
    printf( "\n************ ZCB_HandleVersionResponse\n" );
    // dump( (char *)pvMessage, u16Length );

    struct _VersionResponse {
        uint32_t    u32Version;
    } __attribute__((__packed__)) *psVersionResponse = (struct _VersionResponse *)pvMessage;

    uint32_t version = ntohl(psVersionResponse->u32Version);
    newDbSystemSaveIntval( "zcb_version", version );

    int major = ( version >> 16 ) & 0xFFFF;
    int minor = version & 0xFFFF;
    printf("Connected to control bridge version r%d.%d\n", major, minor );

    char buf[20];
    sprintf( buf, "r%d.%d", major, minor );
    // Save version to database
    newDbSystemSaveVal( "version", counter++, buf );
}

static void ZCB_HandleNodeClusterList(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleNodeClusterList\n" );

    struct _tsClusterList {
        uint8_t     u8Endpoint;
        uint16_t    u16ProfileID;
        uint16_t    au16ClusterList[255];
    } __attribute__((__packed__)) *psClusterList = (struct _tsClusterList *)pvMessage;

    psClusterList->u16ProfileID = ntohs(psClusterList->u16ProfileID);

    DEBUG_PRINTF( "Cluster list for endpoint %d, profile ID 0x%04X\n",
                psClusterList->u8Endpoint,
                psClusterList->u16ProfileID);

    int nClusters = ( u16Length - 3 ) / 2;
    int i;
    for ( i=0; i<nClusters; i++ ) {
        DEBUG_PRINTF( "- ID 0x%04X\n", ntohs( psClusterList->au16ClusterList[i] ) );
    }
}


static void ZCB_HandleNodeClusterAttributeList(void *pvUser, uint16_t u16Length, void *pvMessage) {
#if 0
    DEBUG_PRINTF( "\n************ ZCB_HandleNodeClusterAttributeList\n" );

    struct _tsClusterAttributeList {
        uint8_t     u8Endpoint;
        uint16_t    u16ProfileID;
        uint16_t    u16ClusterID;
        uint16_t    au16AttributeList[255];
    } __attribute__((__packed__)) *psClusterAttributeList = (struct _tsClusterAttributeList *)pvMessage;

    psClusterAttributeList->u16ProfileID = ntohs(psClusterAttributeList->u16ProfileID);
    psClusterAttributeList->u16ClusterID = ntohs(psClusterAttributeList->u16ClusterID);

    DEBUG_PRINTF( "Cluster attribute list for endpoint %d, cluster 0x%04X, profile ID 0x%04X\n",
                psClusterAttributeList->u8Endpoint,
                psClusterAttributeList->u16ClusterID,
                psClusterAttributeList->u16ProfileID);
#endif
}


static void ZCB_HandleNodeCommandIDList(void *pvUser, uint16_t u16Length, void *pvMessage) {
#if 0
    DEBUG_PRINTF( "\n************ ZCB_HandleNodeCommandIDList\n" );

    struct _tsCommandIDList {
        uint8_t     u8Endpoint;
        uint16_t    u16ProfileID;
        uint16_t    u16ClusterID;
        uint8_t     au8CommandList[255];
    } __attribute__((__packed__)) *psCommandIDList = (struct _tsCommandIDList *)pvMessage;

    psCommandIDList->u16ProfileID = ntohs(psCommandIDList->u16ProfileID);
    psCommandIDList->u16ClusterID = ntohs(psCommandIDList->u16ClusterID);

    DEBUG_PRINTF( "Command ID list for endpoint %d, cluster 0x%04X, profile ID 0x%04X\n",
                psCommandIDList->u8Endpoint,
                psCommandIDList->u16ClusterID,
                psCommandIDList->u16ProfileID);
#endif
}


static void ZCB_HandleNetworkJoined(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleNetworkJoined\n" );

    struct _tsNetworkJoinedFormedShort {
        uint8_t     u8Status;
        uint16_t    u16ShortAddress;
        uint64_t    u64IEEEAddress;
        uint8_t     u8Channel;
    } __attribute__((__packed__)) *psMessageShort = (struct _tsNetworkJoinedFormedShort *)pvMessage;

    struct _tsNetworkJoinedFormedExtended {
        uint8_t     u8Status;
        uint16_t    u16ShortAddress;
        uint64_t    u64IEEEAddress;
        uint8_t     u8Channel;
        uint64_t    u64PanID;
        uint16_t    u16PanID;
    } __attribute__((__packed__)) *psMessageExt = (struct _tsNetworkJoinedFormedExtended *)pvMessage;

    psMessageShort->u16ShortAddress = ntohs(psMessageShort->u16ShortAddress);
    psMessageShort->u64IEEEAddress  = be64toh(psMessageShort->u64IEEEAddress);

    zcbAddNode( psMessageShort->u16ShortAddress, psMessageShort->u64IEEEAddress );

    char  ePanId[16+2];
    if ( psMessageShort->u16ShortAddress == 0x0000 ) {
        // Coordinator
        u642nibblestr( psMessageShort->u64IEEEAddress, ePanId );
        newDbSystemSaveStringval( "zcb_extpanid", ePanId );
        zcbNodeSetDeviceID( psMessageShort->u16ShortAddress, SIMPLE_DESCR_GATEWAY );
        eZCB_AddGroupMembership( 0x0000, 0x0AA1 );
    }

    if (u16Length == sizeof(struct _tsNetworkJoinedFormedExtended)) {
        psMessageExt->u64PanID      = be64toh(psMessageExt->u64PanID);
        psMessageExt->u16PanID      = ntohs(psMessageExt->u16PanID);

        DEBUG_PRINTF( "EXTENDED Network %s on channel %d\n- Control bridge address 0x%04X (0x%016llX)\n- PAN ID 0x%04X (0x%016llX)\n",
                psMessageExt->u8Status == 0 ? "joined" : "formed",
                psMessageExt->u8Channel,
                psMessageExt->u16ShortAddress,
                (unsigned long long int)psMessageExt->u64IEEEAddress,
                psMessageExt->u16PanID,
                (unsigned long long int)psMessageExt->u64PanID);

        /* Update global network information */
        u642nibblestr( psMessageExt->u64PanID, ePanId );
        newDbSystemSaveStringval( "zcb_extpanid", ePanId );
        newDbSystemSaveIntval( "zcb_panid", psMessageExt->u16PanID );
        newDbSystemSaveIntval( "zcb_channel", psMessageExt->u8Channel );
    } else {
        DEBUG_PRINTF( "NON-EXTENDED Network %s on channel %d\n- Control bridge address 0x%04X (0x%016llX)\n",
                    psMessageShort->u8Status == 0 ? "joined" : "formed",
                    psMessageShort->u8Channel,
                    psMessageShort->u16ShortAddress,
                    (unsigned long long int)psMessageShort->u64IEEEAddress);
    }
}



static void ZCB_HandleDeviceAnnounce(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleDeviceAnnounce\n" );

    struct _tsDeviceAnnounce {
        uint16_t    u16ShortAddress;
        uint64_t    u64IEEEAddress;
        uint8_t     u8MacCapability;
    } __attribute__((__packed__)) *psMessage = (struct _tsDeviceAnnounce *)pvMessage;
    newdb_zcb_t sZcb;

    psMessage->u16ShortAddress  = ntohs(psMessage->u16ShortAddress);
    psMessage->u64IEEEAddress   = be64toh(psMessage->u64IEEEAddress);

    DEBUG_PRINTF( "Device Joined, Address 0x%04X (0x%016llX). Mac Capability Mask 0x%02X\n",
                psMessage->u16ShortAddress,
                (unsigned long long int)psMessage->u64IEEEAddress,
                psMessage->u8MacCapability
            );


    zcbAddNode(psMessage->u16ShortAddress, psMessage->u64IEEEAddress);
    newDbGetZcbSaddr(psMessage->u16ShortAddress, &sZcb);

    if (sZcb.type == SIMPLE_DESCR_UNKNOWN)
    {
      /* TODO : query for End Point first */
      if ( SimpleDescriptorRequest( psMessage->u16ShortAddress ) != E_ZCB_OK)
      {
        DEBUG_PRINTF( "Error sending simple descriptor request\n");
      }
    }

    ZcbMsg.AnnounceStart = true;

    return;
}

static void ZCB_HandleDeviceLeave(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleDeviceLeave\n" );

    struct _tsDeviceLeave {
        uint64_t    u64IEEEAddress;
        uint8_t     bRejoin;
    } __attribute__((__packed__)) *psMessage = (struct _tsDeviceLeave *)pvMessage;

    psMessage->u64IEEEAddress   = be64toh(psMessage->u64IEEEAddress);

    DEBUG_PRINTF( "Device Left, Address 0x%016llX, rejoining: %d\n",
                (unsigned long long int)psMessage->u64IEEEAddress,
                psMessage->bRejoin
            );

    zcbNodeLeft( psMessage->u64IEEEAddress, psMessage->bRejoin );

    return;
}


static void ZCB_HandleMatchDescriptorResponse(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleMatchDescriptorResponse\n" );

    struct _tMatchDescriptorResponse {
        uint8_t     u8SequenceNo;
        uint8_t     u8Status;
        uint16_t    u16ShortAddress;
        uint8_t     u8NumEndpoints;
        uint8_t     au8Endpoints[255];
    } __attribute__((__packed__)) *psMatchDescriptorResponse = (struct _tMatchDescriptorResponse *)pvMessage;
    psMatchDescriptorResponse->u16ShortAddress  = ntohs(psMatchDescriptorResponse->u16ShortAddress);

    DEBUG_PRINTF( "Match descriptor request response from node 0x%04X - %d matching endpoints.\n",
                psMatchDescriptorResponse->u16ShortAddress,
                psMatchDescriptorResponse->u8NumEndpoints
            );
}


static void ZCB_HandleAttributeReport(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleAttributeReport\n" );

    struct _tsAttributeReport {
        uint8_t     u8SequenceNo;
        uint16_t    u16ShortAddress;
        uint8_t     u8Endpoint;
        uint16_t    u16ClusterID;
        uint16_t    u16AttributeID;
        uint8_t     u8AttributeStatus;
        uint8_t     u8Type;
        uint16_t    u16SizeOfAttributesInBytes;
        union {
            uint8_t     u8Data;
            uint16_t    u16Data;
            uint32_t    u32Data;
            uint64_t    u64Data;
        } uData;
    } __attribute__((__packed__)) *psMessage = (struct _tsAttributeReport *)pvMessage;

    psMessage->u16ShortAddress  = ntohs(psMessage->u16ShortAddress);
    psMessage->u16ClusterID     = ntohs(psMessage->u16ClusterID);
    psMessage->u16AttributeID   = ntohs(psMessage->u16AttributeID);

    DEBUG_PRINTF( "Attribute report from 0x%04X - Endpoint %d, cluster 0x%04X, attribute 0x%04X.\n",
                psMessage->u16ShortAddress,
                psMessage->u8Endpoint,
                psMessage->u16ClusterID,
                psMessage->u16AttributeID
            );

    uint64_t u64Data = 0;
    volatile int32_t  i32Data;

    switch(psMessage->u8Type) {
        case(E_ZCL_GINT8):
        case(E_ZCL_UINT8):
        case(E_ZCL_INT8):
        case(E_ZCL_ENUM8):
        case(E_ZCL_BMAP8):
        case(E_ZCL_BOOL):
        case(E_ZCL_OSTRING):
        case(E_ZCL_CSTRING):
            u64Data = (uint64_t )psMessage->uData.u8Data;
            break;

        case(E_ZCL_LOSTRING):
        case(E_ZCL_LCSTRING):
        case(E_ZCL_STRUCT):
        case(E_ZCL_INT16):
        case(E_ZCL_UINT16):
        case(E_ZCL_ENUM16):
        case(E_ZCL_CLUSTER_ID):
        case(E_ZCL_ATTRIBUTE_ID):
            u64Data = (uint64_t )ntohs(psMessage->uData.u16Data);
            break;

        case(E_ZCL_UINT24):
        case(E_ZCL_UINT32):
        case(E_ZCL_TOD):
        case(E_ZCL_DATE):
        case(E_ZCL_UTCT):
        case(E_ZCL_BACNET_OID):
            u64Data = (uint64_t )ntohl(psMessage->uData.u32Data);
            break;

        case E_ZCL_INT24:
            i32Data = (int32_t)ntohl(psMessage->uData.u32Data);
            // REPAIR/EXTEND SIGN
            i32Data <<= 8;
            i32Data >>= 8;
            u64Data = (uint64_t )i32Data;
            break;

        case(E_ZCL_UINT40):
        case(E_ZCL_UINT48):
        case(E_ZCL_UINT56):
        case(E_ZCL_UINT64):
        case(E_ZCL_IEEE_ADDR):
            u64Data = (uint64_t )be64toh(psMessage->uData.u64Data);
            break;

        default:
            printf( "Unknown attribute data type (%d)\n", psMessage->u8Type );
            printf( "-  8: Data = %llx\n", (uint64_t )psMessage->uData.u8Data );
            printf( "- 16: Data = %llx\n", (uint64_t )ntohs( psMessage->uData.u16Data ) );
            printf( "- 32: Data = %llx\n", (uint64_t )ntohl( psMessage->uData.u32Data ) );
            printf( "- 64: Data = %llx\n", (uint64_t )be64toh( psMessage->uData.u64Data ) );
            break;
    }
    DEBUG_PRINTF( "Data = 0x%016llx\n", u64Data);

    handleAttribute( psMessage->u16ShortAddress,
                    psMessage->u16ClusterID,
                    psMessage->u16AttributeID,
                    u64Data,
                    psMessage->u8Endpoint );

    return;
}



static void ZCB_HandleSimpleDescriptorResponse(
    void *pvUser,
    uint16_t u16Length,
    void *pvMessage)
{

  tsZDSimpleDescRsp *psMessage = (tsZDSimpleDescRsp *) pvMessage;
  newdb_zcb_t sZcb;
  uint16_t  u16ShortAddress  = ntohs(psMessage->u16ShortAddress);
  uint16_t  u16DeviceId      = ntohs(psMessage->u16DeviceID);
  uint8_t   u8DeviceVersion  = psMessage->sBitField.u8DeviceVersion;
  uint16_t  u16ProfileId     = ntohs(psMessage->u16ProfileID);
  uint64_t  extendedAddress= zcbNodeGetExtendedAddress(u16ShortAddress);
  uint8_t   u8Cluster;
  uint8_t * pOutputClusterListAddr;

  DEBUG_PRINTF( "\n************ ZCB_HandleSimpleDescriptorResponse"
      "(addr=0x%04x, ID=0x%04x)\n",
      u16ShortAddress, u16DeviceId);

  //zcbNodeSetDeviceID(u16ShortAddress, u16DeviceID);
  if (newDbGetZcbSaddr(u16ShortAddress, &sZcb))
  {
    sZcb.type = u16DeviceId;
    sZcb.u16ProfileId = u16ProfileId;
    sZcb.u8DeviceVersion = u8DeviceVersion;
    newDbSetZcb(&sZcb);


    /* Get node's state : on/off
     * TODO : extend to all clusters and attributes
     */

    //DEBUG_PRINTF( "Dump SimpleDescResp\n");
    //dump((char*) pvMessage, u16Length);

    /* Take into account field "ClusterCount */
    pOutputClusterListAddr = (uint8_t*) &(psMessage->sInputClusters)+1;
    /* Scan input cluster list */
    for (u8Cluster = 0;
        u8Cluster < psMessage->sInputClusters.u8ClusterCount;
        u8Cluster++)
    {
      DEBUG_PRINTF( "Input clusters %i/%i -> 0x%04x\n",
          u8Cluster+1, psMessage->sInputClusters.u8ClusterCount,
          ntohs(psMessage->sInputClusters.au16Clusters[u8Cluster]));
      switch (ntohs(psMessage->sInputClusters.au16Clusters[u8Cluster]))
      {
        case E_ZB_CLUSTERID_ONOFF :
        {
          uint16_t u16OnOffAttrId = E_ZB_ATTRIBUTEID_ONOFF_ONOFF;
          DEBUG_PRINTF( "ON/OFF cluster detected\n");
          sZcb.uSupportedClusters.sClusterBitmap.hasOnOff = 1;
          ZDReadAttrReq(
              u16ShortAddress,
              ZB_ENDPOINT_ZHA,
              psMessage->u8Endpoint,
              E_ZB_CLUSTERID_ONOFF,
              1,
              &u16OnOffAttrId);

          break;
        } // case E_ZB_CLUSTERID_ONOFF
        case E_ZB_CLUSTERID_MEASUREMENTSENSING_TEMP :
        {
          DEBUG_PRINTF( "Meas. Sensing (temperature) cluster detected\n");
          sZcb.uSupportedClusters.sClusterBitmap.hasTemperatureSensing = 1;
          break;
        } // case E_ZB_CLUSTERID_MEASUREMENTSENSING_TEMP

        case E_ZB_CLUSTERID_OCCUPANCYSENSING :
        {
          DEBUG_PRINTF( "Meas. Sensing (occupancy) cluster detected\n");
          sZcb.uSupportedClusters.sClusterBitmap.hasOccupancySensing = 1;
          break;
        } // case E_ZB_CLUSTERID_OCCUPANCYSENSING

        case E_ZB_CLUSTERID_MEASUREMENTSENSING_ILLUM :
        {
          DEBUG_PRINTF( "Meas. Sensing (illuminance) cluster detected\n");
          sZcb.uSupportedClusters.sClusterBitmap.hasIlluminanceSensing = 1;
          break;
        } // case E_ZB_CLUSTERID_MEASUREMENTSENSING_ILLUM
        default:
          break;
      } // switch (psMessage->sInputClusters.au16Clusters[u8Cluster])
    } // for (u8Cluster=..
    pOutputClusterListAddr += psMessage->sInputClusters.u8ClusterCount
        * sizeof(uint16_t);
#if 0
    uint8_t * pOutputClusterListAddr = (uint8_t*) &(psMessage->sInputClusters);
    tsZDClusterList * ptsOutputClusters = (tsZDClusterList *) pOutputClusterListAddr;
    DEBUG_PRINTF( "ptsOutputClusters->u8ClusterCount:0x%02x",
        ptsOutputClusters->u8ClusterCount);
#endif

    /*
     * Update database with what we have for now,
     * additional info might come later
     */
    newDbSetZcb(&sZcb);
    announceDevice(extendedAddress, u16DeviceId, u16ProfileId, u8DeviceVersion);

    if ( ZcbMsg.AnnounceStart == true) {
        ZcbMsg.AnnounceStart = false;
        eZCB_SendMsg( BRIDGE_ADD_DEV, &sZcb, NULL);
#ifdef PERSIST
        newDbSave();
#endif
    }

  } // if (newDbGetZcbSaddr(u16ShortAddress, &sZcb))
  else
  {
    DEBUG_PRINTF("Unknown device %016jX, 0x%04X", extendedAddress, u16ShortAddress);
  }
} // ZCB_HandleSimpleDescriptorResponse

/*!
 * Handle reception of response to attribute read request.
 *
 * @param pvUser    Cookie from callback registration
 * @param u16Length Length of received message
 * @param pvMessage pointer to message content
 */
static void ZCB_HandleReadAttrResp(
    void *pvUser,
    uint16_t u16Length,
    void *pvMessage)
{
  DEBUG_PRINTF( "\n************ ZCB_HandleReadAttrResp\n" );
  tsZDReadAttrResp *psMessage = (tsZDReadAttrResp *) pvMessage;

  /* Retrieve device from database */
  newdb_zcb_t sZcbDev;
  uint16_t    u16ShortAddr = ntohs(psMessage->u16ShortAddress);
  uint64_t    extendedAddress= zcbNodeGetExtendedAddress(u16ShortAddr);

  if (!newDbGetZcbSaddr(u16ShortAddr, &sZcbDev))
  {
    DEBUG_PRINTF( "Error : device with short address %04x not found in DB\n", u16ShortAddr);
  }
  else
  {
    /* make sure it is cluster OnOff & OnOff attribute */
    /* For sake of simplicity, we ignore end-point Id */
    if (ntohs(psMessage->u16ClusterId) == E_ZB_CLUSTERID_ONOFF)
    {
      /* Make sure Attribute reading is ok */
      if (psMessage->u8AttributeStatus == SUCCESS &&
          ntohs(psMessage->u16AttributeId) == E_ZB_ATTRIBUTEID_ONOFF_ONOFF &&
          psMessage->u8AttributeDataType == E_ZCL_BOOL)
      {
        strncpy(sZcbDev.info,
            (psMessage->au8AttributeValue[0] ? "on" : "off"),
            LEN_CMD);
        /* Store in ZCB DB */
        newDbSetZcb(&sZcbDev);

        DEBUG_PRINTF("onoff device %04x status: %s \n", u16ShortAddr, sZcbDev.info);
      }
      else
      {
        DEBUG_PRINTF( "Error 0x%02x : Read Attr On/off for device 0x%04x failed\n",
            psMessage->u8AttributeStatus,
            u16ShortAddr);
        DEBUG_PRINTF( "Cluster: 0x%04x , Attr : 0x%04x, type : 0x%02x\n",
            ntohs(psMessage->u16ClusterId),
            ntohs(psMessage->u16AttributeId),
            psMessage->u8AttributeDataType
            );
      }
    }
    /* Store on/off state in DB, it will then be push in Device DB during Announce flow */
    if (extendedAddress)
    {
      announceDevice(extendedAddress,
          (uint16_t) sZcbDev.type,
          sZcbDev.u16ProfileId,
          sZcbDev.u8DeviceVersion);
    }

    if ( sZcbDev.type == SIMPLE_DESCR_THERMOSTAT ) {
      // Let coordinator become part of the AAL group (to receive group-cast thermostat messages)
      eZCB_AddGroupMembership( 0x0000, 0x0AA1 );
    }
  }

  return;
}


static void ZCB_HandleAddGroupResponse(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleAddGroupResponse\n" );

    struct _sAddGroupMembershipResponse {
        uint8_t     u8SequenceNo;
        uint8_t     u8Endpoint;
        uint16_t    u16ClusterID;
        uint8_t     u8Status;
        uint16_t    u16GroupAddress;
    } __attribute__((__packed__)) *psMessage = (struct _sAddGroupMembershipResponse *)pvMessage;

    psMessage->u16GroupAddress  = ntohs(psMessage->u16GroupAddress);

    DEBUG_PRINTF( "Add group membership 0x%04X status: %d\n",
        psMessage->u16GroupAddress, psMessage->u8Status);

    return;
}


static void ZCB_HandleDefaultResponse(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleDefaultResponse\n" );

    struct _sDefaultResponse {
        uint8_t             u8SequenceNo;           /**< Sequence number of outgoing message */
        uint8_t             u8Endpoint;             /**< Source endpoint */
        uint16_t            u16ClusterID;           /**< Source cluster ID */
        uint8_t             u8CommandID;            /**< Source command ID */
        uint8_t             u8Status;               /**< Command status */
    } __attribute__((__packed__)) *psMessage = (struct _sDefaultResponse *)pvMessage;

    psMessage->u16ClusterID  = ntohs(psMessage->u16ClusterID);

    DEBUG_PRINTF( "Default Response : cluster 0x%04X Command 0x%02x status: %02x\n",
        psMessage->u16ClusterID, psMessage->u8CommandID, psMessage->u8Status);

    return;
}

static void ZCB_HandleRemoveGroupMembershipResponse(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleRemoveGroupMembershipResponse\n" );

    struct _sRemoveGroupMembershipResponse {
        uint8_t     u8SequenceNo;
        uint8_t     u8Endpoint;
        uint16_t    u16ClusterID;
        uint8_t     u8Status;
        uint16_t    u16GroupAddress;
    } __attribute__((__packed__)) *psMessage = (struct _sRemoveGroupMembershipResponse *)pvMessage;

    psMessage->u16GroupAddress = ntohs(psMessage->u16GroupAddress);

    DEBUG_PRINTF( "Remove group membership 0x%04X status: %d\n",
        psMessage->u16GroupAddress, psMessage->u8Status);
}

static void ZCB_HandleRemoveSceneResponse(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleRemoveSceneResponse\n" );

    struct _sRemoveSceneResponse {
        uint8_t     u8SequenceNo;
        uint8_t     u8Endpoint;
        uint16_t    u16ClusterID;
        uint8_t     u8Status;
        uint16_t    u16GroupAddress;
        uint8_t     u8SceneID;
    } __attribute__((__packed__)) *psMessage = (struct _sRemoveSceneResponse *)pvMessage;

    psMessage->u16GroupAddress = ntohs(psMessage->u16GroupAddress);

    DEBUG_PRINTF( "Remove scene %d (Group0x%04X) status: %d\n",
                psMessage->u8SceneID, psMessage->u16GroupAddress, psMessage->u8Status);
}

static void ZCB_HandleStoreSceneResponse(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleStoreSceneResponse\n" );

    struct _sStoreSceneResponse {
        uint8_t     u8SequenceNo;
        uint8_t     u8Endpoint;
        uint16_t    u16ClusterID;
        uint8_t     u8Status;
        uint16_t    u16GroupAddress;
        uint8_t     u8SceneID;
    } __attribute__((__packed__)) *psMessage = (struct _sStoreSceneResponse *)pvMessage;

    psMessage->u16GroupAddress = ntohs(psMessage->u16GroupAddress);

    DEBUG_PRINTF( "Store scene %d (Group0x%04X) status: %d\n",
                psMessage->u8SceneID, psMessage->u16GroupAddress, psMessage->u8Status);
}

static void
ZCB_HandleLog(
    void *pvUser,
    uint16_t u16Length,
    void *pvMessage)
{
  struct sLog
  {
      uint8_t  u8Level;
      uint8_t  au8Message[255];
  }  __attribute__((__packed__)) * psMessage = (struct sLog *) pvMessage;
  // dump(psMessage, u16Length);
  psMessage->au8Message[u16Length] = '\0';
  DEBUG_PRINTF( "Log : %s (%d)\n",
      psMessage->au8Message,
      psMessage->u8Level);
}

static void ZCB_HandleZcbStatusResponse(void *pvUser, uint16_t u16Length, void *pvMessage) {
    DEBUG_PRINTF( "\n************ ZCB_HandleZcbStatusResponse\n" );

    struct _sZcbStatusResponse {
        uint8_t     u8Status;
    } __attribute__((__packed__)) *psZcbStatusResponse = (struct _sZcbStatusResponse *)pvMessage;

    newDbSetNwkState(psZcbStatusResponse->u8Status);

    printf("GetZcbStatusResponse %s \n", ( psZcbStatusResponse->u8Status  ) ? "Network On" : "Network Off");
}

// ------------------------------------------------------------------
// END OF FILE
// ------------------------------------------------------------------