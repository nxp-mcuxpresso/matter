---
orphan: true
---

# NXP manufacturing data guide

By default, the example application is configured to use generic test
certificates and provisioning data embedded with the application code. It is
possible for a final stage application to generate its own manufacturing data
using the procedure described below.

## 1. Prerequisites

Generate build files from Matter root folder by running:
```
gn gen out
```

Build `chip-cert` tool:

```
ninja -C out chip-cert
```

Build `spake2p` tool:

```
ninja -C out spake2p
```

### Environment variables

A user can customize the certificate generation by setting some environment
variables that are used within the utility scripts. Please note that the values
below are just an example and should be modified accordingly:

```
export FACTORY_DATA_DEST=path/factory/data/dest
export DEVICE_TYPE=100
export DATE="2023-01-01"
export TIME=$(date +"%T")
export LIFETIME="7305"
export VID="1037"
export PID="A220"
```

Please pay extra attention to the `DATE` field, since device attestation could fail
if the certificate date is not valid (e.g. validity starts from a future date).

`FACTORY_DATA_DEST` is the path where all factory related data is generated.

`DEVICE_TYPE` should be updated according to the application device type (0x0100
for the provided K32W0 lighting app).

Additionally, `PAA_CERT` and `PAA_KEY` paths can be specified to use an already
existent **PAA**:

```
export PAA_CERT=path/certs/Chip-PAA-NXP-Cert.pem
export PAA_KEY=path/certs/Chip-PAA-NXP-Key.pem
```

## 2. Generate

### a. Certificates

```
./scripts/tools/nxp/generate_cert.sh ./out/chip-cert
```

The output of the script is the **DAC**, **PAI** and **PAA** certificates. If
`FACTORY_DATA_DEST` is set, the certificates will be moved there. The **DAC**
and **PAI** certificates will be written in a special section of the internal
flash, while the **PAA** will be used by `chip-tool` as trust anchor. Please
note that for _real production manufacturing_ the "production PAA" is trusted
via the **DCL** rather than through the generated **PAA** certificate. The
**PAI** certificate may also have a different lifecycle.

### b. Certification declaration (CD)

```
./out/chip-cert gen-cd --key ./credentials/test/certification-declaration/Chip-Test-CD-Signing-Key.pem --cert ./credentials/test/certification-declaration/Chip-Test-CD-Signing-Cert.pem --out $FACTORY_DATA_DEST/Chip-Test-CD-$VID-$PID.der --format-version 1 --vendor-id "0x$VID" --product-id "0x$PID" --device-type-id "0x$DEVICE_TYPE" --certificate-id "ZIG20142ZB330003-24" --security-level 0 --security-info 0 --version-number 9876 --certification-type 1
```

The command above is extracted from `./credentials/test/gen-test-cds.sh` script.
The CD generation uses predefined key and certificate found in
`./credentials/test/certification-declaration`. This **CSA** certificate is also
hard-coded as Trust Anchor in the current `chip-tool` version.

By default, the CD is added to the factory data section. In order to have it
integrated in the application binary, set
`CHIP_USE_DEVICE_CONFIG_CERTIFICATION_DECLARATION` to 1 in the application's
CHIPProjectConfig.h file.

### c. Provisioning data

Generate new provisioning data and convert all the data to a binary (unencrypted
data):

```
python3 ./scripts/tools/nxp/factory_data_generator/generate.py -i 10000 -s UXKLzwHdN3DZZLBaL2iVGhQi/OoQwIwJRQV4rpEalbA= -p 14014 -d 1000 --vid "0x$VID" --pid "0x$PID" --vendor_name "NXP Semiconductors" --product_name "Lighting app" --serial_num "12345678" --date "$DATE" --hw_version 1 --hw_version_str "1.0" --cert_declaration $FACTORY_DATA_DEST/Chip-Test-CD-$VID-$PID.der --dac_cert $FACTORY_DATA_DEST/Chip-DAC-NXP-$VID-$PID-Cert.der --dac_key $FACTORY_DATA_DEST/Chip-DAC-NXP-$VID-$PID-Key.der --pai_cert $FACTORY_DATA_DEST/Chip-PAI-NXP-$VID-$PID-Cert.der --spake2p_path ./out/spake2p --unique_id "00112233445566778899aabbccddeeff" --out $FACTORY_DATA_DEST/factory_data.bin
```

