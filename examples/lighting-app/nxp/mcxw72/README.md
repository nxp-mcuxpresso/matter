# Matter MCXW72 Lighting Example Application

For generic information related to on/off light application,
please see the [common README](../README.md).

- [Matter MCXW72 Lighting Example Application](#matter-mcxw72-lighting-example-application)
  - [Introduction](#introduction)
  - [Device UI](#device-ui)
    - [Configure `matter-cli`](#configure-matter-cli)
  - [Building](#building)
  - [Flashing](#flashing)
    - [Flashing the `NBU` image with `blhost`](#flashing-the-nbu-image-with-blhost)
    - [Flashing the `NBU` image with `JLink`](#flashing-the-nbu-image-with-jlink)
    - [Flashing the host image](#flashing-the-host-image)

## Introduction

This is an on/off lighting application implemented for an mcxw72 device.

The following board was used when testing this Matter reference app
for a `mcxw72` device:

![MCXW72-EVK](../../../platform/nxp/mcxw72/doc/images/mcxw72evk.png)

Please see [MCXW72 product page](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/mcx-arm-cortex-m/mcx-w-series-microcontrollers/mcx-w72x-secure-and-ultra-low-power-mcus-for-matter-thread-zigbee-and-bluetooth-le:MCX-W72X) for more information.

## Device UI

This reference app is using `matter-cli` to send commands
to the board through a UART interface.

| interface | role                       |
| --------- | -------------------------- |
| UART0     | Used for logs and flashing |
| UART1     | Used for `matter-cli`      |

The user actions are summarised below:

| matter-cli command        | output                   |
| ------------------------- | ------------------------ |
| `mattercommissioning on`  | Enable BLE advertising   |
| `mattercommissioning off` | Disable BLE advertising  |
| `matterfactoryreset`      | Initiate a factory reset |
| `matterreset`             | Reset the device         |

### Configure `matter-cli`

You need a `USB-UART` bridge to make use of the second UART interface.
The pin configuration is the following:
-  `J3 pin 32` (UART1 TX)
-  `J3 pin 34` (UART1 RX)
-  `J18 pin 4` (GND)

## Building

Manually building requires running the following commands:

```
user@ubuntu:~/Desktop/git/connectedhomeip$ export NXP_SDK_ROOT=<path_to_SDK>
user@ubuntu:~/Desktop/git/connectedhomeip$ cd examples/lighting-app/nxp/mcxw72
user@ubuntu:~/Desktop/git/connectedhomeip/examples/lighting-app/nxp/mcxw72$ gn gen out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/lighting-app/nxp/mcxw72$ ninja -C out/debug
```

Please note that running `gn gen out/debug` without `--args` option will use the default
gn args values found in `args.gni`.

After a successful build, the `elf` and `srec` files are found in `out/debug/`.
See the files prefixed with `chip-mcxw72-light-example`.

## Flashing

We recommend using [JLink](https://www.segger.com/downloads/jlink/)
to flash both host and `NBU` cores. To support this device, a `JLink`
patch shall be applied, so please contact your NXP liaison for guidance.

| core | JLink target     |
| ---- | ---------------- |
| host | KW47B42ZB7_M33_0 |
| NBU  | KW47B42ZB7_M33_1 |

Note: `NBU` image should be written only when a new NXP SDK is released.

### Flashing the `NBU` image with `blhost`

1. Install [Secure Provisioning SDK tool](https://www.nxp.com/design/design-center/software/development-software/secure-provisioning-sdk-spsdk:SPSDK) using Python:

    ```
    pip install spsdk
    ```

    Note: There might be some dependencies that cause conflicts with
    already installed Python modules. However, `blhost` tool is still
    installed and can be used.

2. Updating `NBU` for Wireless examples

    It is necessary to work with the matching NBU image for the SDK
    version of the application you are working with. This means that
    when you download your SDK, prior to loading any wireless SDK example,
    update your NBU image with the SDK provided binaries:

    `middleware\wireless\ieee-802.15.4\bin\mcxw72\mcxw72_nbu_ble_15_4_dyn.bin`

    1. Place your device in `ISP` mode:
        - Press and hold `SW4` (`BOOT_CONFIG`)
        - Press and hold `SW1` (`RST`)
        - Relax `SW1`
        - Relax `SW4`

    2. Once the device is connected, you may find the assigned port by running:

        ```
        nxpdevscan
        ```

    3. Run the `blhost` command to write the `bin` file:

        ```
        blhost -p <assigned_port> write-memory 0x48800000 <path_to_SDK>/middleware/wireless/ieee-802.15.4/bin/mcxw72/mcxw72_nbu_ble_15_4_dyn.bin

        ```

### Flashing the `NBU` image with `JLink`

Steps:
-   Plug MCXW72 to the USB port
-   Connect JLink to the device:
    ```bash
    JLinkExe -device KW47B42ZB7_M33_1 -if SWD -speed 4000 -autoconnect 1
    ```
-   Run the following commands:
    ```bash
    reset
    halt
    loadbin <path_to_SDK>/middleware/wireless/ieee-802.15.4/bin/mcxw72/mcxw72_nbu_ble_15_4_dyn.bin 0
    reset
    go
    quit
    ```

Note: If running into issues related to board connection, please refer to
[Flashing the `NBU` image with `blhost`](#flashing-the-nbu-image-with-blhost).
This might be needed when the `NBU` core is empty.

### Flashing the host image

Host image is the one found under `out/debug/`. It should be written after each
build process.

Steps:
-   Plug MCXW72 to the USB port
-   Connect JLink to the device:
    ```bash
    JLinkExe -device KW47B42ZB7_M33_0 -if SWD -speed 4000 -autoconnect 1
    ```
-   Run the following commands:
    ```bash
    reset
    halt
    loadfile chip-mcxw72-light-example.srec
    reset
    go
    quit
    ```