# Matter over Wi-Fi on RW612

## Hardware requirements

- RD-RW612-BGA board
- Wi-Fi antenna (to plug in Ant1) + BLE antenna (to plug in Ant2)

<a name="building"></a>

## Building

First instructions from [README.md 'Building section'][readme_building_section] should be followed.

[readme_building_section]: README.md#building

- Build Matter-over-Wifi configuration with BLE commissioning (ble-wifi) :

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ gn gen --args="chip_enable_wifi=true is_sdk_package=true" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ ninja -C out/debug
```
- Build Matter-over-Wifi configuration with onnetwork commissioning (without BLE, the WiFi network credentials are provided at build-time which will enable the device to join the network at the startup) :
```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ gn gen --args="chip_enable_wifi=true is_sdk_package=true chip_config_network_layer_ble=false tcp_download=true wifi_ssid=\"your_wifi_ssid\" wifi_password=\"your_wifi_password\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rw610-all-cluster-example.

<a name="flashdebug"></a>

## Flashing and debugging

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashing-and-debugging

## Testing the example

Follow instructions from [README.md 'Testing the example'][readme_test_example_section].

[readme_test_example_section]:README.md#testing-the-example
