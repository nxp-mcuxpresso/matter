# RW610 All-cluster Application for Matter over Wi-Fi

## Hardware requirements

- RD-RW610-BGA board
- Wi-Fi antenna (to plug in Ant1) + BLE antenna (to plug in Ant2)

<a name="building"></a>

## Building

First instructions from [README.md 'Building section'][readme_building_section] should be followed.

[readme_building_section]: README.md#building

Note : Building with SDK package and SDK internal are both supported, for this replace ```is_<sdk_type>``` with ```is_sdk_package``` if building with SDK package, or with ```is_sdk_internal``` if internal SDK is used instead. The argument ```is_debug=true optimize_debug=false``` could be used to build the application in debug mode. 

To enable the [matter CLI](README.md#matter-shell), the argument ```chip_enable_matter_cli=true``` could be added.

- Build Matter-over-Wifi configuration with BLE commissioning (ble-wifi) :

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ gn gen --args="chip_enable_wifi=true is_<sdk_type>=true" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ ninja -C out/debug
```
- Build Matter-over-Wifi configuration with onnetwork commissioning (without BLE, the WiFi network credentials are provided at build-time which will enable the device to join the network at the startup) :
```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ gn gen --args="chip_enable_wifi=true is_<sdk_type>=true chip_config_network_layer_ble=false tcp_download=true wifi_ssid=\"your_wifi_ssid\" wifi_password=\"your_wifi_password\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rw610-all-cluster-example.

<a name="flashdebug"></a>

## Flashing and debugging

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashdebug

## Testing the example

Follow instructions from [README.md 'Testing the example'][readme_test_example_section].

[readme_test_example_section]:README.md#testing-the-example

- To commision the device over BLE using chip-tool, run the command below. This will enable chip-tool to discover the device over BLE with the discriminator number ("3840") and provision the device into the network :
```
user@ubuntu:~/apps$ ./chip-tool pairing ble-wifi ${NODE_ID_TO_ASSIGN} ${SSID} ${PASSWORD} 20202021 3840
```
The ```${SSID}``` and ```${PASSWORD}``` should be replaced with the Wi-Fi SSID and password.

- To commission the device onnetwork using chip-tool, run the command below. This will discover devices on the network and try to pair with the one matching the provided setup code ("20202021") : 
```
user@ubuntu:~/apps$ ./chip-tool pairing onnetwork ${NODE_ID_TO_ASSIGN} 20202021
```
The ${NODE_ID_TO_ASSIGN} is a decimal number (or a 0x-prefixed hex number) that would be assigned to the endpoint device by chip-tool.

### Known issues/limitations

If the Matter commissioning failed for some reasons, it is recommended to always either reflash the RW610 with a new `All-clusters application` binary, or use the ```matterfactoryreset``` command if the shell is enabled, before starting a new commissioning. This would allow to erase all previously saved settings.
