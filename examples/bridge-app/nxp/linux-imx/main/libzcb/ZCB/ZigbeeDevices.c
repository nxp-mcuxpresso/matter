/****************************************************************************
 *
 * MODULE:             ZigbeeDevices
 *
 * COMPONENT:          ZigbeeDevices.c
 *
 * REVISION:           $Revision$
 *
 * DATED:              $Date$
 *
 * DESCRIPTION:
 *
 ****************************************************************************
 *
 * This software is owned by NXP B.V. and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on NXP products [NXP Microcontrollers such as JN5148, JN5142, JN5139].
 * You, and any third parties must reproduce the copyright and warranty notice
 * and any other legend of ownership on each copy or partial copy of the
 * software.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Copyright NXP B.V. 2016. All rights reserved
 *
 ***************************************************************************/
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "ZigbeeDevices.h"
#include "ZigbeeConstant.h"
#include "SerialLink.h"
// #include "cJSON.h"
#include "newDb.h"
#include "zcb.h"

/* Defined in zcb_main.c */
extern void announceDevice(
    uint64_t u64IEEEAddress,
    uint16_t u16DeviceId,
    uint16_t u16ProfileId,
    uint8_t  u8DeviceVersion);

#define ZD_DEBUG(...) printf(__VA_ARGS__)

#define NULL_DEVICE {NULL, 0, NULL}

