# CHIP NXP Zephyr All-clusters Application

The all-clusters example implements a server which can be accessed by a CHIP controller and can accept basic cluster commands.

The example is based on [Project CHIP](https://github.com/project-chip/connectedhomeip) and the NXP Zephyr SDK,
and provides a prototype application that demonstrates device commissioning and different cluster control.

<hr>

-   [Introduction](#intro)
-   [Building](#building)
-   [Flashing and debugging](#flashdebug)
-   [Manufacturing data](#manufacturing)
-   [Testing the example](#testing-the-example)
-   [Using Matter CLI in NXP Zephyr examples](#matter-cli)

<hr>

<a name="intro"></a>

## Introduction

The Zephyr all-clusters application provides a working demonstration of supported board integration from Zephyr,
built using the Project CHIP codebase and the NXP/Zephyr SDK.
The example supports basic ZCL commands and acts as a laundry washer device-type.

The example supports:
- Matter over Wi-Fi

The example targets the following Zephyr boards:
- `rd_rw612_bga`

<a name="building"></a>

## Building

In order to build the Project CHIP example, we recommend using a Linux
distribution (the demo-application was compiled on Ubuntu 20.04).

Prerequisites:
- Follow instruction from [BUILDING.md](../../../../docs/guides/BUILDING.md) to setup the Matter environment
- Follow instruction from [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) to setup a Zephyr workspace

>**Note**: Currently, supported NXP platforms in Zephyr targetting Matter are not available in the official Zephyr repo,
> you'll have to use a private github repo. Reach to your NXP contact for more details.

Steps to build the example, targetting `rd_rw612_bga` board:

1. Activate your Matter env:
```shell
source <path to CHIP workspace>/scripts/activate.sh
```
2. Source zephyr-env.sh:
```shell
source <path to zephyr repo>/zephyr-env.sh
```
3. Run west build command:
```shell
west build -b rd_rw612_bga -p  <path to this folder>
```

By default, a folder `build` will be created in the same folder you run the command from.
The binaries will be created in `build/zephyr` with the name `zephyr.elf` and `zephyr.bin`.

You can get more details on `west build` with [Zephyr's building guide](https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html#building-west-build)

<a name="flashdebug"></a>

## Flashing and debugging

### Flashing without debugging

`west` can be used to flash a target, as an example for `rd_rw612_bga` board:
```shell
west flash -i <J-Link serial number>
```

You can get more details on `west flash` with [Zephyr's flashing guide](https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html#flashing-west-flash)

>**Note**: `west flash` will not start a debug session, it will only flash and reset the device

### Flash and debug

To debug a Matter with Zephyr application, you could use several methods:
- [MCUXpresso IDE (version >= 11.6.0)](https://www.nxp.com/design/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE)
- `VS Code` paired with [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) extension
- `west flash` [Zephyr's debugging guide](https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html#id29)

This guide will cover the VS Code use case.

>**Note**: As the build provides an elf file, any compatible debugging tool can be used.

#### Debugging with VS Code

TODO

<a name="manufacturing"></a>

## Manufacturing data

See [Guide for writing manufacturing data on NXP devices](../../../../docs/guides/nxp_manufacturing_flow.md)

>**Note**: The all cluster app demonstrates the usage of encrypted Matter manufacturing data storage.
> Matter manufacturing data should be encrypted using an AES 128 software key before flashing them to the device flash.

<a name="testing-the-example"></a>

## Testing the example

To know how to commission a device over BLE, follow the instructions from [chip-tool's README.md 'Commission a device over BLE'](../../../chip-tool/README.md#commission-a-device-over-ble).

Please refer to boards specific documents in `docs` folder to learn how the example is configured for each supported board:
- [rd_rw612_bga](docs/rd_rw612_bga.md)

<a name="matter-cli"></a>

## Using Matter CLI in NXP Zephyr examples

Some Matter examples for the development kits from NXP include
a command-line interface that allows access to application logs and
[Zephyr shell](https://docs.zephyrproject.org/1.13.0/subsystems/shell.html).

Depending on the platform, the CLI console and the logs can be split on two different interfaces.
You may refer to `docs/<board name>.md` file to check how the board is configured for this example.
The binding `zephyr,console` is used to print the logs, while the binding `zephyr,shell-uart` is used for the CLI.
If the logs and the CLI are split among two serial interfaces, you will have to open both ports.

To access the CLI console, use a serial terminal emulator of your choice, like
Minicom or GNU Screen. Use the baud rate set to `115200`.

### Example: Starting the CLI console with Minicom

For example, to start using the CLI console with Minicom, run the following
command with `/dev/ttyACM0` replaced with the device node name of your
development kit:

```
minicom -D /dev/ttyACM0 -b 115200
```

When you reboot the kit, you will see the boot logs in the console, similar to
the following messages:

```shell
uart:~$ MAC Address: C0:95:DA:00:00:6E
...
wlan-version
wlan-mac
wlan-thread-info
wlan-net-stats
...
*** Booting Zephyr OS build zephyr-v3.4.0-187-g2ddf1330209b ***
I: Init CHIP stack
...
```

This means that the console is working correctly and you can start using shell
commands. For example, issuing the `kernel threads` command will print
information about all running threads:

```shell
uart:~$ kernel threads
Scheduler: 277 since last call
Threads:
 0x20006518 CHIP
        options: 0x0, priority: -1 timeout: 536896912
        state: pending
        stack size 8192, unused 7256, usage 936 / 8192 (11 %)

 0x20004ab0 SDC RX
        options: 0x0, priority: -10 timeout: 536890152
        state: pending
        stack size 1024, unused 848, usage 176 / 1024 (17 %)
...
```

## Listing all commands

To list all available commands, use the Tab key, which is normally used for the
command completion feature.

Pressing the Tab key in an empty command line prints the list of available
commands:

```shell
uart:~$
  clear            device           help             history
  hwinfo           kernel           matter           resize
  retval           shell
```

Pressing the Tab key with a command entered in the command line cycles through
available options for the given command.

You can also run the `help` command:
```shell
uart:~$ help
Please press the <Tab> button to see all available commands.
You can also use the <Tab> button to prompt or auto-complete all commands or its subcommands.
You can try to call commands with <-h> or <--help> parameter for more information.

Shell supports following meta-keys:
  Ctrl + (a key from: abcdefklnpuw)
  Alt  + (a key from: bf)
Please refer to shell documentation for more details.

Available commands:
  clear            :Clear screen.
  device           :Device commands
  help             :Prints the help message.
  history          :Command history.
  hwinfo           :HWINFO commands
  kernel           :Kernel commands
  matter           :Matter commands
  resize           :Console gets terminal screen size or assumes default in case
                    the readout fails. It must be executed after each terminal
                    width change to ensure correct text display.
  retval           :Print return value of most recent command
  shell            :Useful, not Unix-like shell commands.
```

## Using General purpose commands

### `kernel` command group

#### `reboot` subcommand

Performs either a warm or cold reset.
Difference between warm and cold resets may vary from a platform to another, but on default Cortex-M architectures,
both methods are the same, so use either one if nothing is specific to your platform.

```shell
uart:~$ kernel reboot cold|warm
```

## Using Matter-specific commands

The NXP Zephyr examples let you use several Matter-specific CLI commands.

These commands are not available by default and to enable using them, set the
`CONFIG_CHIP_LIB_SHELL=y` Kconfig option in the `prj.conf` file of the given
example.

Every invoked command must be preceded by the `matter` prefix.

See the following subsections for the description of each Matter-specific
command.

### `device` command group

Handles a group of commands that are used to manage the device. You must use
this command together with one of the additional subcommands listed below.

#### `factoryreset` subcommand

Performs device factory reset that is hardware reset preceded by erasing of the
whole Matter settings stored in a non-volatile memory.

```shell
uart:~$ matter device factoryreset
Performing factory reset ...
```

### `onboardingcodes` command group

Handles a group of commands that are used to view information about device
onboarding codes. The `onboardingcodes` command takes one required parameter for
the rendezvous type, then an optional parameter for printing a specific type of
onboarding code.

The full format of the command is as follows:

```
onboardingcodes none|softap|ble|onnetwork [qrcode|qrcodeurl|manualpairingcode]
```

#### `none` subcommand

Prints all onboarding codes. For example:

```shell
uart:~$ matter onboardingcodes none
QRCode:             MT:W0GU2OTB00KA0648G00
QRCodeUrl:          https://project-chip.github.io/connectedhomeip/qrcode.html?data=MT%3AW0GU2OTB00KA0648G00
ManualPairingCode:  34970112332
```

#### `none qrcode` subcommand

Prints the device onboarding QR code payload. Takes no arguments.

```shell
uart:~$ matter onboardingcodes none qrcode
MT:W0GU2OTB00KA0648G00
```

#### `none qrcodeurl` subcommand

Prints the URL to view the device onboarding QR code in a web browser. Takes no arguments.

```shell
uart:~$ matter onboardingcodes none qrcodeurl
https://project-chip.github.io/connectedhomeip/qrcode.html?data=MT%3AW0GU2OTB00KA0648G00
```

#### `none manualpairingcode` subcommand

Prints the pairing code for the manual onboarding of a device. Takes no
arguments.

```shell
uart:~$ matter onboardingcodes none manualpairingcode
34970112332
```

### `config` command group

Handles a group of commands that are used to view device configuration
information. You can use this command without any subcommand to print all
available configuration data or to add a specific subcommand.

```shell
uart:~$ matter config
VendorId:        65521 (0xFFF1)
ProductId:       32768 (0x8000)
HardwareVersion: 1 (0x1)
FabricId:
PinCode:         020202021
Discriminator:   f00
DeviceId:
```

The `config` command can also take the subcommands listed below.

#### `pincode` subcommand

Prints the PIN code for device setup. Takes no arguments.

```shell
uart:~$ matter config pincode
020202021
```

#### `discriminator` subcommand

Prints the device setup discriminator. Takes no arguments.

```shell
uart:~$ matter config discriminator
f00
```

#### `vendorid` subcommand

Prints the vendor ID of the device. Takes no arguments.

```shell
uart:~$ matter config vendorid
65521 (0xFFFF1)
```

#### `productid` subcommand

Prints the product ID of the device. Takes no arguments.

```shell
uart:~$ matter config productid
32768 (0x8000)
```

#### `hardwarever` subcommand

Prints the hardware version of the device. Takes no arguments.

```shell
uart:~$ matter config hardwarever
1 (0x1)
```

#### `deviceid` subcommand

Prints the device identifier. Takes no arguments.

#### `fabricid` subcommand

Prints the fabric identifier. Takes no arguments.

### `ble` command group

Handles a group of commands that are used to control the device Bluetooth LE
transport state. You must use this command together with one of the additional
subcommands listed below.

#### `help` subcommand

Prints help information about the `ble` command group.

```shell
uart:~$ matter ble help
  help            Usage: ble <subcommand>
  adv             Enable or disable advertisement. Usage: ble adv <start|stop|state>
```

#### `adv start` subcommand

Enables Bluetooth LE advertising.

```shell
uart:~$ matter ble adv start
Starting BLE advertising
```

#### `adv stop` subcommand

Disables Bluetooth LE advertising.

```shell
uart:~$ matter ble adv stop
Stopping BLE advertising
```

#### `adv status` subcommand

Prints the information about the current Bluetooth LE advertising status.

```shell
uart:~$ matter ble adv state
BLE advertising is disabled

```

### `dns` command group

Handles a group of commands that are used to trigger performing DNS queries. You
must use this command together with one of the additional subcommands listed
below.

#### `browse` subcommand

Browses for DNS services of `_matterc_udp` type and prints the received
response. Takes no argument.

```shell
uart:~$ matter dns browse
Browsing ...
DNS browse succeeded:
   Hostname: 0E824F0CA6DE309C
   Vendor ID: 9050
   Product ID: 20043
   Long discriminator: 3840
   Device type: 0
   Device name:
   Commissioning mode: 0
   IP addresses:
      fd08:b65e:db8e:f9c7:2cc2:2043:1366:3b31
```

#### `resolve` subcommand

Resolves the specified Matter node service given by the _fabric-id_ and
_node-id_.

```shell
uart:~$ matter dns resolve fabric-id node-id
Resolving ...
DNS resolve for 000000014A77CBB3-0000000000BC5C01 succeeded:
   IP address: fd08:b65e:db8e:f9c7:8052:1a8e:4dd4:e1f3
   Port: 5540
```

### `stat` command group

Handles a group of commands that are used to get and reset the peak usage of
critical system resources used by Matter. These commands are only available when
the `CONFIG_CHIP_STATISTICS=y` Kconfig option is set.

#### `peak` subcommand

Prints the peak usage of system resources.

```shell
uart:~$ matter stat peak
Packet Buffers: 1
Timers: 2
TCP endpoints: 0
UDP endpoints: 1
Exchange contexts: 0
Unsolicited message handlers: 5
Platform events: 2
```

#### `reset` subcommand

Resets the peak usage of system resources.

```shell
uart:~$ matter stat reset
```
