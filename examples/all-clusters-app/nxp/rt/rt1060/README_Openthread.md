# RT1060 All-cluster Application for Matter over Openthread

## Hardware requirements

Host part:

- 1 MIMXRT1060-EVKB or EVK-MIMXRT1060 board

Transceiver part:

- 1 OM15076-3 Carrier Board (DK6 board)
- 1 K32W0 Module to be plugged on the Carrier Board


## Board settings

Board settings are described [here][ot_cli_rt1060_readme].

[ot_cli_rt1060_readme]:https://github.com/NXP/ot-nxp/blob/v1.0.0.2-tag-nxp/src/imx_rt/rt1060/README.md#board-settings-for-mimxrt1060-evkb

<a name="building"></a>

## Building

### Pre-build instructions
First instructions from [README.md 'Building section'][readme_building_section] should be followed.

[readme_building_section]: README.md#building

### Build instructions

### Build the Openthread configuration with BLE commissioning.

For this configuration a K32W0 RCP image is required and must support in a single image the openthread RCP configuration and the BLE HCI BB configuration. Messages between the host and the K32W0 transceiver are transfered on a single UART with flow control support.
For that the HDLC-Lite framing protocol is used to transfert spinel and hci frames. In addition, hci and spinel frames can be distinguished by using the Spinel convention which is line compatible with BT/BLE HCI.

Before building the Matter host application, it is required to generate the K32W0 image supporting features as described above. To build this binary the target ````ot_rcp_ble_hci_bb_single_uart_fc```` should be built by following the [Readme.md][ot_rcp_ble_hci_bb_k32w0_readme]. After a successfull build, a ````".h"```` file will be generated and would contain the K32W0 RCP binary. As described in the [Readme.md][ot_rcp_ble_hci_bb_k32w0_readme], the application binaries will be generated in `ot_nxp/build_k32w061/ot_rcp_ble_hci_bb_single_uart_fc/bin/ot-rcp-ble-hci-bb-k32w061.elf.bin.h`.

The generate K32W0 transceiver binary ````".h"```` file path must be indicated to the host Matter application build. In fact the Matter host application is in charge of storing the K32W0 firmware in its flash to be able to use the ````The Over The Wire (OTW) protocol (over UART)```` to download (at host startup) the k32w0 transceiver image from the host to the K32W0 internal flash.  For more information on the k32w0 OTW protocol, user can consult the doxygen header of [fwk_otw.c][fwk_otw_sdk_path].

Here is a summary of the k32w0 *gn gen* arguments that are mandatory or optional:
- Mandatory: ````k32w0_transceiver=true````
- Mandatory: ````hci_spinel_single_uart=true````
- Optional: ````k32w0_transceiver_bin_path=\"/home/ot-nxp/build_k32w061/ot_rcp_ble_hci_bb_single_uart_fc/bin/ot-rcp-ble-hci-bb-k32w061.elf.bin.h\"```` This argument is optional, by default, if not set, the binary file located in "${chip_root}/third_party/openthread/ot_nxp/build_k32w061/ot_rcp_ble_hci_bb_single_uart_fc/bin/ot-rcp-ble-hci-bb-k32w061.elf.bin.h" will be used. If the K32W061 transceiver binary is saved at another location an absolute path of its location should be given.

[fwk_otw_sdk_path]:../../../../../third_party/nxp/rt_sdk/repo/middleware/wireless/framework/OTW/k32w0_transceiver/fwk_otw.c

[ot_rcp_ble_hci_bb_k32w0_readme]:https://github.com/NXP/ot-nxp/blob/v1.0.0.2-tag-nxp/examples/hybrid/ot_rcp_ble_hci_bb/k32w061/README.md#building-the-examples

Below is presented an example of *gn gen* argument that could be used to generate the host matter application with a k32w0 transceiver.

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_openthread=true k32w0_transceiver=true k32w0_transceiver_bin_path=\"/home/ot-nxp/build_k32w061/ot_rcp_ble_hci_bb_single_uart_fc/bin/ot-rcp-ble-hci-bb-k32w061.elf.bin.h\" hci_spinel_single_uart=true chip_inet_config_enable_ipv4=false chip_config_network_layer_ble=true" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rt1060-all-cluster-example

## Flash Binaries

### Flashing and debugging the RT1060

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashdebug

## Raspberrypi Border Router setup

Instructions to start an openthread border router should be followed. In this section a mechanism to start the BR, without accessing the web interface, is described.

Start the docker BR image:

```
sudo docker run -it --rm --network host --privileged -v /dev/ttyACM0:/dev/radio connectedhomeip/otbr:sve2 --radio-url spinel+hdlc+uart:///dev/radio -B eth0
```

Get the docker ID of the BR image:
```
sudo docker container ls
```

Create a thread network on the border router (44bd463c5ba9 should be replaced by the previously gotten BR docker ID):

```
sudo docker exec -it 44bd463c5ba9 sh -c "sudo ot-ctl dataset init new"; sudo docker exec -it 44bd463c5ba9 sh -c "sudo ot-ctl dataset channel 17"; sudo docker exec -it 44bd463c5ba9 sh -c "sudo ot-ctl dataset panid 0x12aa"; sudo docker exec -it 44bd463c5ba9 sh -c "sudo ot-ctl dataset extpanid 1111111122222222"; sudo docker exec -it 44bd463c5ba9 sh -c "sudo ot-ctl dataset networkkey 00112233445566778899aabbccaaaaaa"; sudo docker exec -it 44bd463c5ba9 sh -c "sudo ot-ctl dataset commit active"; sudo docker exec -it 44bd463c5ba9 sh -c "sudo ot-ctl ifconfig up"; sudo docker exec -it 44bd463c5ba9 sh -c "sudo ot-ctl thread start"; sudo docker exec -it 44bd463c5ba9 sh -c "sudo ot-ctl prefix add fd11:22::/64 pasor"; sudo docker exec -it 44bd463c5ba9 sh -c "sudo ot-ctl netdata register"
```

## Testing the all cluster app example (with BLE commissioning support) - default configuration

The pairing "ble-thread" feature must be used and instructions from [README.md 'Testing the example'][readme_test_example_section] should be followed.

[readme_test_example_section]:README.md#testing-the-example

## Testing the all cluster app example (without BLE commissioning support) - only for testing purpose

For such test, having the Matter CLI is mandatory, instructions from [README.md 'Testing the all-clusters application with Matter CLI enabled'][readme_test_with_matter_cli_section] should be followed.

[readme_test_with_matter_cli_section]:README.md#testing-the-all-clusters-application-with-matter-cli-enabled

Then using the Matter CLI below commands to join an existing thread network should be enterred, networkey and panid should be changed depending on thread network configurations:

```
otcli dataset networkkey 00112233445566778899aabbccddeeff
otcli dataset panid 0x1234
otcli dataset commit active
otcli ifconfig up
otcli thread start
```

Note: The pairing "onnetwork" feature should be used as the pairing/commissioning method.
