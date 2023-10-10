// ------------------------------------------------------------------
// zcb main
// ------------------------------------------------------------------
// Author:    nxf47724
// Copyright: NXP B.V. 2022. All rights reserved
// ------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "zcb.h"
#include "ZigbeeDevices.h"

#ifndef __ZCB_MAIN_H__
#define __ZCB_MAIN_H__

#if defined __cplusplus
extern "C" {
#endif

typedef struct sDeviceMapEntry
{
  uint16_t u16Id;
  uint16_t u16ProfileId;
  uint8_t u8DeviceVersion;
  uint16_t u16ResultingId;
} tsDeviceMapEntry;


// ---------------------------------------------------------------------
// Send announce message to IoT
// ---------------------------------------------------------------------
void zcbHandleAnnounce( char * mac, char * devstr, char * ty );
void SmartPlugUpdateIntervalMsg( uint64_t u64IEEEAddress, uint16_t u16UpdateInterval );


void sendActuatorOnOff( uint64_t u64IEEEAddress, int sid, int data );
void zcbHandleActuator( char * mac, int sid, char * cmd, int lvl );


// ---------------------------------------------------------------------
// Send message to Matter
// ---------------------------------------------------------------------



#if defined __cplusplus
}
#endif

#endif  /* _ZCB_MAIN */

// ------------------------------------------------------------------
// End of file
// ------------------------------------------------------------------