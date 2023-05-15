# Matter crypto unit tests on RT1060

This directory contains test driver for the NXP RT1060 platform.

The application is used to test crypto module.

## Build crypto unit tests

Build the runner using gn:

```
cd <connectedhomeip>/src/test_driver/nxp/rt/rt1060
gn gen --args="chip_config_network_layer_ble=false chip_enable_ethernet=true chip_inet_config_enable_ipv4=false chip_build_tests=true chip_build_test_static_libraries=false" out/debug
ninja -C out/debug
```

Argument is_debug=true optimize_debug=false could be used to build the
application in debug mode. Argument evkname="evkmimxrt1060" must be used in gn
gen command when building for EVK-MIMXRT1060 board instead of the default
MIMXRT1060-EVKB. Argument chip_build_test_static_libraries=false is mandatory in
order to link unit test file during the build.

## Flashing and debugging the RT1060

To know how to flash and debug follow instructions from [README.md 'Flashing and
debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:
    ../../../../../examples/all-clusters-app/nxp/rt/rt1060/README.md#flashdebug

## Testing the test driver app

To view logs, start a terminal emulator like PuTTY and connect to the used COM
port with the following UART settings:

-   Baud rate: 115200
-   8 data bits
-   1 stop bit
-   No parity
-   No flow control
