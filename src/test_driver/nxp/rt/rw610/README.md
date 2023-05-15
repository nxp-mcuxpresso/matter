# Matter crypto unit tests on RW610

This directory contains test driver for the NXP RW610 platform.

The application is used to test crypto module.

## Build crypto unit tests

Build the runner using gn. Note : Building with SDK package and SDK internal are both supported, for this replace ```is_<sdk_type>``` with ```is_sdk_package``` if building with SDK package, or with ```is_sdk_internal``` if internal SDK is used instead :

```
user@ubuntu:~/Desktop/git/connectedhomeip$ cd src/test_driver/nxp/rt/rw610
user@ubuntu:~/Desktop/git/connectedhomeip/src/test_driver/nxp/rt/rw610$ gn gen --args="chip_config_network_layer_ble=false chip_enable_wifi=true chip_inet_config_enable_ipv4=false chip_build_tests=true chip_build_test_static_libraries=false is_<sdk_type>=true" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/src/test_driver/nxp/rt/rw610$ ninja -C out/debug
```

Note : Argument ```chip_build_test_static_libraries=false``` is mandatory in
order to link unit test file during the build. Argument ```is_debug=true optimize_debug=false``` could be used to build the
application in debug mode. 

## Flashing and debugging RW610

To know how to flash and debug follow instructions from [README.md 'Flashing and
debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:
    ../../../../../examples/all-clusters-app/nxp/rt/rw610/README.md#flashdebug

## Testing the test driver app

To view logs, start a terminal emulator like PuTTY and connect to the used COM
port with the following UART settings:

-   Baud rate: 115200
-   8 data bits
-   1 stop bit
-   No parity
-   No flow control
