/****************************************************************************
 *
 * MODULE:             ZigbeeDevices
 *
 * COMPONENT:          ZigbeeDevices.h
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

#ifndef  ZIGBEEDEVICES_H_INCLUDED
#define  ZIGBEEDEVICES_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

  /****************************************************************************/
  /***        Include files                                                 ***/
  /****************************************************************************/
#include <stdint.h>
#include "ZigbeeConstant.h"

  /****************************************************************************/
  /***        Macro Definitions                                             ***/
  /****************************************************************************/

#define PACKED __attribute__((__packed__))

  // Domains covered by ZHA profile
#define CZD_DOMAIN_GENERIC   "Generic"
#define CZD_DOMAIN_LIGHTING  "Lighting"
#define CZD_DOMAIN_CLOSURE   "Closure"
#define CZD_DOMAIN_HVAC      "HVAC"
#define CZD_DOMAIN_IAS       "IAS"
#define CZD_DOMAIN_LIGHT_DEV "Lighting devices"
#define CZD_DOMAIN_CTRL_DEV  "Controller devices"

  // Profiles name
#define CZD_PROFILE_ZHA     "ZHA"
#define CZD_PROFILE_ZLL     "ZLL"
#define CZD_PROFILE_ZLO     "ZLO"

  // Profiles ID
#define CZD_PROFILE_ID_ZHA_ZLO  0x0104
#define CZD_PROFILE_ID_ZHA      0x0104
#define CZD_PROFILE_ID_ZLO      0x0104
#define CZD_PROFILE_ID_ZLL      0xC05E

// Device versions
#define CZD_DEVICE_VERSION_ZHA  0x02
#define CZD_DEVICE_VERSION_ZLL  0x02
#define CZD_DEVICE_VERSION_ZLO  0x01

  // Macro to bind Device types, domains, and device Id
