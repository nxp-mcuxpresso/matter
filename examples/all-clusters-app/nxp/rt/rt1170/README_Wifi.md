# RT1170 All-cluster Application for Matter over Openthread

## Hardware requirements

Host part:

- 1 MIMXRT1170-EVK board

Transceiver part:

- 1 IW612 module


## Board settings

Board settings are described [here][ot_cli_rt1170_readme].

[ot_cli_rt1170_readme]:https://github.com/NXP/ot-nxp/blob/v1.0.0.2-tag-nxp/src/imx_rt/rt1170/README.md#Board-settings

<a name="building"></a>

## Building

First instructions from [README.md 'Building section'][readme_building_section] should be followed.

[readme_building_section]: README.md#building

-   Build Matter-over-Wifi configuration with BLE commissioning (ble-wifi):

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1170$ gn gen --args="chip_enable_wifi=true iwx12_transceiver=true chip_config_network_layer_ble=true chip_enable_ble=true is_sdk_package=true " out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1170$ ninja -C out/debug
```

-   Build Matter-over-Wifi configuration with onnetwork commissioning (without BLE, the WiFi network credentials are provided at build-time which will enable the device to join the network at the startup) :

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1170$ gn gen --args="chip_enable_wifi=true iwx12_transceiver=true is_sdk_package=true chip_config_network_layer_ble=false tcp_download=true wifi_ssid=\"your_wifi_ssid\" wifi_password=\"your_wifi_password\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1170$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rt1170-all-cluster-example

<a name="flashdebug"></a>

## Flashing and debugging the RT1170

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashing-and-debugging

## Testing the example

Follow instructions from [README.md 'Testing the example'][readme_test_example_section].

[readme_test_example_section]:README.md#testing-the-example