# Matter with Open Thread Border Router support on RT1060+88W8801+K32W0x1DK6

## Hardware requirements

- i.MX RT1060 EVKA or EVKB board
- 88W8801 module (for Wi-Fi connection), for example 88W8801 2DS M.2 Module (rev A) and Murata uSD-M.2 Adapter (rev B1)
- K32W0x1 mezzanine module (for Thread connection)
- IOTZTB-DK006 carrier board for the K32W0x1 module (referenced as DK6 carrier board)

<a name="building"></a>

## Building

First instructions from [README.md 'Building section'][readme_building_section] should be followed.

[readme_building_section]: README.md#building

In the Border Router configuration, the Matter CLI needs to be enabled to control OpenThread interface.

- Build Matter Border Router configuration with onnetwork commissioning (the Wi-Fi network credentials are provided at build-time which will enable the device to join the network at the startup):
```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_wifi=true w8801_transceiver=true chip_enable_matter_cli=true chip_config_network_layer_ble=true tcp_download=true is_debug=true optimize_debug=false chip_enable_openthread=true k32w0_transceiver=true k32w0_transceiver_bin_path=\"/path/to/ot-rcp/ot-rcp-ble-hci-bb-k32w061.elf.bin.h\" hci_spinel_single_uart=true wifi_ssid=\"your_wifi_ssid\" wifi_password=\"your_wifi_password\" openthread_root =\"//third_party/connectedhomeip/third_party/openthread/ot-nxp/openthread-br\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rt1060-all-cluster-example.

<a name="flashdebug"></a>

## Flashing and debugging

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashdebug

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
> otcli ifconfig up
> otcli thread start
```

To check that the Thread interface is up and running, the following command can be entered:

```
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

Then we can run the chip tool from any Linux based controller device that is in the same Wi-Fi network with the RT1060 OTBR device to commission a Matter over Thread device using the active dataset obtained above.

```
./chip-tool pairing ble-thread 1 hex:<dataset> 20202021 3840

```

To commission the Matter application on RT1060 over Wi-Fi, we can use either use wifi-ble or onnetwork commissioning modes depending on how the application was built as described in the section [Building][building_section].

[building_section]:#building

For Wi-Fi credentials provided at compile time:

```
./chip-tool pairing onnetwork 1 20202021

```
