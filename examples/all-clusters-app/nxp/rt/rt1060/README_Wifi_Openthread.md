# RT1060 All-cluster Application for Matter over Open Thread + TCP over Wi-Fi component

## Hardware requirements

Mandatory:
- MIMXRT1060-EVKB
- DK6 (Dual PAN and PTA firmware)
- 8801 2DS M.2 Module (rev A)
- Murata uSD-M.2 Adapter (rev B1)
- FTDI

Optional:
- External J-Link (for better debugging)

## Hardware setup

### 8801 2DS M.2 Module - Murata uSD-M.2 Adapter

Insert 8801 2DS M.2 Module into Murata uSD-M.2 Adapter.

### MIMXRT1060-EVKB - Murata uSD-M.2 Adapter

Insert Murata uSD-M.2 Adapter into uSD slot J22 of MIMXRT1060-EVKB.

---
**NOTE**

In case MIMXRT1060-EVKB is used, but the project is compiled with evkA SDK, Murata uSD-M.2 Adapter can't be powered up using uSD pins.
As workaround it is required to power up the Murata uSD-M.2 Adapter using an external micro USB.
To switch the power supply source set the J1 jumper of Murata uSD-M.2 to position 1-2 (towards the center of the Adapter).
Due to the same issue, before each MIMXRT1060-EVKB power up, Murata uSD-M.2 Adapter should be powered off and powered on again.
In this case, the proper boot sequence is:

1. power off uSD-M.2 Adapter
2. power off MIMXRT1060-EVKB
3. power on uSD-M.2 Adapter
4. power on MIMXRT1060-EVKB

---

### MIMXRT1060-EVKB - DK6

| MIMXRT1060-EVKB |       DK6       |
|-----------------|-----------------|
|     J16 - 1     |     PIO - 8     |
|     J16 - 2     |     PIO - 9     |
|     J33 - 3     |     PIO - 6     |
|     J33 - 4     |     PIO - 7     |
|     J17 - 2     |     RSTN        |
|     J32 - 6     |     GND         |

### MIMXRT1060-EVKB - FTDI (optional, for logs)

| MIMXRT1060-EVKB |      FTDI       |
|-----------------|-----------------|
|     J16 - 7     |     RX          |
|     J32 - 7     |     GND         |

### 8801 2DS M.2 Module - DK6

|      8801        |       DK6       |
|------------------|-----------------|
| GPIO1 (Grant)    |     PIO - 14    |
| GPIO3 (Request)  |     PIO - 15    |
| GPIO2 (Priority) |     PIO - 16    |

### MIMXRT1060-EVKB debug with external J-Link (optional)

Remove J9 and J10 jumpers from MIMXRT1060-EVKB. Attach J-Link to J2.


## Building

Note: for the next step, use SDK 2.12.0.
Follow the instructions from [README.md 'Building section'][readme_building_section].

[readme_building_section]: README.md#building

Current demo is TCP over Wi-Fi component on top of a simple All-cluster Application for Matter over Open Thread.

### Prepare All-cluster Application for Matter over Open Thread
Follow the instructions from README_Openthread.md 'Pre-build instructions', 'Flashing the K32W061 OT-RCP transceiver image' and 'Raspberrypi Test harness setup'.

### Build the All-cluster Application

-   Matter over Open Thread + TCP over Wi-Fi component

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_openthread=true k32w0_transceiver=true w8801_transceiver=true chip_enable_wifi=true chip_inet_config_enable_ipv4=false chip_with_ot_cli=1 chip_config_network_layer_ble=false tcp_download=true wifi_ssid=\"my_ssid\" wifi_password=\"my_password\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

-   Matter over Open Thread + TCP over Wi-Fi component and **PTA**

```
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ gn gen --args="chip_enable_openthread=true k32w0_transceiver=true w8801_transceiver=true chip_enable_wifi=true chip_inet_config_enable_ipv4=false chip_with_ot_cli=1 chip_config_network_layer_ble=false tcp_download=true wifi_enable_pta=true wifi_ssid=\"my_ssid\" wifi_password=\"my_password\"" out/debug
user@ubuntu:~/Desktop/git/connectedhomeip/examples/all-clusters-app/nxp/rt/rt1060$ ninja -C out/debug
```

In the above examples, make sure to adjust the following parameters to match your WiFi router configuration:
```
wifi_ssid=\"my_ssid\" wifi_password=\"my_password\"
```

In both cases, the resulting output file can be found in out/debug/chip-rt1060-all-cluster-example.

## Flashing and debugging

To flash and debug follow the instructions from [README.md 'Flashing and debugging'][readme_flash_debug_section].

[readme_flash_debug_section]:README.md#flashdebug


## Testing the example

### Testing All-cluster Application for Matter over Open Thread

Follow the instructions from README_Openthread.md 'Testing the example'.

### Testing TCP over Wi-Fi component

---
**NOTE**

When re-initializing the WiFi driver, the `wlan_init()` call must be guarded as below. Make sure that Thread Stack Manager is already initialized.

```
...
    /* De-initialize the Wi-Fi module. */
    wlan_deinit(0);

    /* Disable the coexistence on K32W0. */
    ThreadStackMgrImpl().LockThreadStack();
    otPlatRadioSetCoexEnabled(NULL, false);
    ThreadStackMgrImpl().UnlockThreadStack();

    /* Initialize the Wi-Fi module. */
    wifi_status = wlan_init(wlan_fw_bin, wlan_fw_bin_len);
    if (wifi_status == WM_SUCCESS)
    {
        /* Wi-Fi module initialized.
         * If required, enable Wi-Fi coexistence here. */
    }

    /* Enable the coexistence on K32W0. */
    ThreadStackMgrImpl().LockThreadStack();
    otPlatRadioSetCoexEnabled(NULL, true);
    ThreadStackMgrImpl().UnlockThreadStack();
...
```

The first call of `otPlatRadioSetCoexEnabled(NULL, false)` ensures that the 8801 PRIO/REQ GPIOs are set at the proper logical level (+3V3), while the WiFi initialization takes place.
During this time, the K32W0 coexistence is disabled, thus the K32W0 will no longer take into account any signal that's being present on the GRANT pin.
The second call `otPlatRadioSetCoexEnabled(NULL, true)` re-enables the coexistence mechanism between the 8801 and the K32W0, ensuring fair access on the air.

---

Extract the IP address of the 8801 2DS M.2 Module from the RT logs (via FTDI).

Log example: '[INFO] [DL] Connected to "my_ssid" with IP = [192.168.1.177]'

On your laptop, navigate to the examples/all-clusters-app/nxp/common/tcp_download_test/script/ folder where tcp_test.py is located.

Open a terminal and run the below command:
```
python3 tcp_test.py -ip <ip of 8801 extracted from RT logs> -p 80 -cs 1024 -f <file you want to transfer>
```

The file you chose to transfer will be sent from the laptop to the RT via TCP over Wi-Fi.
The transferred file will not be stored on the RT, but instead both python script and the RT will compute a simple
hash of the transferred and received file and will display its value after the transfer.
On both sides the hash should be the same, meaning that the file was transferred successfully.
