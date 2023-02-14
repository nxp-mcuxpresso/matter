# RT1060 All-cluster Application for Matter over Openthread

## Hardware requirements

Host part:

- 1 MIMXRT1060-EVKB or EVK-MIMXRT1060 board

Transceiver part:

- 1 OM15076-3 Carrier Board (DK6 board)
- 1 K32W061 Module to be plugged on the Carrier Board


## Board settings

Board settings are described [here][ot_cli_rt1060_readme].

[ot_cli_rt1060_readme]:https://github.com/NXP/ot-nxp/blob/v1.0-branch-nxp/src/imx_rt/rt1060/README.md#board-settings-for-mimxrt1060-evkb

<a name="building"></a>

## Building

### Pre-build instructions
First instructions from [README.md 'Building section'][readme_building_section] should be followed.

Make sure to update ot_nxp submodules if not already done:

```
user@ubuntu: cd ~/Desktop/git/connectedhomeip/third_party/openthread/ot-nxp
user@ubuntu: git submodule update --init
```

[readme_building_section]: README.md#building

### Build instructions

-   Build the Openthread configuration with BLE commissioning. Argument evkname=\"evkmimxrt1060\" must be used in *gn gen* command when building for EVK-MIMXRT1060 board instead of the default MIMXRT1060-EVKB. Also argument is_debug=true optimize_debug=false could be used to build the application in debug mode. For this configuration a K32W061 image supporting HCI and spinel on a single UART should be used. To build with the option to have Matter certificates/keys pre-loaded in a specific flash area the option chip_with_factory_data=1 should be added (for more information see [Guide for writing manufacturing data on NXP devices](../../../../platform/nxp/doc/manufacturing_flow.md). To enable the [matter CLI](README.md#matter-shell), the argument ```chip_enable_matter_cli=true``` could be added.

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_openthread=true k32w0_transceiver=true chip_inet_config_enable_ipv4=false chip_config_network_layer_ble=true hci_spinel_single_uart=true out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

-   Build the Openthread configuration (without BLE, matter-cli enabled to join an existing thread network, **not the standard way, only for test purpose**) - argument is_debug=true optimize_debug=false could be used to build the application in debug mode. Argument evkname=\"evkmimxrt1060\" must be used in *gn gen* command when building for EVK-MIMXRT1060 board instead of the default MIMXRT1060-EVKB.

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_openthread=true k32w0_transceiver=true chip_inet_config_enable_ipv4=false chip_enable_matter_cli=true chip_config_network_layer_ble=false" out/debug 
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

Note : 
- To build the application with OTW enabled that would allow to flash the K32W0 transceiver image at boot, argument "enable_otw_k32w0=true" must be added to the *gn gen* command from above.
- To build the application with OTA Requestor functionality enabled, argument "chip_enable_ota_requestor=true" must be added to the *gn gen* command from above. 

The resulting output file can be found in out/debug/chip-rt1060-all-cluster-example

## Flash Binaries

### Flashing a K32W061 transceiver image

A dedicated k32w0 transceiver binary is delivered. The binary can be flashed using OTW and therefore the argument "enable_otw_k32w0=true" must be added to the *gn gen* command from above. This binary supports OT and BLE. It allows to support spinel and hci on a single UART. 

In a next release, the application code to rebuild the K32W0 transceiver image would be delivered in ot_nxp. In this case the binary called “ot-rcp-ble-hci-bb-k32w061.bin” would be the result of a build in ot-nxp for the target "ot_rcp_ble_hci_bb_single_uart". To have more information to build such a configuration the following [Readme.md][ot_rcp_ble_hci_bb_k32w0_readme]. Then the generated binary will need to be converted into a header file using the **xxd** command as below:

```
user@ubuntu:~/<location of the binary>/$ xxd -i rcp_name.bin > rcp_name.bin.h
```

Then the **ot-rcp-fw-placeholder.bin.h** file will need to be replaced with the generated header file from above in the following directory:

```
~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060/otw_uploader/include/
```

And, as a last step, the include of the **ot-rcp-fw-placeholder.bin.h** in **ISPInterface.cpp** will need to be changed to the newly added header file and the following line edited acordingly with the new name of the char string storing the firmware, which is located in the newly generated header file.

```
isp.eBL_ProgramFirmware((uint8_t *)ot_rcp_fw_placeholder_bin, sizeof(ot_rcp_fw_placeholder_bin));
                                                 |
                                                 V
isp.eBL_ProgramFirmware((uint8_t *)rcp_name_bin, sizeof(rcp_name_bin));
```

[ot_rcp_ble_hci_bb_k32w0_readme]:https://github.com/NXP/ot-nxp/blob/v1.0-branch-nxp/examples/hybrid/ot_rcp_ble_hci_bb/k32w061/README.md

