# Matter over Wi-Fi on RT1060 + transceiver

## Configuration(s) supported

Here are listed configurations that allow to support Matter over Wi-Fi on RT1060:

- RT1060 + IW416 (Wi-Fi + BLE)
- RT1060 + 8801 (Wi-Fi)
- RT1060-EVKC + IW612 (Wi-fi +BLE)

## Hardware requirements RT1060+IW416

Host part:

- 1 MIMXRT1060-EVKB or EVK-MIMXRT1060 board
- external 5V supply

Transceiver part:

- 1 AzureWave AW-AM510-uSD evaluation board

Jumper settings for MIMXRT1060-EVKB (enables external 5V supply):

- remove  J40 5-6
- connect J40 1-2
- connect J45 with external power (controlled by SW6 - position 3)

Jumper settings for EVK-MIMXRT1060 (enables external 5V supply):

- remove  J1 5-6
- connect J1 1-2
- connect J2 with external power (enabled by SW1 - position 3)

The hardware should be reworked according to the chapter *Hardware Rework Guide for MIMXRT1060-EVKB and AW-AM510-uSD* or the chapter *Hardware Rework Guide for MIMXRT1060-EVK and AW-AM510-uSD* in the document *Hardware Rework Guide for EdgeFast BT PAL* which can be found in the NXP RT1060 SDK (docs/wireless/Bluetooth/Edgefast_bluetooth/Hardware Rework Guide for EdgeFast BT PAL.pdf):

- Make sure resistors R368/R376/R347/R349/R365/R363/R193/R186 are removed.

Only the SDK package downloaded from https://mcuxpresso.nxp.com contains the PDF document mentioned above, it is not present in the SDK downloaded from GitHub using the west tool.

Jumper settings for AzureWave AW-AM510-uSD Module:

- J4  1-2: VIO 1.8V (Voltage level of SDIO pins is 1.8V)
- J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
- The pin 1 of J4 is not marked on the board. Please note that pin numbering of J4 is opposite to J2.

Plug AW-AM510-uSD into uSD connector J22 on MIMXRT1060-EVKB or J39 on EVK-MIMXRT1060.

Connect the following pins between RT1060 and AW-AM510-uSD to enable Bluetooth HCI UART:

| PIN NAME | AW-AM510-USD | MIMXRT1060-EVKB | EVK-MIMXRT1060 | PIN NAME OF RT1060 | GPIO NAME OF RT1060 |
|----------|--------------|-----------------|----------------|--------------------|---------------------|
| UART_TXD | J10 (pin 4)  | J16 (pin 1)     | J22 (pin 1)    | LPUART3_RXD        | GPIO_AD_B1_07       |
| UART_RXD | J10 (pin 2)  | J16 (pin 2)     | J22 (pin 2)    | LPUART3_TXD        | GPIO_AD_B1_06       |
| UART_RTS | J10 (pin 6)  | J33 (pin 3)     | J23 (pin 3)    | LPUART3_CTS        | GPIO_AD_B1_04       |
| UART_CTS | J10 (pin 8\) | J33 (pin 4)     | J23 (pin 4)    | LPUART3_RTS        | GPIO_AD_B1_05       |
| GND      | J6  (pin 7)  | J32 (pin 7)     | J25 (pin 7)    | GND                | GND                 |

Attach external antenna into connector on AW-AM510-uSD.

Additional information about the AW-AM510-uSD can be found in the user manual *UM11441 - Getting Started with NXP-based Wireless Modules and i.MX RT Platform Running RTOS*, which can be found in the NXP RT1060 SDK (docs/wireless/UM11441-Getting-Started-with-NXP-based-Wireless-Modules-and-i.MX-RT-Platform-Running-on-RTOS.pdf). Only the SDK package downloaded from https://mcuxpresso.nxp.com contains the PDF document, it is not present in the SDK downloaded from GitHub using the west tool.

## Hardware requirements RT1060+8801
Host part:
- 1 MIMXRT1060-EVKB

Transceiver part :
- 1 8801 2DS M.2 Module (rev A)
- 1 Murata uSD-M.2 Adapter (rev B1)

The 8801 2DS M.2 Module should be inserted into the Murata uSD-M.2 Adapter and inserted in the uSD slot J22 of MIMXRT1060-EVKB. The Murata uSD-M.2 Adapter can be powered up using uSD pins. For that, set the J1 jumper of Murata uSD-M.2 to position 2-3 (Position 2-3: VBAT supply, typical 3.1 ~ 3.3V, from microSD connector).

Note: as the 8801 module supports only the 2.4 GHz Wi-Fi band, it is mandatory to connect it to a Wi-Fi access point on the 2.4 GHz band.

## Hardware requirements RT1060-EVKC+IW612
Host part:
- 1 MIMXRT1060-EVKC

    Hardware should be reworked as below:
    - populate R93, R96, R2155, R2156, R2157, R2158, R2159 with 0Ohm resistors
    - J76 and J107 jumpers in 2-3 position.

Transceiver part :
- 1 IW612 ( Firecrest)  2EL M.2 Module (rev A1)

The Iw612 module should be plugged to the M.2 connector on RT1060-EVKC board. 

## Building

First instructions from [README.md 'Building section'][readme_building_section] should be followed.

[readme_building_section]: README.md#building

-   Build the Wi-fi configuration for MIMXRT1060-EVKB board + IW416 transceiver (with BLE for commissioning).

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_wifi=true iw416_transceiver=true" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

-   Build the Wi-fi configuration for EVKB-MIMXRT1060 board + 8801 with Matter-over-Wifi configuration and only onnetwork commissioning (without BLE, the WiFi network credentials are provided at build-time which will enable the device to join the Wi-Fi AP at startup):

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_wifi=true w8801_transceiver=true chip_config_network_layer_ble=false tcp_download=true wifi_ssid=\"your_wifi_ssid\" wifi_password=\"your_wifi_password\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

-   Build the Wi-fi configuration for MIMXRT1060-EVKC board + IW612 transceiver (with BLE for commissioning).

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_wifi=true iwx12_transceiver=true evkname=\"evkcmimxrt1060\" " out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```
-   Build the Wi-fi configuration for MIMXRT1060-EVKC board + IW612 with Matter-over-Wifi configuration and only onnetwork commissioning (without BLE, the WiFi network credentials are provided at build-time which will enable the device to join the Wi-Fi AP at startup):

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_wifi=true iwx12_transceiver=true evkname=\"evkcmimxrt1060\" chip_config_network_layer_ble=false tcp_download=true wifi_ssid=\"your_wifi_ssid\" wifi_password=\"your_wifi_password\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

The resulting output file can be found in out/debug/chip-rt1060-all-cluster-example

<a name="flashdebug"></a>

## Flashing and debugging

To know how to flash and debug follow instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashing-and-debugging

## Testing the example

Follow instructions from [README.md 'Testing the example'][readme_test_example_section].

[readme_test_example_section]:README.md#testing-the-example