Same example as above, but with an already generated verifier passed as input:

```
python3 ./scripts/tools/nxp/factory_data_generator/generate.py -i 10000 -s UXKLzwHdN3DZZLBaL2iVGhQi/OoQwIwJRQV4rpEalbA= -p 14014 -d 1000 --vid "0x$VID" --pid "0x$PID" --vendor_name "NXP Semiconductors" --product_name "Lighting app" --serial_num "12345678" --date "$DATE" --hw_version 1 --hw_version_str "1.0" --cert_declaration $FACTORY_DATA_DEST/Chip-Test-CD-$VID-$PID.der --dac_cert $FACTORY_DATA_DEST/Chip-DAC-NXP-$VID-$PID-Cert.der --dac_key $FACTORY_DATA_DEST/Chip-DAC-NXP-$VID-$PID-Key.der --pai_cert $FACTORY_DATA_DEST/Chip-PAI-NXP-$VID-$PID-Cert.der --spake2p_path ./out/spake2p --spake2p_verifier ivD5n3L2t5+zeFt6SjW7BhHRF30gFXWZVvvXgDxgCNcE+BGuTA5AUaVm3qDZBcMMKn1a6CakI4SxyPUnJr0CpJ4pwpr0DvpTlkQKqaRvkOQfAQ1XDyf55DuavM5KVGdDrg== --unique_id "00112233445566778899aabbccddeeff" --out $FACTORY_DATA_DEST/factory_data.bin
```

Generate new provisioning data and convert all the data to a binary (encrypted
data with the AES key). Add the following option to one of the above examples:

```
--aes128_key 2B7E151628AED2A6ABF7158809CF4F3C
```

Here is the interpretation of the **required** parameters:

```
-i                 -> SPAKE2+ iteration
-s                 -> SPAKE2+ salt (passed as base64 encoded string)
-p                 -> SPAKE2+ passcode
-d                 -> discriminator
--vid              -> Vendor ID
--pid              -> Product ID
--vendor_name      -> Vendor Name
--product_name     -> Product Name
--hw_version       -> Hardware Version as number
--hw_version_str   -> Hardware Version as string
--cert_declaration -> path to the Certification Declaration (der format) location
--dac_cert         -> path to the DAC (der format) location
--dac_key          -> path to the DAC key (der format) location
--pai_cert         -> path to the PAI (der format) location
--spake2p_path     -> path to the spake2p tool
--out              -> name of the binary that will be used for storing all the generated data
```

Here is the interpretation of the **optional** parameters:

```
--dac_key_password     -> Password to decode DAC key
--dac_key_use_sss_blob -> Used when --dac_key contains a path to an encrypted blob, instead of the
                          actual DAC private key. The blob metadata size is 24, so the total length
                          of the resulting value is private key length (32) + 24 = 56. False by default.
--spake2p_verifier     -> SPAKE2+ verifier (passed as base64 encoded string). If this option is set,
                          all SPAKE2+ inputs will be encoded in the final binary. The spake2p tool
                          will not be used to generate a new verifier on the fly.
--aes128_key           -> 128 bits AES key used to encrypt the whole dataset. Please make sure
                          that the target application/board supports this feature: it has access to
                          the private key and implements a mechanism which can be used to decrypt
                          the factory data information.
--date                 -> Manufacturing Date (YYYY-MM-DD format)
--part_number          -> Part number as string
--product_url          -> Product URL as string
--product_label        -> Product label as string
--serial_num           -> Serial Number
--unique_id            -> Unique id used for rotating device id generation
```

## 3. Write provisioning data

For the **K32W0x1** variants, the binary needs to be written in the internal
flash at location **0x9D600** using `DK6Programmer.exe`:

```
DK6Programmer.exe -Y -V2 -s <COM_PORT> -P 1000000 -Y -p FLASH@0x9D600="factory_data.bin"
```