### Flashing and debugging the RT1060

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashdebug

## Raspberrypi Test harness setup

Instructions to start an openthread border router should be followed. In this section a mechanism to start the BR, without accessing the web interface, is described.

Start the docker BR image:

```
sudo docker run -it --rm --network host --privileged -v /dev/ttyACM0:/dev/radio connectedhomeip/otbr:sve2 --radio-url spinel+hdlc+uart:///dev/radio -B eth0
```

Get the docker ID of the BR image:
```
sudo docker container ls
```

Create a thread network on the border router (048bf89bb3dd should be replaced by the previously gotten BR docker ID):

```
sudo docker exec -it 048bf89bb3dd sh -c "sudo ot-ctl dataset init new"; sudo docker exec -it 048bf89bb3dd sh -c "sudo ot-ctl dataset channel 17"; sudo docker exec -it 048bf89bb3dd sh -c "sudo ot-ctl dataset panid 0x1222"; sudo docker exec -it 048bf89bb3dd sh -c "sudo ot-ctl dataset extpanid 1111111122222222"; sudo docker exec -it 048bf89bb3dd sh -c "sudo ot-ctl dataset networkkey 00112233445566778899aabbccddeeaa"; sudo docker exec -it 048bf89bb3dd sh -c "sudo ot-ctl dataset commit active"; sudo docker exec -it 048bf89bb3dd sh -c "sudo ot-ctl ifconfig up"; sudo docker exec -it 048bf89bb3dd sh -c "sudo ot-ctl thread start"; sudo docker exec -it 048bf89bb3dd sh -c "sudo ot-ctl prefix add fd11:22::/64 pasor"; sudo docker exec -it 048bf89bb3dd sh -c "sudo ot-ctl netdata register"
```



## Testing the all custer app example (with BLE commissioning support)
1. Prepare the board with the flashed `All-cluster application` supporting Openthread and BLE.
2. The All-cluster example uses UART1 to print logs while runing the server. To view logs, start a terminal emulator like PuTTY and connect to the used COM port with the following UART settings:

   - Baud rate: 115200
   - 8 data bits
   - 1 stop bit
   - No parity
   - No flow control

3. Once flashed, BLE advertising will be started automatically.

4. On the BR, start sending commands using the [chip-tool](../../../../../examples/chip-tool)  application as it is described [here](../../../../../examples/chip-tool/README.md#using-the-client-to-send-matter-commands). The pairing "ble-thread" feature should be used and is described [here](../../../../../examples/chip-tool/README.md#Using-the-Client-to-commission-a-device).
## Testing the all custer app example (without BLE commissioning support) - only for testing purpose
1. Prepare the board with the flashed `All-cluster application` supporting Openthread only.
2. The matter CLI is accessible in UART1. For that, start a terminal emulator like PuTTY and connect to the used COM port with the following UART settings:

   - Baud rate: 115200
   - 8 data bits
   - 1 stop bit
   - No parity
   - No flow control
2. The All-cluster example uses UART2 to print logs while runing the server. To view raw UART output, a pin should be plugged to an USB to UART adapter (connector J16 pin 7 in case of MIMXRT1060-EVKB board or connector J22 pin 7 in case of EVK-MIMXRT1060 board), then start a terminal emulator like PuTTY and connect to the used COM port with the following UART settings:

   - Baud rate: 115200
   - 8 data bits
   - 1 stop bit
   - No parity
   - No flow control

3. On the matter CLI enter the below commands:

```
otcli networkkey 00112233445566778899aabbccddeeff
otcli panid 0x1234
otcli commit active
otcli ifconfig up
otcli thread start
```

4. On the BR, start sending commands using the [chip-tool](../../../../../examples/chip-tool)  application as it is described [here](../../../../../examples/chip-tool/README.md#using-the-client-to-send-matter-commands). The pairing "onnetwork" feature should be used as the pairing/commissioning over BLE is not supported in this version.

## Matter Commissioning recommendations

Before starting a commissioning stage it is recommended to run the following commands on the Border Router and to remove files located in /tmp/chip_*: 

1. Get the "CONTAINER ID"
```
sudo docker container ls
```
2. Disable SRP server
```
sudo docker exec -it <container_id> sh -c "sudo ot-ctl srp server disable"
```
3. Enable SRP server
```
sudo docker exec -it <container_id> sh -c "sudo ot-ctl srp server enable"
```

### Known issues/limitations

- If the Matter commissioning failed for some reasons, it is recommended to always either reflash the RT1060 with a new `All-clusters application` binary, or use the ```matterfactoryreset``` command if the shell is enabled, before starting a new commissioning. This would allow to erase all previously saved settings.
