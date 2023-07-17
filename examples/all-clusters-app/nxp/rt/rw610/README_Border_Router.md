# Matter with Open Thread Border Router support on RW612

## Hardware requirements

- RD-RW612-BGA board
- Wi-Fi antenna (to plug in Ant1) + BLE/15.4 antenna (to plug in Ant2)

<a name="building"></a>

## Building

First instructions from [README.md 'Building section'][readme_building_section] should be followed.

[readme_building_section]: README.md#building

In the Border Router configuration, the Matter CLI is enabled to control Open Thread.
- Build Matter with Border Router configuration with BLE commissioning (ble-wifi) :

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ gn gen --args="chip_enable_wifi=true chip_enable_openthread=true chip_enable_matter_cli=true is_sdk_package=true" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ ninja -C out/debug
```
- Build Matter Border Router configuration with onnetwork commissioning (the WiFi network credentials are provided at build-time which will enable the device to join the network at the startup):
```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ gn gen --args="chip_enable_wifi=true chip_enable_openthread=true chip_enable_matter_cli=true is_sdk_package=true tcp_download=true wifi_ssid=\"your_wifi_ssid\" wifi_password=\"your_wifi_password\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rw610$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rw610-all-cluster-example.

<a name="flashdebug"></a>

## Flashing and debugging

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashing-and-debugging

## Testing the example

To create or join a Thread network on the Matter Border Router we use the otcli command with the Matter CLI.

To get the list of Matter CLI commands type help:
```
> help
  base64          Base64 encode / decode utilities
  exit            Exit the shell application
  help            List out all top level commands
  version         Output the software version
  ble             BLE transport commands
  wifi            Usage: wifi <subcommand>
  config          Manage device configuration. Usage to dump value: config [param_name] and to set some values (discriminator): config [param_name] [param_value].
  device          Device management commands
  onboardingcodes Dump device onboarding codes. Usage: onboardingcodes none|softap|ble|onnetwork [qrcode|qrcodeurl|manualpairingcode]
  dns             Dns client commands
  echo            Echo back provided inputs
  log             Logging utilities
  rand            Random number utilities
  otcli           Dispatch OpenThread CLI command
  mattercommissioning Open/close the commissioning window. Usage : mattercommissioning [on|off]
  matterfactoryreset Perform a factory reset on the device
  matterreset     Reset the device
  switch          Switch commands. Usage: switch [on|off]
Done
```
Then we configure the Thread network parameters to start/join a network:

```
> otcli channel 21
> otcli panid 21
> otcli networkkey 00112233445566778899aabbccddeeff
> otlci ifconfig up
> otcli thread start

> otcli state
leader
Done
```
To retrieve the active dataset that is used by the chip tool to comission other Matter over Thread devices, use:

```
> otcli dataset active -x
0e080000000000000000000300001535060004001fffe00208dead00beef00cafe0708fddead00beef0000051000112233445566778899aabbccddeeff030a4f70656e5468726561640102228c04105b3e84a53232b7736bb195a98d535d390c0402a0f7f8
Done
```
Then we can run the chip tool on the raspberry pi to commission a Matter over Thread device using the active dataset obtained above. Note that on the raspberry pi, the OTBR docker must not be started and no RCP dongle is needed as RW612 takes the role of Open Thread Border Router.

```
./chip-tool pairing ble-thread 1 hex:<dataset> 20202021 3840

```

To commission the Matter application on RW612 over Wi-Fi we can use either use wifi-ble or onnetwork commissioning modes depending on how the application was built as described in the section [Building][building_section].

[building_section]:#building

For ble-wifi:

```
./chip-tool pairing ble-wifi 1 <ssid> <password> 20202021 3840

```
For Wi-Fi credentials provided at compile time:

```
./chip-tool pairing onnetwork 1 20202021

```
