# SRP server feature for Matter over Openthread

## Enabling the SRP server
1. In *connectedhomeip/third_party/openthread/ot-nxp/openthread/src/core/config/srp_server.h* set OPENTHREAD_CONFIG_SRP_SERVER_ENABLE to 1
2. At application level, define the IHD_SRP_SERVER macro and set it to 1
3. At application level, use the following API to enable the SRP server:
```
 // otSrpClientStop() also disables the auto-start mode
 otSrpClientStop(chip::DeviceLayer::ThreadStackMgrImpl().OTInstance());

 ChipLogProgress(DeviceLayer, "Enabling SRP Server!");
 otSrpServerSetEnabled(chip::DeviceLayer::ThreadStackMgrImpl().OTInstance(), true);
```

To customize the SRP server's behaviour when a host issues an update, implement the *otSrpServerServiceUpdateHandler()* function. The behaviour of this handler is described in the *connectedhomeip/third_party/openthread/ot-nxp/openthread/include/openthread/srp_server.h* header.
You can start from the *_SrpServerHandler()* implementation in the *connectedhomeip/src/platform/OpenThread/GenericThreadStackManagerImpl_OpenThread.cpp* module.

### Known issues/limitations
- With the SRP server enabled on the i.mxrt1060, commissioning of a new node fails, even though the node registers itself with the SRP server.