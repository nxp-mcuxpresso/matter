// ------------------------------------------------------------------
// ZCB - include file
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

#ifndef  _ZCB_INC
#define  _ZCB_INC

#include <stdint.h>
#include <netinet/in.h>
#include <sys/time.h>

#include "ZigbeeConstant.h"
#include "Utils.h"

#include "newDb.h"

#if defined __cplusplus
extern "C" {
#endif

// ------------------------------------------------------------------
// Endpoints
// ------------------------------------------------------------------

#define ZB_ENDPOINT_ZHA             1   // ZigBee-HA
#define ZB_ENDPOINT_ONOFF           1   // ON/OFF cluster
#define ZB_ENDPOINT_GROUP           1   // GROUP cluster
#define ZB_ENDPOINT_SCENE           1   // SCENE cluster
#define ZB_ENDPOINT_SIMPLE          1   // SimpleDescriptor cluster
#define ZB_ENDPOINT_LAMP            1   // ON/OFF, Color control
#define ZB_ENDPOINT_TUNNEL          1   // For tunnel messages
#define ZB_ENDPOINT_ATTR            1   // For attrs

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/** Enumerated type of statuses - This fits in with the Zigbee ZCL status codes */
typedef enum
{
    /* Zigbee ZCL status codes */
    E_ZCB_OK                            = 0x00,
    E_ZCB_ERROR                         = 0x01,

    /* ZCB internal status codes */
    E_ZCB_ERROR_NO_MEM                  = 0x10,
    E_ZCB_COMMS_FAILED                  = 0x11,
    E_ZCB_UNKNOWN_NODE                  = 0x12,
    E_ZCB_UNKNOWN_ENDPOINT              = 0x13,
    E_ZCB_UNKNOWN_CLUSTER               = 0x14,
    E_ZCB_REQUEST_NOT_ACTIONED          = 0x15,

    /* Zigbee ZCL status codes */
    E_ZCB_NOT_AUTHORISED                = 0x7E,
    E_ZCB_RESERVED_FIELD_NZERO          = 0x7F,
    E_ZCB_MALFORMED_COMMAND             = 0x80,
    E_ZCB_UNSUP_CLUSTER_COMMAND         = 0x81,
    E_ZCB_UNSUP_GENERAL_COMMAND         = 0x82,
    E_ZCB_UNSUP_MANUF_CLUSTER_COMMAND   = 0x83,
    E_ZCB_UNSUP_MANUF_GENERAL_COMMAND   = 0x84,
    E_ZCB_INVALID_FIELD                 = 0x85,
    E_ZCB_UNSUP_ATTRIBUTE               = 0x86,
    E_ZCB_INVALID_VALUE                 = 0x87,
    E_ZCB_READ_ONLY                     = 0x88,
    E_ZCB_INSUFFICIENT_SPACE            = 0x89,
    E_ZCB_DUPLICATE_EXISTS              = 0x8A,
    E_ZCB_NOT_FOUND                     = 0x8B,
    E_ZCB_UNREPORTABLE_ATTRIBUTE        = 0x8C,
    E_ZCB_INVALID_DATA_TYPE             = 0x8D,
    E_ZCB_INVALID_SELECTOR              = 0x8E,
    E_ZCB_WRITE_ONLY                    = 0x8F,
    E_ZCB_INCONSISTENT_STARTUP_STATE    = 0x90,
    E_ZCB_DEFINED_OUT_OF_BAND           = 0x91,
    E_ZCB_INCONSISTENT                  = 0x92,
    E_ZCB_ACTION_DENIED                 = 0x93,
    E_ZCB_TIMEOUT                       = 0x94,

    E_ZCB_HARDWARE_FAILURE              = 0xC0,
    E_ZCB_SOFTWARE_FAILURE              = 0xC1,
    E_ZCB_CALIBRATION_ERROR             = 0xC2,
} teZcbStatus;



/** Union type for all Zigbee attribute data types */
typedef union
{
    uint8_t     u8Data;
    uint16_t    u16Data;
    uint32_t    u32Data;
    uint64_t    u64Data;
} tuZcbAttributeData;


/** Enumerated type of module modes */
typedef enum
{
    E_MODE_COORDINATOR      = 0,        /**< Start module as a coordinator */
    E_MODE_ROUTER           = 1,        /**< Start module as a router */
    E_MODE_HA_COMPATABILITY = 2,        /**< Start module as router in HA compatability mode */
} teModuleMode;


/** Enumerated type of allowable channels */
typedef enum
{
    E_CHANNEL_AUTOMATIC     = 0,
    E_CHANNEL_MINIMUM       = 11,
    E_CHANNEL_MAXIMUM       = 26
} teChannel;


/** Enumerated type of supported authorisation schemes */
typedef enum
{
    E_AUTH_SCHEME_NONE,
    E_AUTH_SCHEME_RADIUS_PAP,
} teAuthScheme;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

uint16_t zcbNodeGetShortAddress( char * mac );
uint64_t zcbNodeGetExtendedAddress( uint16_t shortAddress );
// int32_t eGetzcbNodeNum(void);

/** Initialise control bridge connected to serial port */
teZcbStatus eZCB_Init(char *cpSerialDevice, uint32_t u32BaudRate);


/** Finished with control bridge - call this to tidy up */
teZcbStatus eZCB_Finish(void);

/** Attempt to establish comms with the control bridge.
 *  \return E_ZCB_OK when comms established, otherwise E_ZCB_COMMS_FAILED
 */
teZcbStatus eZCB_EstablishComms(void);

teZcbStatus eZCB_SendMsg(int MsgType, newdb_zcb_t *Zcb, void* data);

teZcbStatus eOnOff( uint16_t u16ShortAddress, uint8_t u8Mode );

int eZCB_NeighbourTableRequest( int start );

teZcbStatus eZCB_WriteAttributeRequest(uint16_t u16ShortAddress,
				       uint16_t u16ClusterID,
                                       uint8_t u8Direction,
				       uint8_t u8ManufacturerSpecific,
				       uint16_t u16ManufacturerID,
                                       uint16_t u16AttributeID,
				       teZCL_ZCLAttributeType eType,
				       void *pvData);

teZcbStatus eZCB_SendBindCommand(uint64_t u64IEEEAddress,
                                 uint16_t u16ClusterID );

teZcbStatus eZCB_SendConfigureReportingCommand(uint16_t u16ShortAddress,
                                       uint16_t u16ClusterID,
                                       uint16_t u16AttributeID,
                                       teZCL_ZCLAttributeType eType );

/** Add a node to a group */
teZcbStatus eZCB_AddGroupMembership(uint16_t u16ShortAddress, uint16_t u16GroupAddress);

/** Remove a node from a group */
teZcbStatus eZCB_RemoveGroupMembership(uint16_t u16ShortAddress, uint16_t u16GroupAddress);

/** Populate the nodes group membership information */
teZcbStatus eZCB_GetGroupMembership(uint16_t u16ShortAddress);

/** Remove a node from all groups */
teZcbStatus eZCB_ClearGroupMemberships(uint16_t u16ShortAddress);

/** Remove a scene in a node */
teZcbStatus eZCB_RemoveScene(uint16_t u16ShortAddress, uint16_t u16GroupAddress, uint8_t u8SceneID);

/** Remove all scenes in a node */
teZcbStatus eZCB_RemoveAllScenes( uint16_t u16ShortAddress, uint16_t u16GroupAddress);

/** Store a scene in a node */
teZcbStatus eZCB_StoreScene(uint16_t u16ShortAddress, uint16_t u16GroupAddress, uint8_t u8SceneID);

/** Recall a scene */
teZcbStatus eZCB_RecallScene(uint16_t u16ShortAddress, uint16_t u16GroupAddress, uint8_t u8SceneID);

/** Get a nodes scene membership */
teZcbStatus eZCB_GetSceneMembership(uint16_t u16ShortAddress, uint16_t u16GroupAddress, uint8_t *pu8NumScenes, uint8_t **pau8Scenes);


#if defined __cplusplus
}
#endif

#endif  /* _ZCB_INC */

// ------------------------------------------------------------------
// End of file
// ------------------------------------------------------------------