#define CZD_DEVICE(szName, szProfile, u16Id, szDomain) \
  {#szName, u16Id,  szProfile, szDomain}
#define CZD_CLUSTER(u16Id, szName) {u16Id, #szName}

  // TODO : the definitions below are really confusing
  // ZCb End points
#define CZD_ENDPOINT_ZHA             1   // Zigbee-HA
#define CZD_ENDPOINT_ONOFF           1   // ON/OFF cluster
#define CZD_ENDPOINT_GROUP           1   // GROUP cluster
#define CZD_ENDPOINT_SCENE           1   // SCENE cluster
#define CZD_ENDPOINT_SIMPLE          1   // SimpleDescriptor cluster
#define CZD_ENDPOINT_LAMP            1   // ON/OFF, Color control
#define CZD_ENDPOINT_TUNNEL          1   // For tunnel mesages
#define CZD_ENDPOINT_ATTR            1   // For attrs

#define CZD_NW_STATUS_SUCCESS                   0x00 // A request has been executed successfully.
#define CZD_NW_STATUS_INVALID_PARAMETER         0xc1 // An invalid or out-of-range parameter has been passed to a primitive from the next higher layer.
#define CZD_NW_STATUS_INVALID_REQUEST           0xc2 // The next higher layer has issued a request that is invalid or cannot be executed given the current state of the NWK lay-er.
#define CZD_NW_STATUS_NOT_PERMITTED             0xc3 // An NLME-JOIN.request has been disallowed.
#define CZD_NW_STATUS_STARTUP_FAILURE           0xc4 // An NLME-NETWORK-FORMATION.request has failed to start a network.
#define CZD_NW_STATUS_ALREADY_PRESENT           0xc5 // A device with the address supplied to the NLME-DIRECT-JOIN.request is already present in the neighbor table of the device on which the NLME-DIRECT-JOIN.request was issued.
#define CZD_NW_STATUS_SYNC_FAILURE              0xc6 // Used to indicate that an NLME-SYNC.request has failed at the MAC layer.
#define CZD_NW_STATUS_NEIGHBOR_TABLE_FULL       0xc7 // An NLME-JOIN-DIRECTLY.request has failed because there is no more room in the neighbor table.
#define CZD_NW_STATUS_UNKNOWN_DEVICE            0xc8 // An NLME-LEAVE.request has failed because the device addressed in the parameter list is not in the neighbor table of the issuing device.
#define CZD_NW_STATUS_UNSUPPORTED_ATTRIBUTE     0xc9 // An NLME-GET.request or NLME-SET.request has been issued with an unknown attribute identifier.
#define CZD_NW_STATUS_NO_NETWORKS               0xca // An NLME-JOIN.request has been issued in an environment where no networks are detectable.
#define CZD_NW_STATUS_MAX_FRM_COUNTER           0xcc // Security processing has been attempted on an outgoing frame, and has failed because the frame counter has reached its maximum value.
#define CZD_NW_STATUS_NO_KEY                    0xcd // Security processing has been attempted on an outgoing frame, and has failed because no key was available with which to process it.
#define CZD_NW_STATUS_BAD_CCM_OUTPUT            0xce // Security processing has been attempted on an outgoing frame, and has failed because the security engine produced erroneous output.
#define CZD_NW_STATUS_ROUTE_DISCOVERY_FAILED    0xd0 // An attempt to discover a route has failed due to a reason other than a lack of routing capacity.
#define CZD_NW_STATUS_ROUTE_ERROR               0xd1 // An NLDE-DATA.request has failed due to a routing failure on the sending device or an NLME-ROUTE-DISCOVERY.request has failed due to the cause cited in the accompanying NetworkStatusCode.
#define CZD_NW_STATUS_BT_TABLE_FULL             0xd2 // An attempt to send a broadcast frame or member mode mul-ticast has failed due to the fact that there is no room in the BTT.
#define CZD_NW_STATUS_FRAME_NOT_BUFFERED        0xd3 // An NLDE-DATA.request has failed due to insufficient buffering available.  A non-member mode multicast frame was discarded pending route discovery.

#define MAX_NB_READ_ATTRIBUTES 10

  /****************************************************************************/
  /***        Type Definitions                                              ***/
  /****************************************************************************/

  typedef enum
  {
    E_ZD_ADDRESS_MODE_BOUND = 0x00,
    E_ZD_ADDRESS_MODE_GROUP = 0x01,
    E_ZD_ADDRESS_MODE_SHORT = 0x02,
    E_ZD_ADDRESS_MODE_IEEE = 0x03,
    E_ZD_ADDRESS_MODE_BROADCAST = 0x04,
    E_ZD_ADDRESS_MODE_NO_TRANSMIT = 0x05,
    E_ZD_ADDRESS_MODE_BOUND_NO_ACK = 0x06,
    E_ZD_ADDRESS_MODE_SHORT_NO_ACK = 0x07,
    E_ZD_ADDRESS_MODE_IEEE_NO_ACK = 0x08,
  } teZDAddressMode;

  /* Broadcast modes for data requests */
  typedef enum
  {
    E_ZD_BROADCAST_ALL, /* broadcast to all nodes, including all end devices */
    E_ZD_BROADCAST_RX_ON, /* broadcast to all nodes with their receivers enabled when idle */
    E_ZD_BROADCAST_ZC_ZR /* broadcast only to coordinator and routers */
  } teZDBroadcastMode;

  typedef struct
  {
    teZDAddressMode eAddressMode;
    union
    {
      uint16_t u16GroupAddress;
      uint16_t u16ShortAddress;
      uint64_t u64IEEEAddress;
      teZDBroadcastMode eBroadcastMode;
    } uAddress;
  }PACKED tsZDAddress;

  /*
   * Types used to handle Zigbee devices
   */
  typedef struct
  {
    char *   szName;
    uint16_t u16Id;
    char *   szProfile;
    char *   szDomain;
  } tsZDDevice;

  typedef struct
  {
    uint16_t u16Id;
    char * szName;
  } tsZDCluster;

  typedef struct
  {
    uint16_t u16Id;
    char * szName;
  } tsZDAttributes;

  /*!
   * @brief Simple Descriptor Response Message
   */
  typedef struct
  {
    uint8_t    u8ClusterCount;
    uint16_t   au16Clusters[];
  }PACKED tsZDClusterList;

  typedef struct
  {
    uint8_t u8SequenceNo;
    uint8_t u8Status;
    uint16_t u16ShortAddress;
    uint8_t u8Length;
    uint8_t u8Endpoint;
    uint16_t u16ProfileID;
    uint16_t u16DeviceID;
    struct
    {
      uint8_t u8DeviceVersion :4;
      uint8_t u8Reserved :4;
    }PACKED sBitField;
    tsZDClusterList sInputClusters;
  }PACKED tsZDSimpleDescRsp;

  /*!
   * @brief Discover Attributes Request structure.
   * Uses short address mode.
   */
  typedef struct
  {
    uint8_t         u8AddressMode;
    uint16_t        u16ShortAddress;
    uint8_t         u8SourceEndPointId;
    uint8_t         u8DestinationEndPointId;
    uint16_t        u16ClusterId;
    uint16_t        u16AttributeId;
    uint8_t         bDirectionIsServerToClient;
    uint8_t         bIsManufacturerSpecific;
    uint16_t        u16ManufacturerCode;
    uint8_t         u8MaximumNumberOfIdentifiers;
  }PACKED tsZDAttrDiscReq;

  /*!
   * @brief Read Attributes Request structure.
   * Uses short address mode.
   */
  typedef struct
  {
    uint8_t         u8AddressMode;
    uint16_t        u16ShortAddress;
    uint8_t         u8SourceEndPointId;
    uint8_t         u8DestinationEndPointId;
    uint16_t        u16ClusterId;
    uint8_t         bDirectionIsServerToClient;
    uint8_t         bIsManufacturerSpecific;
    uint16_t        u16ManufacturerCode;
    uint8_t         u8NumberOfAttributes;
    uint16_t        au16AttributeList[MAX_NB_READ_ATTRIBUTES]; /* 10 is max number hard-coded in ZCB */
  }PACKED tsZDReadAttrReq;

  /*!
   * @brief Read Attributes Response structure.
   * Uses short address mode.
   */
  typedef struct
  {
    uint8_t   u8SequenceNumber;
    uint16_t  u16ShortAddress;
    uint8_t   u8SourceEndPointId;
    uint16_t  u16ClusterId;
    uint16_t  u16AttributeId;
    uint8_t   u8AttributeStatus;
    uint8_t   u8AttributeDataType;
    uint16_t  u16SizeOfAttributesInBytes;
    uint8_t   au8AttributeValue[];
  }PACKED tsZDReadAttrResp;

  /*!
   * @brief Active End Point Request structure.
   * Uses short address mode.
   */
  typedef struct
  {
    uint16_t        u16ShortAddress;
  }PACKED tsZDActiveEndPointReq;

  /*!
   * @brief Active End Point Response structure.
   */
  typedef struct
  {
    uint8_t   u8SequenceNumber;
    uint8_t   u8Status;
    uint16_t  u16ShortAddress;
    uint8_t   u8EndPointCount;
    uint8_t   au8EndPointList[];
  }PACKED tsZDActiveEndPointResp;

  tsZDDevice *
  ZDLookupDevice(
      uint16_t u16ProfileId,
      uint16_t u16DeviceId,
      uint8_t  u8DeviceVersion);

  void
  ZDSimpleDescResp(void *pvUser, uint16_t u16Length, void *pvMessage);
  void
  ZDAttrDiscReq(tsZDSimpleDescRsp *ptsSimpleDescRsp);
  void
  ZDAttrDiscResp(void *pvUser, uint16_t u16Length, void *pvMessage);
  void
  ZDReadAttrReq(
      uint16_t u16ShortAddress,
      uint8_t u8SrcEndpoint,
      uint8_t u8DestEndpoint,
      eZigbee_ClusterID u16ClusterID,
      uint8_t u8NbAttr,
      uint16_t* au16AttrList);

  void
  ZDReadAttrRsp(void *pvUser, uint16_t u16Length, void *pvMessage);
  void
  ZDActiveEndPointReq(uint16_t u16ShortAddress);
  void
  ZDActiveEndPointResp(void *pvUser, uint16_t u16Length, void *pvMessage);

#if defined __cplusplus
}
#endif

#endif  /* ZIGBEEDEVICES_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
