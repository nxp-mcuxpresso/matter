# Matter over Wi-Fi on RT1170 + transceiver

## Configuration(s) supported

Here are listed configurations that allow to support Matter over Wi-Fi on RT1170:

- RT1170 + IWX12 (Wi-Fi + BLE + 15.4)

## Hardware requirements for RT1170 + IWX12

To know hardware and board settings for such a configuration follow instructions form  [README.md 'Hardware requirements for RT1170 + IWX12'][readme_rt1170_iwx12_hardware].

[readme_rt1170_iwx12_hardware]:README.md#hardware-requirements-for-rt1170-and-iwx12

## Building

First instructions from [README.md 'Building section'][readme_building_section] should be followed.

[readme_building_section]: README.md#building

-   Build the Wi-fi configuration for MIMXRT1170 board + IWX12 transceiver (with BLE for commissioning).

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1170$ gn gen --args="chip_enable_wifi=true iwx12_transceiver=true chip_config_network_layer_ble=true chip_enable_ble=true " out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1170$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rt1170-all-cluster-example

## Flashing and debugging the RT1170

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashing-and-debugging

## Testing the example

Follow instructions from [README.md 'Testing the example'][readme_test_example_section].

[readme_test_example_section]:README.md#testing-the-example