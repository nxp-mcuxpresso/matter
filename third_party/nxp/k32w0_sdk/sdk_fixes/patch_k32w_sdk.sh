#!/bin/bash

if [[ ! -d $NXP_K32W0_SDK_ROOT ]]; then
    echo "NXP_K32W0_SDK_ROOT is not set"
    exit 1
fi

board=$(ls "$NXP_K32W0_SDK_ROOT"/boards)

convert_to_dos() {

    [[ $(file -b - <$1) != *"CRLF"* ]] && sed -i 's/$/\r/' "$1"
}

SOURCE=${BASH_SOURCE[0]}
SOURCE_DIR=$(cd "$(dirname "$SOURCE")" >/dev/null 2>&1 && pwd)

convert_to_dos "$NXP_K32W0_SDK_ROOT"/middleware/wireless/framework/OSAbstraction/Source/fsl_os_abstraction_free_rtos.c
convert_to_dos "$NXP_K32W0_SDK_ROOT"/middleware/wireless/framework/OSAbstraction/Source/fsl_os_abstraction_bm.c
convert_to_dos "$NXP_K32W0_SDK_ROOT"/middleware/wireless/framework/OSAbstraction/Interface/fsl_os_abstraction.h
convert_to_dos "$NXP_K32W0_SDK_ROOT"/middleware/wireless/framework/SecLib/SecLib.c
convert_to_dos "$NXP_K32W0_SDK_ROOT"/middleware/wireless/framework/SecLib/SecLib.h
convert_to_dos "$NXP_K32W0_SDK_ROOT"/middleware/wireless/framework/OtaSupport/Interface/OtaUtils.h
convert_to_dos "$NXP_K32W0_SDK_ROOT"/middleware/mbedtls/port/ksdk/sha256_alt.h

patch -N --binary -d "$NXP_K32W0_SDK_ROOT"/middleware/wireless/framework/OSAbstraction/ -p1 <"$SOURCE_DIR/fsl_os_abstraction.patch"
patch -N --binary -d "$NXP_K32W0_SDK_ROOT"/middleware/wireless/framework/SecLib/ -p1 <"$SOURCE_DIR/SecLib.patch"
patch -N --binary -d "$NXP_K32W0_SDK_ROOT"/middleware/wireless/framework/OtaSupport/Interface/ -p1 <"$SOURCE_DIR/OtaUtils_h.patch"
patch -N --binary -d "$NXP_K32W0_SDK_ROOT"/middleware/mbedtls/port/ksdk/ -p1 <"$SOURCE_DIR/sha256_alt.patch"

echo "K32W SDK 2.6.7 was patched!"
exit 0