For **K32W1** platform, the binary needs to be written in the internal flash at
location given by `__MATTER_FACTORY_DATA_START`, using `JLink`:

```
loadfile factory_data.bin 0xec000
```
where `0xec000` is the value of `__MATTER_FACTORY_DATA_START` in the corresponding .map file (can be different if using a custom linker script).

For the **RT1060**, **RT1170** and **RW61X** platform, the binary needs to be
written using `MCUXpresso Flash Tool GUI` at the address value corresponding to
`__FACTORY_DATA_START` (the map file of the application should be checked to get
the exact value).

## 4. Build app and usage

Use `chip_with_factory_data=1` when compiling to enable factory data usage.

Run chip-tool with a new PAA:

```
./chip-tool pairing ble-thread 2 hex: $hex_value 14014 1000 --paa-trust-store-path /home/ubuntu/certs/paa
```

Here is the interpretation of the parameters:

```
--paa-trust-store-path -> path to the generated PAA (der format)
```

`paa-trust-store-path` must contain only the PAA certificate. Avoid placing
other certificates in the same location as this may confuse `chip-tool`.

**PAA** certificate can be copied to the chip-tool machine using **SCP** for
example.

This is needed for testing self-generated **DACs**, but likely not required for
"true production" with production **PAI** issued **DACs**.

## 5. Useful information/Known issues

Implementation of manufacturing data provisioning has been validated using test
certificates generated by `OpenSSL 1.1.1l`.

Also, demo **DAC**, **PAI** and **PAA** certificates needed in case
`chip_with_factory_data=1` is used can be found in
`./scripts/tools/nxp/demo_generated_certs`.

## 6. Increased security for DAC private key
Supported platforms:
-   K32W1 - `src/plaftorm/nxp/k32w/k32w1/FactoryDataProviderImpl.h`

For platforms that have a secure subsystem (`SSS`), the DAC private key can be converted
to an encrypted blob. This blob will overwrite the DAC private key in factory data and
will be imported in the `SSS` at initialization, by the factory data provider instance.

The conversion process shall happen at manufacturing time and should be run one time only:
-   Write factory data binary.
-   Build the application with `chip_with_factory_data=1 chip_convert_dac_private_key=1` set.
-   Write the application to the board and let it run.

After the conversion process:
-   Make sure the application is built with `chip_with_factory_data=1`, but without
    `chip_convert_dac_private_key` arg, since conversion already happened.
-   Write the application to the board.

If you are using Jlink, you can see a conversion script example in:
```
./scripts/tools/nxp/factory_data_generator/k32w1/example_convert_dac_private_key.jlink
```

Factory data should now contain a corresponding encrypted blob instead of the DAC private key.

If an encrypted blob of the DAC private key is already available (e.g. obtained previously, using
other methods), then the conversion process shall be skipped. Instead, option `--dac_key_use_sss_blob`
can be used in the factory data generation command:

```
python3 ./scripts/tools/nxp/factory_data_generator/generate.py -i 10000 -s UXKLzwHdN3DZZLBaL2iVGhQi/OoQwIwJRQV4rpEalbA= -p 14014 -d 1000 --vid "0x$VID" --pid "0x$PID" --vendor_name "NXP Semiconductors" --product_name "Lighting app" --serial_num "12345678" --date "$DATE" --hw_version 1 --hw_version_str "1.0" --cert_declaration $FACTORY_DATA_DEST/Chip-Test-CD-$VID-$PID.der --dac_cert $FACTORY_DATA_DEST/Chip-DAC-NXP-$VID-$PID-Cert.der --dac_key $FACTORY_DATA_DEST/Chip-DAC-NXP-$VID-$PID-Key-encrypted-blob.bin --pai_cert $FACTORY_DATA_DEST/Chip-PAI-NXP-$VID-$PID-Cert.der --spake2p_path ./out/spake2p --unique_id "00112233445566778899aabbccddeeff" --dac_key_use_sss_blob --out $FACTORY_DATA_DEST/factory_data_with_blob.bin
```
Please note that `--dac_key` now points to a binary file that contains the encrypted blob.
