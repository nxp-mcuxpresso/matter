# RW610 All-Cluster Application for Matter over Openthread

## Hardware requirements

- RD-RW610 board
- BLE/15.4 antenna

<a name="building"></a>

## Building

### Pre-build instructions
First instructions from [README.md 'Building section'][readme_building_section] should be followed.

Make sure to update ot-nxp submodules if not already done:

```
user@ubuntu: cd ~/Desktop/git/connectedhomeip/third_party/openthread/ot-nxp
user@ubuntu: git submodule update
```

[readme_building_section]: README.md#building

### Build instructions

Note : Building with SDK package and SDK internal are both supported, for this replace ```is_<sdk_type>``` with ```is_sdk_package``` if building with SDK package, or with ```is_sdk_internal``` if internal SDK is used instead.

-   Build the Openthread configuration with BLE commissioning. The argument ```is_debug=true optimize_debug=false``` could be used to build the application in debug mode. To enable the [matter CLI](README.md#matter-shell), the argument ```chip_enable_matter_cli=true``` could be added.

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ gn gen --args="chip_enable_openthread=true chip_inet_config_enable_ipv4=false chip_config_network_layer_ble=true is_<sdk_type>=true" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ ninja -C out/debug
```

-   Build the Openthread configuration (without BLE, matter-cli enabled to join an existing thread network, **not the standard way, only for test purpose**) - argument is_debug=true optimize_debug=false could be used to build the application in debug mode.

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ gn gen --args="chip_enable_openthread=true chip_inet_config_enable_ipv4=false chip_enable_matter_cli=true chip_config_network_layer_ble=false is_<sdk_type>=true" out/debug 
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rw610-all-cluster-example.

## Flash Binaries
### Flashing and debugging RW610

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

Create a thread network on the border router (<container_id> should be replaced by the previously gotten BR docker ID):

```
sudo docker exec -it <container_id> sh -c "sudo ot-ctl dataset init new"; sudo docker exec -it <container_id> sh -c "sudo ot-ctl dataset channel 17"; sudo docker exec -it <container_id> sh -c "sudo ot-ctl dataset panid 0x1222"; sudo docker exec -it <container_id> sh -c "sudo ot-ctl dataset extpanid 1111111122222222"; sudo docker exec -it <container_id> sh -c "sudo ot-ctl dataset networkkey 00112233445566778899aabbccddeeaa"; sudo docker exec -it <container_id> sh -c "sudo ot-ctl dataset commit active"; sudo docker exec -it <container_id> sh -c "sudo ot-ctl ifconfig up"; sudo docker exec -it <container_id> sh -c "sudo ot-ctl thread start"; sudo docker exec -it <container_id> sh -c "sudo ot-ctl prefix add fd11:22::/64 pasor"; sudo docker exec -it <container_id> sh -c "sudo ot-ctl netdata register"
```

Get the dataset active of the thread network created :
```
sudo docker exec -it <container_id> sh -c "sudo ot-ctl dataset active -x"
```


## Testing the all custer app example (with BLE commissioning support)
1. Prepare the board with the flashed `All-clusters application` supporting Openthread and BLE.
2. The All-cluster example uses UART (FlexComm3) to print logs while runing the server. To view logs, start a terminal emulator like PuTTY and connect to the used COM port with the following UART settings:

   - Baud rate: 115200
   - 8 data bits
   - 1 stop bit
   - No parity
   - No flow control

3. Once flashed, BLE advertising will be started automatically.

4. On the BR, start sending commands using the [chip-tool](../../../../../examples/chip-tool)  application as it is described [here](../../../../../examples/chip-tool/README.md#using-the-client-to-send-matter-commands). The pairing "ble-thread" feature should be used and is described [here](../../../../../examples/chip-tool/README.md#Using-the-Client-to-commission-a-device).

## Testing the all custers app example (without BLE commissioning support) - only for testing purpose
1. Prepare the board with the flashed `All-clusters application` supporting Openthread only.
2. The matter CLI is accessible in UART1 (FlexComm3). For that, start a terminal emulator like PuTTY and connect to the used COM port with the following UART settings:

   - Baud rate: 115200
   - 8 data bits
   - 1 stop bit
   - No parity
   - No flow control
2. The All-cluster example uses UART2 (FlexComm0) to print logs while runing the server. To view raw UART output, a pin should be plugged to an USB to UART adapter (connector HD2 pin 03), then start a terminal emulator like PuTTY and connect to the used COM port with the following UART settings:

   - Baud rate: 115200
   - 8 data bits
   - 1 stop bit
   - No parity
   - No flow control

3. On the matter CLI enter the below commands:

```
otcli networkkey 00112233445566778899aabbccddeeaa
otcli panid 0x1222
otcli channel 17
otcli dataset commit active
otcli ifconfig up
otcli thread start
```

4. On the BR, start sending commands using the [chip-tool](../../../../../examples/chip-tool)  application as it is described [here](../../../../../examples/chip-tool/README.md#using-the-client-to-send-matter-commands). The pairing "onnetwork" feature should be used as the pairing/commissioning over BLE is not supported in this version.

### Chip-tool commands shortcut
- Matter-over-Thread commissioning with BLE :
```
user@ubuntu:~/apps$ ./chip-tool pairing ble-thread ${NODE_ID_TO_ASSIGN} hex:<active_dataset> 20202021 3840
```
- Matter-over-Thread commissioning onnetwork (for testing purpose only) :
```
user@ubuntu:~/apps$ ./chip-tool pairing onnetwork ${NODE_ID_TO_ASSIGN} 20202021
```
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

- If the Matter commissioning failed for some reasons, it is recommended to always either reflash the RW610 with a new `All-clusters application` binary, or use the ```matterfactoryreset``` command if the shell is enabled, before starting a new commissioning. This would allow to erase all previously saved settings.
