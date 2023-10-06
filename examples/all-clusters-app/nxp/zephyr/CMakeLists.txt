#
# Copyright (c) 2023 Project CHIP Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
cmake_minimum_required(VERSION 3.13.1)

get_filename_component(CHIP_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/third_party/connectedhomeip REALPATH)
get_filename_component(GEN_DIR ${CHIP_ROOT}/zzz_generated/ REALPATH)
get_filename_component(ALL_CLUSTERS_COMMON_DIR ${CHIP_ROOT}/examples/all-clusters-app/all-clusters-common REALPATH)
get_filename_component(ALL_CLUSTERS_NXP_COMMON_DIR ${CHIP_ROOT}/examples/all-clusters-app/nxp/common REALPATH)

list(APPEND ZEPHYR_EXTRA_MODULES ${CHIP_ROOT}/config/nxp/chip-module)
find_package(Zephyr HINTS $ENV{ZEPHYR_BASE})

# -Wmaybe-uninitialized has too many false positives, including on std::optional
# and chip::Optional.  Make it nonfatal.
#
# See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=80635
target_compile_options(app PRIVATE -Werror -Wno-error=maybe-uninitialized)

project(chip-nxp-all-clusters-app-example)

include(${CHIP_ROOT}/config/nxp/app/enable-gnu-std.cmake)
include(${CHIP_ROOT}/src/app/chip_data_model.cmake)

target_include_directories(app
    PRIVATE
    ${ALL_CLUSTERS_NXP_COMMON_DIR}/main/include
    ${ALL_CLUSTERS_COMMON_DIR}/include
    ${GEN_DIR}/app-common
    ${GEN_DIR}/all-clusters-app
)

target_sources(app
    PRIVATE
    main/main.cpp
    main/AppTask.cpp
    ${ALL_CLUSTERS_NXP_COMMON_DIR}/main/CHIPDeviceManager.cpp
    ${ALL_CLUSTERS_NXP_COMMON_DIR}/main/DeviceCallbacks.cpp
    ${ALL_CLUSTERS_NXP_COMMON_DIR}/main/ZclCallbacks.cpp
    ${ALL_CLUSTERS_COMMON_DIR}/src/binding-handler.cpp
)

target_compile_definitions(app PUBLIC
    "EXTERNAL_FACTORY_DATA_PROVIDER_IMPL_HEADER=\"platform/nxp/zephyr/FactoryDataProviderImpl.h\""
)

if(CONFIG_CHIP_APP_DEVICE_TYPE_ALL_CLUSTERS)
    chip_configure_data_model(app
        INCLUDE_SERVER
        ZAP_FILE ${ALL_CLUSTERS_COMMON_DIR}/all-clusters-app.zap
    )

    target_sources(app
        PRIVATE
        ${ALL_CLUSTERS_COMMON_DIR}/src/smco-stub.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/static-supported-modes-manager.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/static-supported-temperature-levels.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/bridged-actions-stub.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/fan-stub.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/air-quality-instance.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/concentration-measurement-instances.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/resource-monitoring-delegates.cpp
    )
elseif(CONFIG_CHIP_APP_DEVICE_TYPE_LAUNDRY_WASHER)
    chip_configure_data_model(app
        INCLUDE_SERVER
        ZAP_FILE ${ALL_CLUSTERS_COMMON_DIR}/../../laundry-washer-app/nxp/zap/laundry-washer-app.zap
    )

    target_sources(app
        PRIVATE
        ${ALL_CLUSTERS_COMMON_DIR}/src/smco-stub.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/bridged-actions-stub.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/static-supported-modes-manager.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/static-supported-temperature-levels.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/laundry-washer-mode.cpp
        ${ALL_CLUSTERS_COMMON_DIR}/src/laundry-washer-controls-delegate-impl.cpp
        ${ALL_CLUSTERS_NXP_COMMON_DIR}/main/operational-state-delegate-impl.cpp
        ${ALL_CLUSTERS_NXP_COMMON_DIR}/main/ICDUtil.cpp
    )
elseif(CONFIG_CHIP_APP_DEVICE_TYPE_THERMOSTAT)
    chip_configure_data_model(app
        INCLUDE_SERVER
        ZAP_FILE ${ALL_CLUSTERS_COMMON_DIR}/../../thermostat/nxp/zap/thermostat_matter_wifi.zap
    )
else()
    message(FATAL_ERROR "CHIP_APP_DEVICE_TYPE not supported.")
endif()
