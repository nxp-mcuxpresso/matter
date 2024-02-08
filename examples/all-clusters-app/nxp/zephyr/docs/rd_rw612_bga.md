# Board information

## Matter CLI

The Matter CLI is configured to be output on `flexcomm3` with a baudrate of `115200`.

`flexcomm3` is wired to the USB FTDI port of the RD BGA board by default.

## Logs

The logs are configured to be output on `flexcomm0` with a baudrate of `115200`.

`flexcomm0` is wired to `GPIO2` (RX) and `GPIO3` (TX).
Those pins are accessible on `HD2` pin header.

## Factory data partition

The factory data partition is reserved in the last sector of the flexspi flash of RD BGA board, at `0x0BFFF000`

## File system partition

The file system partition used to store various settings for Matter and the application is reserved starting from
`0x8A60000` and ending at `0xBFFEFFF` (right before the factory data partition).

>**Note**: This is subject to change when OTA will be supported on NXP Zephyr applications
