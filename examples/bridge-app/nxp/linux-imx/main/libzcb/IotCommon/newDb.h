// ------------------------------------------------------------------
// New DB - Based on shared memory - include file
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2015. All rights reserved
// ------------------------------------------------------------------

#include <limits.h>
#include <stdint.h>

#ifndef __NEWDB_H__
#define __NEWDB_H__

#if defined __cplusplus
extern "C" {
#endif

#define DEVICE_DEV_UNKNOWN       0
#define DEVICE_DEV_MANAGER       1
#define DEVICE_DEV_UI            2
#define DEVICE_DEV_SENSOR        3
#define DEVICE_DEV_UISENSOR      4
#define DEVICE_DEV_PUMP          5
#define DEVICE_DEV_LAMP          6
#define DEVICE_DEV_PLUG          7
#define DEVICE_DEV_SWITCH        8

#define DEVICE_TYPE_UNKNOWN      0
#define DEVICE_TYPE_MAN_HEATING  "h"
#define DEVICE_TYPE_MAN_COOLING  "c"
#define DEVICE_TYPE_LAMP_ONOFF   "onoff"
#define DEVICE_TYPE_LAMP_DIMM    "dim"
#define DEVICE_TYPE_LAMP_COLOR   "col"
#define DEVICE_TYPE_LAMP_TW      "tw"
#define DEVICE_TYPE_SWITCH_ONOFF "onoff"
#define DEVICE_TYPE_SWITCH_DIMM  "dim"
#define DEVICE_TYPE_SWITCH_COLOR "col"

#define MODE_DEV_EMPTY_ALL       0
#define MODE_DEV_EMPTY_CLIMATE   1
#define MODE_DEV_EMPTY_LAMPS     2
#define MODE_DEV_EMPTY_PLUGS     3
#define MODE_DEV_EMPTY_SWITCHES  4

#define FLAG_NONE                0x00
#define FLAG_DEV_JOINED          0x01
#define FLAG_UI_IGNORENEXT_HEAT  0x02
#define FLAG_LASTKELVIN          0x04
#define FLAG_ADDED_BY_GW         0x08
#define FLAG_TOPO_CLEAR          0x10
#define FLAG_UI_IGNORENEXT_COOL  0x20

typedef enum {
    ZCB_STATUS_FREE   = 0,
    ZCB_STATUS_USED,
    ZCB_STATUS_JOINED,
    ZCB_STATUS_LEFT,
} zcbNodeStatus;

#define LEN_MAC_NIBBLE  16
#define LEN_TY          8
#define LEN_NM          20
#define LEN_CMD         8
#define LEN_ROOMNM      30
#define LEN_SYSNM       20
#define LEN_SYSSTR      20

typedef struct newdb_system {
    int id;
    char name[LEN_SYSNM+2];
    int intval;
    char strval[LEN_SYSSTR+2];
    int lastupdate;
} newdb_system_t;


typedef struct newdb_dev {
    int id;
    char mac[LEN_MAC_NIBBLE+2];   // Mac saved as nibble
    int dev;
    char ty[LEN_TY+2];
    int par;
    char nm[LEN_NM+2];
    int heat;              // UI
    int cool;              // UI
    int tmp;               // SENSOR
    int hum;               // SENSOR
    int prs;               // SENSOR
    int co2;               // SENSOR
    int bat;               // SENSOR
    int batl;              // SENSOR
    int als;               // SENSOR
    int xloc;              // SENSOR
    int yloc;              // SENSOR
    int zloc;              // SENSOR
    int sid;               // ACT
    char cmd[LEN_CMD+2];   // ACT
    int lvl;               // ACT
    int rgb;               // LAMP
    int kelvin;            // LAMP
    int act;               // PLUG
    int sum;               // PLUG
    int flags;
    int lastupdate;
} newdb_dev_t;

typedef struct newdb_plughist {
    int id;
    char mac[LEN_MAC_NIBBLE+2];
    int sum;               // PLUG
    int lastupdate;
} newdb_plughist_t;

typedef struct newdb_zcb {
    int id;
    int matterIndex;
    char mac[LEN_MAC_NIBBLE+2];
    int status;
    int saddr;
    int type;
    int lastupdate;
    union
    {
      uint64_t u64;
      struct
      {
        uint8_t hasOnOff:1;
        uint8_t hasIlluminanceSensing:1;
        uint8_t hasTemperatureSensing:1;
        uint8_t hasOccupancySensing:1;

      } sClusterBitmap;
    } uSupportedClusters;
    char info[LEN_CMD+2];
    uint16_t u16ProfileId;
    uint8_t  u8DeviceVersion;
} newdb_zcb_t;

typedef struct zcb_node {
    newdb_zcb_t zcb;
    newdb_dev_t dev;
} ZigbeeDev_t;

typedef int (*deviceCb_t)( newdb_dev_t * pdev );
typedef int (*plughistCb_t)( newdb_plughist_t * phist );
typedef int (*zcbCb_t)( newdb_zcb_t * pzcb );

int newDbOpen( void );
int newDbClose( void );
int newDbSave( void );

void newDbFileLock( void );
void newDbFileUnlock( void );

void newDbStrNcpy( char * dst, char * src, int max );

int newDbGetSystem( char * name, newdb_system_t * psys );
int newDbGetNewSystem( char * name, newdb_system_t * psys ) ;
int newDbSetSystem( newdb_system_t * psys );
int newDbDeleteSystem( char * name );
int newDbEmptySystem( void );

int newDbGetDevice( char * mac, newdb_dev_t * pdev );
int newDbGetNewDevice( char * mac, newdb_dev_t * pdev );
int newDbGetDeviceId( int id, newdb_dev_t * pdev );
int newDbSetDevice( newdb_dev_t * pdev );
char * newDbDeviceGetMac( int id, char * mac );
int newDbDeleteDevice( char * mac );
int newDbEmptyDevices( int mode );
// int newDbLoopDevices( deviceCb_t deviceCb );
int newDbLoopDevicesRoom( int room, deviceCb_t deviceCb );
int newDbDevicesCalcChecksumRoom( int room );
int newDbDevicesSetClearFlag( void );
int newDbDevicesRemoveWithClearFlag( void );


int newDbGetMatchingOrNewPlugHist( char * mac, int min, newdb_plughist_t * phist );
int newDbSetPlugHist( newdb_plughist_t * phist );
int newDbLoopPlugHist( char * mac, plughistCb_t plugHistCb );
int newDbDeletePlugHist( int id );
int newDbGetNumOfPlughist( void );
int newDbEmptyPlugHist( void );

int newDbGetZcb( char * mac, newdb_zcb_t * pzcb );
int newDbGetZcbSaddr( int saddr, newdb_zcb_t * pzcb );
int newDbGetNewZcb( char * mac, newdb_zcb_t * pzcb );
int newDbSetZcb( newdb_zcb_t * pzcb );
int newDbSetZcbMapID( newdb_zcb_t * pzcb, int index);
int newDbCheckZcbType( int saddr);
int newDbEmptyZcb( void );
int newDbLoopZcb( zcbCb_t zcbCb );

char * newDbSerializeSystem( int MAXBUF, char * buf );
char * newDbSerializeRooms( int MAXBUF, char * buf );
char * newDbSerializeDevs( int MAXBUF, char * buf );
char * newDbSerializePlugHist( int MAXBUF, char * buf );
char * newDbSerializeZcb( int MAXBUF, char * buf );

char * newDbSerializePlugs( int MAXBUF, char * buf );
char * newDbSerializeLamps( int MAXBUF, char * buf );
char * newDbSerializeLampsAndPlugs( int MAXBUF, char * buf );
char * newDbSerializeClimate( int MAXBUF, char * buf );

int newDbGetLastupdateRooms( void );
int newDbGetLastupdateDevices( void );
int newDbGetLastupdateSystem( void );
int newDbGetLastupdateZcb( void );
int newDbGetLastupdatePlugHist( void );

int newDbGetLastupdatePlug( void );
int newDbGetLastupdateLamp( void );
int newDbGetLastupdateClimate( void );

void newDbPrintPlugHist( void );
void newDbPrintDevices (void);

void newDbPrintzcbs( void );
void newDbReset(void);
int newDbGetZigbeeDeviceList(ZigbeeDev_t** zbList, int maxDevices, int *nodeNum);
int newDbfreeZigbeeDeviceList(ZigbeeDev_t** zbList, int nodeNum);

void newDbSetNwkState( uint8_t state );
void newDbGetNwkState( int *state );

#if defined __cplusplus
}
#endif

#endif /* __NEWDB_H__ */