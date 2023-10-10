// ------------------------------------------------------------------
// ZCB_MAIN
// ------------------------------------------------------------------
// Author:    nxf47724
// Copyright: NXP B.V. 2023. All rights reserved
// ------------------------------------------------------------------

#include "newDb.h"
#include "newLog.h"

#include "zcb_main.h"

#define ZCB_DEBUG

#ifdef ZCB_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* ZCB_DEBUG */

// -------------------------------------------------------------
// Handle device announce
// -------------------------------------------------------------

/**
 * \brief Adds an announced device into the database, as JOINED.
 */
void zcbHandleAnnounce( char * mac, char * devstr, char * ty ) {

    int dev = DEVICE_DEV_UNKNOWN;
    if ( strcmp( devstr, "man" ) == 0 ) {
        dev = DEVICE_DEV_MANAGER;
    } else if ( strcmp( devstr, "ui" ) == 0 ) {
        dev = DEVICE_DEV_UI;
    } else if ( strcmp( devstr, "sen" ) == 0 ) {
        dev = DEVICE_DEV_SENSOR;
    } else if ( strcmp( devstr, "pmp" ) == 0 ) {
        dev = DEVICE_DEV_PUMP;
    } else if ( strcmp( devstr, "lmp" ) == 0 ) {
        dev = DEVICE_DEV_LAMP;
    } else if ( strcmp( devstr, "plg" ) == 0 ) {
        dev = DEVICE_DEV_PLUG;
    } else if ( strcmp( devstr, "swi" ) == 0 ) {
        dev = DEVICE_DEV_SWITCH;
    }

    // printf( "Joined %s, %s = %d\n", mac, devstr, dev );
    sprintf( logbuffer, "Announce %s", devstr );
    newLogAdd( NEWLOG_FROM_ZCB_OUT, logbuffer );

    newdb_zcb_t sNode;
    newdb_dev_t device;

    /* Recover Node */
    newDbGetZcb(mac, &sNode);

    if ( newDbGetDevice( mac, &device ) )
    {
      /*
       * Update device type.
       * Note information transmitted over ZB network
       * can be fresher than the one in the NFC tag.
       */
        device.dev = dev;
        newDbStrNcpy( device.ty, ty, LEN_TY );
        device.flags |= FLAG_DEV_JOINED;
        /* Set OnOff state into device DB */
        newDbSetDevice( &device );
    } else if ( newDbGetNewDevice( mac, &device ) ) {  // 3-sep-15: joins always insert
        device.dev = dev;
        newDbStrNcpy( device.ty, ty, LEN_TY );
        device.flags = FLAG_DEV_JOINED | FLAG_ADDED_BY_GW;
        /* Set OnOff state into device DB */
        strcpy(device.cmd, sNode.info);
        newDbSetDevice( &device );
    }
}

// ---------------------------------------------------------------------
// Send update interval message to plug meters
// ---------------------------------------------------------------------

/**
 * \brief Sends a new interval setting to a plug meter
 * \param u64IEEEAddress Mac address of the plug meter
 * \param u16UpdateInterval New interval setting
 */

void SmartPlugUpdateIntervalMsg( uint64_t u64IEEEAddress, uint16_t u16UpdateInterval ) {

    char  mac[LEN_MAC_NIBBLE+2];
    u642nibblestr( u64IEEEAddress, mac );
    uint16_t u16ShortAddress = zcbNodeGetShortAddress( mac );

    DEBUG_PRINTF("vSendPlugMeterUpdateIntervalMsg (0x%llx -> 0x%04x): interval=%d seconds\n",
		 u64IEEEAddress, u16ShortAddress, u16UpdateInterval);

    int ZCL_MANUFACTURER_CODE = 0x1037;  // NXP

    teZcbStatus eStatus = eZCB_WriteAttributeRequest(
	  u16ShortAddress,                                       // ShortAddress
	  E_ZB_CLUSTERID_SIMPLE_METERING,                        // Cluster ID
	  0,                                                     // Direction
	  1,                                                     // Manufacturer Specific
	  ZCL_MANUFACTURER_CODE,                                 // Manufacturer ID
	  E_ZB_ATTRIBUTEID_SM_ATTR_ID_MAN_SPEC_UPDATE_INTERVAL,  // Attr ID
	  E_ZCL_UINT16,                                          // eType
	  (void *)&u16UpdateInterval );                          // &data

    if (E_ZCB_OK != eStatus)
    {
        DEBUG_PRINTF("eZCB_WriteAttributeRequest returned status %d\n", eStatus);
    }
}


// ------------------------------------------------------------------
// Send act message to IoT
// ------------------------------------------------------------------

void sendActuatorOnOff( uint64_t u64IEEEAddress, int sid, int data ) {
    char  mac[16+2];
    u642nibblestr( u64IEEEAddress, mac );
    char * cmd = ( data ) ? "on" : "off";
    // printf( "Act message:  %s", jsonAct( mac, sid, cmd, -1 ) );
    zcbHandleActuator( mac, sid, cmd, -1 );
}

void zcbHandleActuator( char * mac, int sid, char * cmd, int lvl ) {

    if ( cmd ) sprintf( logbuffer, "Actuator %s %s", mac, cmd );
    else if ( lvl >= 0 ) sprintf( logbuffer, "Actuator %s %d", mac, lvl );
    else sprintf( logbuffer, "Actuator %s", mac );
    newLogAdd( NEWLOG_FROM_ZCB_OUT, logbuffer );

    newdb_dev_t device;
    if ( newDbGetDevice( mac, &device ) ) {
        if ( cmd ) newDbStrNcpy( device.cmd, cmd, LEN_CMD );
        if ( lvl >= 0 ) device.lvl = lvl;
        device.flags |= FLAG_DEV_JOINED;
        newDbSetDevice( &device );
    }
}