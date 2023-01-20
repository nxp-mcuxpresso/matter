# RT1170 All-cluster Application for Matter over Openthread

## Hardware requirements

Host part:

- 1 MIMXRT1170-EVK board

Transceiver part:

- 1 IW612 module


## Board settings

Board settings are described [here][ot_cli_rt1170_readme].

[ot_cli_rt1170_readme]:../../../../../third_party/openthread/ot-nxp/src/imx_rt/rt1170/README.md#Board-settings

<a name="building"></a>

## Building

### Build instructions

First instructions from [README.md 'Building section'][readme_building_section] should be followed.


[readme_building_section]: README.md#Building



-   Build the Openthread configuration with BLE commissioning.

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-cluster/nxp/rt/rt1170$ gn gen --args="chip_enable_openthread=true iwx12_transceiver=true chip_inet_config_enable_ipv4=false chip_config_network_layer_ble=true is_sdk_package=true is_debug=true optimize_debug=false" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-cluster/nxp/rt/rt1170/$ ninja -C out/debug
```
Use is_sdk_package=true as above if SDK is a package downloaded.
If SDK is retrieved from NXP internal repository, use is_sdk_internal=true instead.

To enable the [matter CLI](README.md#matter-shell), the argument ```chip_enable_matter_cli=true``` could be added.
  
The resulting output file can be found in out/debug/chip-rt1170-all-cluster-example

## Flashing and debugging the RT1170

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#Flashing-and-debugging

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



# Testing the all custer app example (with BLE commissioning support)
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

- If the Matter commissioning failed for some reasons, it is recommended to always either reflash the RT1170 with a new `All-clusters application` binary, or use the ```matterfactoryreset``` command if the shell is enabled, before starting a new commissioning. This would allow to erase all previously saved settings.