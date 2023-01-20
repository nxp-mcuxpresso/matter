# RT1060 All-cluster Application for Matter over Ethernet

<a name="building"></a>

## Building

First instructions from [README.md 'Building section'][readme_building_section] should be followed.

[readme_building_section]: README.md#building

-   Build the ethernet configuration for MIMXRT1060-EVKB board:

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_ethernet=true chip_config_network_layer_ble=false chip_inet_config_enable_ipv4=false" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

-   Build the ethernet configuration for EVK-MIMXRT1060 board:

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_ethernet=true chip_config_network_layer_ble=false chip_inet_config_enable_ipv4=false evkname=\"evkmimxrt1060\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rt1060-all-cluster-example

<a name="flashdebug"></a>

## Flashing and debugging

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashdebug

## Application UI

To interact with the server running on the RT1060 we recomand using the [chip-tool](https://github.com/project-chip/connectedhomeip/tree/master/examples/chip-tool) application. For this setup you will need a Raspberry PI model 4 or later that has a Linux Ubuntu Server 20.04 distrubution runnig on it. To prepare the board please follow this [ubuntu tutorial](https://ubuntu.com/tutorials/how-to-install-ubuntu-on-your-raspberry-pi) on how to install ubuntu on a raspberry pi. 
After you followed the the steps in the guide and prepared the chip-tool go into the build directory and use this command for pairing:

```
./chip-tool pairing onnetwork 1234 20202021, where 20202021 is the SetUpPinCode and 1234 is the NodeID 
```

## Testing the example

Follow instructions from [README.md 'Testing the example'][readme_test_example_section].

[readme_test_example_section]:README.md#testing-the-example