static tsZDDevice asZigbeeDeviceTable [] =
{
    CZD_DEVICE(On/off light,                         CZD_PROFILE_ZLL, 0x0000, CZD_DOMAIN_LIGHT_DEV),
    CZD_DEVICE(On/Off Switch,                        CZD_PROFILE_ZHA, 0x0000, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Level Control Switch,                 CZD_PROFILE_ZHA, 0x0001, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(On/Off Output,                        CZD_PROFILE_ZHA, 0x0002, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Level Controllable Output,            CZD_PROFILE_ZHA, 0x0003, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Scene Selector,                       CZD_PROFILE_ZHA, 0x0004, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Configuration Tool,                   CZD_PROFILE_ZHA, 0x0005, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Remote Control,                       CZD_PROFILE_ZHA, 0x0006, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Combined Interface,                   CZD_PROFILE_ZHA, 0x0007, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Range Extender,                       CZD_PROFILE_ZHA, 0x0008, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Mains Power Outlet,                   CZD_PROFILE_ZHA, 0x0009, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Door Lock,                            CZD_PROFILE_ZHA, 0x000A, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Door Lock Controller,                 CZD_PROFILE_ZHA, 0x000B, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Simple Sensor,                        CZD_PROFILE_ZHA, 0x000C, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Consumption Awareness Device,         CZD_PROFILE_ZHA, 0x000D, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(On/off plug-in unit,                  CZD_PROFILE_ZLL, 0x0010, CZD_DOMAIN_LIGHT_DEV),
    CZD_DEVICE(Home Gateway,                         CZD_PROFILE_ZHA, 0x0050, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Smart plug,                           CZD_PROFILE_ZHA, 0x0051, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(White Goods,                          CZD_PROFILE_ZHA, 0x0052, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(Meter Interface,                      CZD_PROFILE_ZHA, 0x0053, CZD_DOMAIN_GENERIC),
    CZD_DEVICE(On/Off Light,                         CZD_PROFILE_ZHA, 0x0100, CZD_DOMAIN_LIGHTING),
    CZD_DEVICE(Dimmable Light,                       CZD_PROFILE_ZLL, 0x0100, CZD_DOMAIN_LIGHT_DEV),
    CZD_DEVICE(Dimmable Light,                       CZD_PROFILE_ZHA, 0x0101, CZD_DOMAIN_LIGHTING),
    CZD_DEVICE(Color Dimmable Light,                 CZD_PROFILE_ZHA, 0x0102, CZD_DOMAIN_LIGHTING),
    CZD_DEVICE(On/Off Light Switch,                  CZD_PROFILE_ZHA, 0x0103, CZD_DOMAIN_LIGHTING),
    CZD_DEVICE(Dimmer Switch,                        CZD_PROFILE_ZHA, 0x0104, CZD_DOMAIN_LIGHTING),
    CZD_DEVICE(Color Dimmer Switch,                  CZD_PROFILE_ZHA, 0x0105, CZD_DOMAIN_LIGHTING),
    CZD_DEVICE(Light Sensor,                         CZD_PROFILE_ZHA, 0x0106, CZD_DOMAIN_LIGHTING),
    CZD_DEVICE(Occupancy Sensor,                     CZD_PROFILE_ZHA, 0x0107, CZD_DOMAIN_LIGHTING),
    CZD_DEVICE(On/off ballast,                       CZD_PROFILE_ZHA, 0x0108, ""),
    CZD_DEVICE(Dimmable ballast,                     CZD_PROFILE_ZHA, 0x0109, ""),
    CZD_DEVICE(On/off plug-in unit,                  CZD_PROFILE_ZLO, 0x010A, ""),
    CZD_DEVICE(Dimmable plug-in unit,                CZD_PROFILE_ZLO, 0x010B, ""),
    CZD_DEVICE(Color temperature light,              CZD_PROFILE_ZLO, 0x010C, ""),
    CZD_DEVICE(Extended color light,                 CZD_PROFILE_ZLO, 0x010D, ""),
    CZD_DEVICE(Light level sensor,                   CZD_PROFILE_ZLO, 0x010E, ""),
    CZD_DEVICE(Dimmable plug-in unit,                CZD_PROFILE_ZLL, 0x0110, CZD_DOMAIN_LIGHT_DEV),
    CZD_DEVICE(Shade,                                CZD_PROFILE_ZHA, 0x0200, CZD_DOMAIN_CLOSURE),
    CZD_DEVICE(Color light,                          CZD_PROFILE_ZLL, 0x0200, CZD_DOMAIN_LIGHT_DEV),
    CZD_DEVICE(Shade Controller,                     CZD_PROFILE_ZHA, 0x0201, CZD_DOMAIN_CLOSURE),
    CZD_DEVICE(Window Covering Device,               CZD_PROFILE_ZHA, 0x0202, CZD_DOMAIN_CLOSURE),
    CZD_DEVICE(Window Covering Controller,           CZD_PROFILE_ZHA, 0x0203, CZD_DOMAIN_CLOSURE),
    CZD_DEVICE(Extended color light,                 CZD_PROFILE_ZLL, 0x0210, CZD_DOMAIN_LIGHT_DEV),
    CZD_DEVICE(Color temperature light,              CZD_PROFILE_ZLL, 0x0220, CZD_DOMAIN_LIGHT_DEV),
    CZD_DEVICE(Heating/Cooling Unit,                 CZD_PROFILE_ZHA, 0x0300, CZD_DOMAIN_HVAC),
    CZD_DEVICE(Thermostat,                           CZD_PROFILE_ZHA, 0x0301, CZD_DOMAIN_HVAC),
    CZD_DEVICE(Temperature Sensor,                   CZD_PROFILE_ZHA, 0x0302, CZD_DOMAIN_HVAC),
    CZD_DEVICE(Pump,                                 CZD_PROFILE_ZHA, 0x0303, CZD_DOMAIN_HVAC),
    CZD_DEVICE(Pump Controller,                      CZD_PROFILE_ZHA, 0x0304, CZD_DOMAIN_HVAC),
    CZD_DEVICE(Pressure Sensor,                      CZD_PROFILE_ZHA, 0x0305, CZD_DOMAIN_HVAC),
    CZD_DEVICE(Flow Sensor,                          CZD_PROFILE_ZHA, 0x0306, CZD_DOMAIN_HVAC),
    CZD_DEVICE(Mini Split AC,                        CZD_PROFILE_ZHA, 0x0307, CZD_DOMAIN_HVAC),
    CZD_DEVICE(IAS Control and Indicating Equipment, CZD_PROFILE_ZHA, 0x0400, CZD_DOMAIN_IAS),
    CZD_DEVICE(IAS Ancillary Control Equipment,      CZD_PROFILE_ZHA, 0x0401, CZD_DOMAIN_IAS),
    CZD_DEVICE(IAS Zone,                             CZD_PROFILE_ZHA, 0x0402, CZD_DOMAIN_HVAC),
    CZD_DEVICE(IAS Warning Device,                   CZD_PROFILE_ZHA, 0x0403, CZD_DOMAIN_IAS),
    CZD_DEVICE(Color controller,                     CZD_PROFILE_ZLL, 0x0800, CZD_DOMAIN_CTRL_DEV),
    CZD_DEVICE(Color controller,                     CZD_PROFILE_ZLO, 0x0800, ""),
    CZD_DEVICE(Color scene controller,               CZD_PROFILE_ZLO, 0x0810, ""),
    CZD_DEVICE(Color scene remote control,           CZD_PROFILE_ZLL, 0x0810, CZD_DOMAIN_CTRL_DEV),
    CZD_DEVICE(Non-color controller,                 CZD_PROFILE_ZLO, 0x0820, ""),
    CZD_DEVICE(Non-color controller,                 CZD_PROFILE_ZLL, 0x0820, ""),
    CZD_DEVICE(Non-color scene controller,           CZD_PROFILE_ZLO, 0x0830, CZD_DOMAIN_CTRL_DEV),
    CZD_DEVICE(Non-color scene controller,           CZD_PROFILE_ZLL, 0x0830, CZD_DOMAIN_CTRL_DEV),
    CZD_DEVICE(Control bridge,                       CZD_PROFILE_ZLO, 0x0840, ""),
    CZD_DEVICE(Control bridge,                       CZD_PROFILE_ZLL, 0x0840, ""),
    CZD_DEVICE(On/off sensor,                        CZD_PROFILE_ZLO, 0x0850, CZD_DOMAIN_CTRL_DEV),
    CZD_DEVICE(On/off sensor,                        CZD_PROFILE_ZLL, 0x0850, CZD_DOMAIN_CTRL_DEV),
    NULL_DEVICE,
};

#define NULL_CLUSTER {0, NULL}

tsZDCluster asZigbeeClusters [] =
{
    CZD_CLUSTER(0x0000, Basic),
    CZD_CLUSTER(0x0001, Power),
    CZD_CLUSTER(0x0002, Device),
    CZD_CLUSTER(0x0003, Identify),
    CZD_CLUSTER(0x0004, Groups),
    CZD_CLUSTER(0x0005, Scenes),
    CZD_CLUSTER(0x0006, On/off),
    CZD_CLUSTER(0x0007, On/off Switch Configuration),
    CZD_CLUSTER(0x0008, Level Control),
    CZD_CLUSTER(0x0009, Alarms),
    CZD_CLUSTER(0x000a, Time),
    CZD_CLUSTER(0x000b, RSSI Location),
    CZD_CLUSTER(0x000c, Analog Input (Basic)),
    CZD_CLUSTER(0x000d, Analog Output (Basic)),
    CZD_CLUSTER(0x000e, Analog Value (Basic)),
    CZD_CLUSTER(0x000f, Binary Input (Basic)),
    CZD_CLUSTER(0x0010, Binary Output (Basic)),
    CZD_CLUSTER(0x0011, Binary Value (Basic)),
    CZD_CLUSTER(0x0012, Multistate Input (Basic)),
    CZD_CLUSTER(0x0013, Multistate Output (Basic)),
    CZD_CLUSTER(0x0014, Multistate Value (Basic)),
    CZD_CLUSTER(0x0015, Commissioning),
    CZD_CLUSTER(0x0100, Shade Configuration),
    CZD_CLUSTER(0x0101, Door Lock),
    CZD_CLUSTER(0x0200, Pump Configuration and Control),
    CZD_CLUSTER(0x0201, Thermostat),
    CZD_CLUSTER(0x0202, Fan Control),
    CZD_CLUSTER(0x0203, Dehumidification Control),
    CZD_CLUSTER(0x0204, Thermostat User Interface Configuration),
    CZD_CLUSTER(0x0300, Color control),
    CZD_CLUSTER(0x0301, Ballast Configuration),
    CZD_CLUSTER(0x0400, Illuminance measurement),
    CZD_CLUSTER(0x0401, Illuminance level sensing),
    CZD_CLUSTER(0x0402, Temperature measurement),
    CZD_CLUSTER(0x0403, Pressure measurement),
    CZD_CLUSTER(0x0404, Flow measurement),
    CZD_CLUSTER(0x0405, Relative humidity measurement),
    CZD_CLUSTER(0x0406, Occupancy sensing),
    CZD_CLUSTER(0x0500, IAS Zone),
    CZD_CLUSTER(0x0501, IAS ACE),
    CZD_CLUSTER(0x0502, IAS WD),
    CZD_CLUSTER(0x0600, Generic Tunnel),
    CZD_CLUSTER(0x0601, BACnet Protocol Tunnel),
    CZD_CLUSTER(0x0602, Analog Input (BACnet Regular)),
    CZD_CLUSTER(0x0603, Analog Input (BACnet Extended)),
    CZD_CLUSTER(0x0604, Analog Output (BACnet Regular)),
    CZD_CLUSTER(0x0605, Analog Output (BACnet Extended)),
    CZD_CLUSTER(0x0606, Analog Value (BACnet Regular)),
    CZD_CLUSTER(0x0607, Analog Value (BACnet Extended)),
    CZD_CLUSTER(0x0608, Binary Input (BACnet Regular)),
    CZD_CLUSTER(0x0609, Binary Input (BACnet Extended)),
    CZD_CLUSTER(0x060a, Binary Output (BACnet Regular)),
    CZD_CLUSTER(0x060b, Binary Output (BACnet Extended)),
    CZD_CLUSTER(0x060c, Binary Value (BACnet Regular)),
    CZD_CLUSTER(0x060d, Binary Value (BACnet Extended)),
    CZD_CLUSTER(0x060e, Multistate Input (BACnet Regular)),
    CZD_CLUSTER(0x060f, Multistate Input (BACnet Extended)),
    CZD_CLUSTER(0x0610, Multistate Output (BACnet Regular)),
    CZD_CLUSTER(0x0611, Multistate Output (BACnet Extended)),
    CZD_CLUSTER(0x0612, Multistate Value (BACnet Regular)),
    CZD_CLUSTER(0x0613, Multistate Value (BACnet Extended)),
    CZD_CLUSTER(0x0702, Metering), // HA
    CZD_CLUSTER(0x0b00, EN50523 Appliance Identification), //HA
    CZD_CLUSTER(0x0b01, Meter Identification),  //HA
    CZD_CLUSTER(0x0b02, EN50523 Appliance events and Alert),  //HA
    CZD_CLUSTER(0x0b03, Appliance statistics), // HA
    CZD_CLUSTER(0x0b04, Electricity Measurement),  // HA
    CZD_CLUSTER(0x0b05, Diagnostics),  //HA
    NULL_CLUSTER
};

static int
ZDIsDeviceHandled(
    uint64_t u64IEEEAddress,
    uint16_t u16DeviceID)
{
  int result = 1;

  // Cluster discovery for each device
  switch ( u16DeviceID )
  {
  case SIMPLE_DESCR_LIGHT_SENSOR:
    //zcbHandleAnnounce( mac, "sen", "light" );
    break;

  case SIMPLE_DESCR_SMOKE_SENSOR:
    //zcbHandleAnnounce( mac, "sen", "smoke" );
    break;

  case SIMPLE_DESCR_WINDOW_SENSOR:
    //zcbHandleAnnounce( mac, "sen", "win" );
    break;
  default :
    result = 0;
  }
  return result;
} /* end of ZCB_IsDeviceHandled() */

tsZDDevice *
ZDLookupDevice(
    uint16_t u16ProfileId,
    uint16_t u16DeviceId,
    uint8_t  u8DeviceVersion)
{
  tsZDDevice * psFoundDevice = NULL;

  /*
   *  Find best match. Possible cases:
   *  - match on Device ID and (profile and device version) --> got it !
   *  - match on Device ID only, 2 sub cases
   *    - same device ID only, but single occurrence in the table --> got it !
   *    - same device ID, several occurrences --> perfect match is in the next records
   */

  /* Look up for device ID */
  for (psFoundDevice = &(asZigbeeDeviceTable[0]);
      psFoundDevice->szName != NULL;
      psFoundDevice++)
  {
    if (psFoundDevice->u16Id == u16DeviceId &&
        psFoundDevice->szProfile == u16ProfileId ) // FIXME
    {
      switch (u16ProfileId)
      {
        case CZD_PROFILE_ID_ZHA_ZLO:
          /* */
          break;
          /* */
        case CZD_PROFILE_ID_ZLL:
          break;
        default:
          break;
      }
    }
  }
  return psFoundDevice;
}

// /*!
//  *
//  * @param psJsonClusters (In/out) JSON Cluster Array
//  * @param psClusters     (In)     Clusters supported by the device
//  */
// static void
// ZDPopulateClusters(
//     cJSON *         psJsonClusters,
//     tsZDClusterList * psClusters)
// {
//   uint8_t u8Index;
//   tsZDCluster * psCluster;
//   uint16_t * pu16ClusterId = (uint16_t*) &(psClusters->au16Clusters);

//   for (u8Index = 0; u8Index < psClusters->u8ClusterCount;
//       u8Index++, pu16ClusterId++)
//   {
//     /* Look up for cluster ID */
//     uint16_t u16Id= ntohs(*pu16ClusterId);
//     for (psCluster = &(asZigbeeClusters[0]); psCluster != NULL; psCluster++ )
//     {
//       if (psCluster->u16Id == u16Id)
//       {
//         /* Got it ! add to cluster list */
//         cJSON * psJsonCluster = cJSON_CreateObject();
//         cJSON_AddNumberToObject(psJsonCluster, psCluster->szName, u16Id);
//         cJSON_AddItemToArray(psJsonClusters, psJsonCluster);
//         break;
//       }
//     }
//     /* If cluster is not recognized (private/reserved Id) */
//     if (psCluster == NULL)
//     {
//       /* Use its ID in hex as name */
//       char szHexClusterId[sizeof(uint16_t)+1];
//       sprintf(szHexClusterId, "%04x", u16Id);
//       cJSON * psJsonCluster = cJSON_CreateObject();
//       cJSON_AddNumberToObject(psJsonCluster, szHexClusterId, u16Id);
//       cJSON_AddItemToArray(psJsonClusters, psJsonCluster);
//     }
//   }
// }

// /*!
//  *
//  * @param pvUser    (not used) pointer to Cookie from callback caller
//  * @param u16Length (In) Message length
//  * @param pvMessage (In) pointer to message
//  */
// void ZDSimpleDescResp(
//     void *pvUser,
//     uint16_t u16Length,
//     void *pvMessage)
// {
//   tsZDDevice* psDevice = NULL;

//   tsZDSimpleDescRsp *psMessage = (tsZDSimpleDescRsp *)pvMessage;
//   tsZDClusterList *psInputClusters = (tsZDClusterList *) &psMessage->sInputClusters;
//   uint8_t * pU8AddrOutputClusters = (uint8_t*) psInputClusters
//       + 1 // Count field
//       + sizeof(uint16_t)*psInputClusters->u8ClusterCount; // input cluster array

//   tsZDClusterList *psOutputClusters = (tsZDClusterList *) ((void*)pU8AddrOutputClusters);

//   uint16_t u16ShortAddress  = ntohs(psMessage->u16ShortAddress);
//   uint16_t u16DeviceID      = ntohs(psMessage->u16DeviceID);

//   /* TODO : check if dev already exists */
//   /* Create Device */
//   cJSON * psJsonDesc = cJSON_CreateObject();
//   cJSON * psInputClusterList = cJSON_CreateArray();
//   cJSON * psOutputClusterList = cJSON_CreateArray();

//   /* Look up for device ID */
//   for (psDevice = &(asZigbeeDeviceTable[0]);
//       psDevice->szDomain != NULL;
//       psDevice++)
//   {
//     if (psDevice->u16Id == u16DeviceID)
//     {
//       //cJSON_AddStringToObject(psJsonDesc, "Device", psDevice->szName);
//       cJSON_AddItemToObjectCS(psJsonDesc, "Device",
//           cJSON_CreateString(psDevice->szName));
//       break;
//     }
//   }

//   /* If device is not recognized (private/reserved Id) */
//   if (psDevice == NULL)
//   {
//     /* Just set its ID as a string */
//     // cJSON_AddIntToObject(psJsonDesc, "Device", (int) u16DeviceID);
//     cJSON_AddItemToObjectCS(psJsonDesc, "Device",
//         cJSON_CreateNumber((int) u16DeviceID));
//   }

//   /* Store Endpoint Id */
//   cJSON_AddNumberToObject(psJsonDesc, "EndPointId", psMessage->u8Endpoint);

//   /* Create Populate cluster lists */
//   cJSON_AddItemToObjectCS(psJsonDesc, "InputClusters", psInputClusterList);
//   ZDPopulateClusters(psInputClusterList, psInputClusters);
//   cJSON_AddItemToObjectCS(psJsonDesc, "outputClusters", psOutputClusterList);
//   ZDPopulateClusters(psOutputClusterList, psOutputClusters);
//   /* TODO : store the JSON somewhere */

//   /* For each cluster, request supported attributes */
//   ZDAttrDiscReq(psMessage);
//   return;
// }

/*!
 * Sends message Attribute Discovery Request
 * @param psMessage (In) previously received Simple Descriptor Response.
 */
void
ZDAttrDiscReq(tsZDSimpleDescRsp * psMessage)
{
  uint8_t u8SequenceNo;
  teSL_Status eStatus;

  tsZDAttrDiscReq sAttrDiscReq =
  {
      .u8AddressMode              = E_ZD_ADDRESS_MODE_SHORT_NO_ACK,
      .u16ShortAddress = psMessage->u16ShortAddress,
      .u8SourceEndPointId            = CZD_ENDPOINT_ATTR,
      .u8DestinationEndPointId       = psMessage->u8Endpoint,
      .u16ClusterId                  = htons(0),
      .u16AttributeId                = htons(0),
      .bDirectionIsServerToClient    = 0,
      .bIsManufacturerSpecific       = 0,
      .u16ManufacturerCode           = 0,
      .u8MaximumNumberOfIdentifiers = 10,
  };

  ZD_DEBUG( "Send Attribute Discovery Request to 0x%04X\n",
      ntohs(psMessage->u16ShortAddress));

  eStatus = eSL_SendMessage(E_SL_MSG_ATTRIBUTE_DISCOVERY_REQUEST,
      sizeof(tsZDAttrDiscReq), &sAttrDiscReq, &u8SequenceNo);
  if (eStatus != E_SL_OK)
  {
    ZD_DEBUG( "Send Attribute Discovery Request to 0x%04X : Fail (0x%x)\n",
        ntohs(psMessage->u16ShortAddress), eStatus);
  }

  return;
}

/*!
 * Handle Attribute Discovery Response message
 * @param pvUser    (not used) pointer to Cookie from callback caller
 * @param u16Length (In) Message length
 * @param pvMessage (In) pointer to message
 */
void ZDAttrDiscResp(
    void *pvUser,
    uint16_t u16Length,
    void *pvMessage)
{
}


void ZDReadAttrReq(
    uint16_t          u16ShortAddress,
    uint8_t           u8SrcEndpoint,
    uint8_t           u8DestEndpoint,
    eZigbee_ClusterID u16ClusterID,
    uint8_t           u8NbAttr,
    uint16_t*         au16AttrList)
{
  uint8_t u8SequenceNo;
  teSL_Status eStatus;

  tsZDReadAttrReq sReadAttrReq =
  {
      .u8AddressMode              = E_ZD_ADDRESS_MODE_SHORT,
      .u16ShortAddress            = htons(u16ShortAddress),
      .u8SourceEndPointId         = u8SrcEndpoint,
      .u8DestinationEndPointId    = u8DestEndpoint,
      .u16ClusterId               = htons(E_ZB_CLUSTERID_ONOFF),
      .bDirectionIsServerToClient = 0,
      .bIsManufacturerSpecific    = 0,
      .u16ManufacturerCode        = 0,
      .u8NumberOfAttributes       = u8NbAttr,
  };

  for (uint16_t i = 0; i < u8NbAttr; i++)
  {
    sReadAttrReq.au16AttributeList[i] = htons(au16AttrList[i]);
  }

  ZD_DEBUG( "Send Read Attribute Request to 0x%04x, endpoints: 0x%02x -> 0x%02x\n",
      u16ShortAddress, u8SrcEndpoint, u8DestEndpoint);

  eStatus = eSL_SendMessage(E_SL_MSG_READ_ATTRIBUTE_REQUEST,
      sizeof(tsZDReadAttrReq)+sizeof(uint16_t)*(u8NbAttr-MAX_NB_READ_ATTRIBUTES), &sReadAttrReq, &u8SequenceNo);
  if (eStatus != E_SL_OK)
  {
    ZD_DEBUG( "Send Read Attribute Request to 0x%04x : Fail (0x%x)\n",
        u16ShortAddress, eStatus);
  }

  return;
}

/*!
 * Handle reception of response to attribute read request.
 *
 * @param pvUser    Cookie from callback registration
 * @param u16Length Length of received message
 * @param pvMessage pointer to message content
 */
void
ZDReadAttrResp(
    void *pvUser,
    uint16_t u16Length,
    void *pvMessage)
{
  ZD_DEBUG( "\n************ ZCB_HandleReadAttrResp\n" );
  tsZDReadAttrResp *psMessage = (tsZDReadAttrResp *) pvMessage;
  printf(" ddd ---cluster id  %d\n", ntohs(psMessage->u16ClusterId));

  /* Retrieve device from database */
  newdb_zcb_t sZcbDev;
  uint16_t    u16ShortAddr = ntohs(psMessage->u16ShortAddress);
  uint64_t    extendedAddress= zcbNodeGetExtendedAddress(u16ShortAddr);

  if (!newDbGetZcbSaddr(u16ShortAddr, &sZcbDev))
  {
    ZD_DEBUG( "Error : device with short address %04x not found in DB\n", u16ShortAddr);
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
        printf("debug ======== status  %d\n", psMessage->au8AttributeValue[0]);
        strncpy(sZcbDev.info,
            (psMessage->au8AttributeValue[0] ? "on" : "off"),
            LEN_CMD);        /* Store in ZCB DB */
        newDbSetZcb(&sZcbDev);
      }
      else
      {
        ZD_DEBUG( "Error 0x%02x : Read Attr On/off for device 0x%04x failed\n",
            psMessage->u8AttributeStatus,
            u16ShortAddr);
        ZD_DEBUG( "Cluster: 0x%04x , Attr : 0x%04x, type : 0x%02x\n",
            ntohs(psMessage->u16ClusterId),
            ntohs(psMessage->u16AttributeId),
            psMessage->u8AttributeDataType
            );
      }
    }
    /* Store on/off state in DB, it will then be push in Device DB during Announce flow */
    if (extendedAddress)
    {
      announceDevice( extendedAddress,
          (uint16_t) sZcbDev.type,
          sZcbDev.u16ProfileId,
          sZcbDev.u8DeviceVersion );
    }

    if ( sZcbDev.type == SIMPLE_DESCR_THERMOSTAT ) {
      // Let coordinator become part of the AAL group (to receive group-cast thermostat messages)
      eZCB_AddGroupMembership( 0x0000, 0x0AA1 );
    }
  }
  return;
}


void
ZDActiveEndPointReq(uint16_t u16ShortAddress)
{
  uint8_t u8SequenceNo;
  teSL_Status eStatus;

  tsZDActiveEndPointReq sReadAttrReq =
  {
      .u16ShortAddress            = htons(u16ShortAddress),
  };

  ZD_DEBUG( "Send Active End Point Request to 0x%04x\n",
      u16ShortAddress);

  eStatus = eSL_SendMessageNoWait(E_SL_MSG_READ_ATTRIBUTE_REQUEST,
      sizeof(tsZDReadAttrReq), &sReadAttrReq, &u8SequenceNo);
  if (eStatus != E_SL_OK)
  {
    ZD_DEBUG( "Send Read Attribute Request to 0x%04x : Fail (0x%x)\n",
        u16ShortAddress, eStatus);
  }

  return;
}

void
ZDActiveEndPointResp(
    void *pvUser,
    uint16_t u16Length,
    void *pvMessage)
{
  /*
   * <Sequence number: uint8_t>
   * <status: uint8_t>
   * <Address: uint16_t>
   * <endpoint count: uint8_t>
   * <active endpoint list: each data element of the type uint8_t >
   */

}
