#!/bin/bash
# Important: these patches work for the version of sdk indicated by the west.yml file, which need to be copied in github_sdk/rt/manifest

NXP_RT1060_SDK_ROOT="../../github_sdk/rt/repo/sdk-2.15"
SOURCE=${BASH_SOURCE[0]}
SOURCE_DIR=$(cd "$(dirname "$SOURCE")" >/dev/null 2>&1 && pwd)

patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/boards/evkbmimxrt1060/wireless_config_template/ -p1 <"$SOURCE_DIR/wifi_bt_config_c.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/boards/evkbmimxrt1060/wireless_config_template/ -p1 <"$SOURCE_DIR/wifi_bt_config_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/boards/evkbmimxrt1060/wireless_config_template/ -p1 <"$SOURCE_DIR/sdmmc_config_c.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/boards/evkbmimxrt1060/wireless_config_template/ -p1 <"$SOURCE_DIR/sdmmc_config_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/boards/evkbmimxrt1060/xip/ -p1 <"$SOURCE_DIR/evkbmimxrt1060_flexspi_nor_config_c.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/boards/evkbmimxrt1060/xip/ -p1 <"$SOURCE_DIR/evkbmimxrt1060_flexspi_nor_config_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/components/gpio/ -p1 <"$SOURCE_DIR/fsl_adapter_gpio_c.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/components/osa/ -p1 <"$SOURCE_DIR/fsl_os_abstraction_free_rtos_c.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/devices/MIMXRT1062/ -p1 <"$SOURCE_DIR/MIMXRT1062_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/devices/MIMXRT1062/ -p1 <"$SOURCE_DIR/MIMXRT1062_features_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/devices/MIMXRT1062/ -p1 <"$SOURCE_DIR/fsl_device_registers_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/devices/MIMXRT1062/ -p1 <"$SOURCE_DIR/system_MIMXRT1062_c.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/devices/MIMXRT1062/xip/ -p1 <"$SOURCE_DIR/fsl_flexspi_nor_boot_c.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/devices/MIMXRT1062/xip/ -p1 <"$SOURCE_DIR/fsl_flexspi_nor_boot_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/drivers/gpio/ -p1 <"$SOURCE_DIR/fsl_gpio_c.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/drivers/gpio/ -p1 <"$SOURCE_DIR/fsl_gpio_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/middleware/wireless/framework/FileSystem/ -p1 <"$SOURCE_DIR/fwk_filesystem_c.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/middleware/wifi_nxp/wifi_bt_firmware/ -p1 <"$SOURCE_DIR/sduartIW416_wlan_bt_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/middleware/wireless/framework/boards/rt1060/ -p1 <"$SOURCE_DIR/board_comp_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/middleware/littlefs/ -p1 <"$SOURCE_DIR/lfs_c.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/middleware/littlefs/ -p1 <"$SOURCE_DIR/lfs_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/middleware/littlefs/ -p1 <"$SOURCE_DIR/lfs_util_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/CMSIS/Core/Include/ -p1 <"$SOURCE_DIR/cmsis_armcc_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/CMSIS/Core/Include/ -p1 <"$SOURCE_DIR/cmsis_armclang_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/CMSIS/Core/Include/ -p1 <"$SOURCE_DIR/cmsis_armclang_ltm_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/CMSIS/Core/Include/ -p1 <"$SOURCE_DIR/cmsis_gcc_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/CMSIS/Core/Include/ -p1 <"$SOURCE_DIR/cmsis_iccarm_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/CMSIS/Core/Include/ -p1 <"$SOURCE_DIR/cmsis_version_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/CMSIS/Core/Include/ -p1 <"$SOURCE_DIR/core_cm7_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/CMSIS/Core/Include/ -p1 <"$SOURCE_DIR/mpu_armv7_h.patch"
patch -N --binary -d "$NXP_RT1060_SDK_ROOT"/core/utilities/debug_console/ -p1 <"$SOURCE_DIR/fsl_debug_console_c.patch"
echo "RT1060 SDK 2.15 patched!"

exit 0
