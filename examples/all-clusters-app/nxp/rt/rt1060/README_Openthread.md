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

### Build the Openthread configuration with BLE commissioning.

Argument evkname=\"evkmimxrt1060\" must be used in *gn gen* command when building for EVK-MIMXRT1060 board instead of the default MIMXRT1060-EVKB. Also argument is_debug=true optimize_debug=false could be used to build the application in debug mode. For this configuration a K32W061 image supporting HCI and spinel on a single UART should be used.

To build with the option to have Matter certificates/keys pre-loaded in a specific flash area the option chip_with_factory_data=1 should be added (for more information see [Guide for writing manufacturing data on NXP devices](../../../../platform/nxp/doc/manufacturing_flow.md).

 To enable the [matter CLI](README.md#matter-shell), the argument ```chip_enable_matter_cli=true``` could be added.

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_openthread=true k32w0_transceiver=true k32w0_transceiver_bin_path=\"/home/ot-nxp/build_k32w061/ot_rcp_ble_hci_bb_single_uart_fc/bin/ot-rcp-ble-hci-bb-k32w061.elf.bin.h\" hci_spinel_single_uart=true chip_inet_config_enable_ipv4=false chip_config_network_layer_ble=true" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

### Build the Openthread configuration (without BLE)
In this configuration the matter-cli must be enabled to join an existing thread network, **not the standard way, only for test purpose**) - argument is_debug=true optimize_debug=false could be used to build the application in debug mode. Argument evkname=\"evkmimxrt1060\" must be used in *gn gen* command when building for EVK-MIMXRT1060 board instead of the default MIMXRT1060-EVKB.

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_openthread=true k32w0_transceiver=true k32w0_transceiver_bin_path=\"/home/ot-nxp/build_k32w061/ot_rcp_ble_hci_bb_single_uart_fc/bin/ot-rcp-ble-hci-bb-k32w061.elf.bin.h\" chip_inet_config_enable_ipv4=false chip_enable_matter_cli=true chip_config_network_layer_ble=false" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

Note : 
- If the K32W061 transceiver is selected, by default if the gn option k32w0_transceiver_bin_path is not set, the binary file located in "${chip_root}/third_party/openthread/ot_nxp/build_k32w061/ot_rcp_ble_hci_bb_single_uart_fc/bin/ot-rcp-ble-hci-bb-k32w061.elf.bin.h" will be used. If the K32W061 transceiver binary is saved at another location an absolute path of its location should be given in the k32w0_transceiver_bin_path gn option (as shown above in the examples).

The resulting output file can be found in out/debug/chip-rt1060-all-cluster-example

## Flash Binaries

### K32W061 transceiver image

A dedicated k32w0 transceiver binary should be built. This binary must support OT+BLE and must support spinel and hci on a single UART. To build this binary the [Readme.md][ot_rcp_ble_hci_bb_k32w0_readme] should be followed.

The Over The Wire (OTW) protocol (over UART) would be used to download the k32w0 transceiver image from the RT1060 to the its internal flash.

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
otcli dataset networkkey 00112233445566778899aabbccddeeff
otcli dataset panid 0x1234
otcli dataset commit active
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
