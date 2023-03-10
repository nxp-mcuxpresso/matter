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

First instructions from [README.md 'Building section'][readme_building_section] should be followed.

[readme_building_section]: README.md#building

Note : To enable the [matter CLI](README.md#matter-shell), the argument ```chip_enable_matter_cli=true``` could be added to the *gn gen* command.

The argument ```is_debug=true optimize_debug=false``` could be used to build the application in debug mode.

-   Build Matter-over-Wifi configuration with BLE commissioning (ble-wifi):

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1170$ gn gen --args="chip_enable_wifi=true iwx12_transceiver=true chip_config_network_layer_ble=true chip_enable_ble=true  is_<sdk_type>=true " out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1170$ ninja -C out/debug
```

-   Build Matter-over-Wifi configuration with onnetwork commissioning (without BLE, the WiFi network credentials are provided at build-time which will enable the device to join the network at the startup) :

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1170$ gn gen --args="chip_enable_wifi=true iwx12_transceiver=true is_<sdk_type>=true chip_config_network_layer_ble=false tcp_download=true wifi_ssid=\"your_wifi_ssid\" wifi_password=\"your_wifi_password\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1170$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rt1170-all-cluster-example

<a name="flashdebug"></a>

## Flashing and debugging the RT1170

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#Flashing-and-debugging

## Testing the example

Follow instructions from [README.md 'Testing the example'][readme_test_example_section].

[readme_test_example_section]:README.md#testing-the-example

To commision the device over BLE, follow the instructions from [chip-tool's README.md 'Commission a device over BLE'][readme_ble_commissioning_section].

[readme_ble_commissioning_section]:../../../../chip-tool/README.md#commission-a-device-over-ble

### Known issues/limitations

- If the Matter commissioning failed for some reasons, it is recommended to always either reflash the RT1170 with a new `All-clusters application` binary, or use the ```matterfactoryreset``` command if the shell is enabled, before starting a new commissioning. This would allow to erase all previously saved settings.