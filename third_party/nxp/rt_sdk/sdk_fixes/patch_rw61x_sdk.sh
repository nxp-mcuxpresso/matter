#!/bin/bash

patch_sdk()
{
    # apply fw_bin2c_conv.py patch
    patch -N ${NXP_SDK_ROOT}/components/conn_fwloader/script/fw_bin2c_conv.py <fw_bin2c_conv_fix.patch || :
    echo "MCUXpresso SDK folder \"${NXP_SDK_ROOT}\" has been patched!"
}


main()
{
    patch_sdk
}

main "$@"