#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

get_filename_component(MATTER_MODULE_ROOT $ENV{ZEPHYR_BASE}/../modules/lib/matter REALPATH)

# Set Kconfig root files that will be processed as a first Kconfig for used child images.
set(mcuboot_KCONFIG_ROOT ${MATTER_MODULE_ROOT}/config/nrfconnect/chip-module/Kconfig.mcuboot.root)
set(multiprotocol_rpmsg_KCONFIG_ROOT ${MATTER_MODULE_ROOT}/config/nrfconnect/chip-module/Kconfig.multiprotocol_rpmsg.root)
set(hci_rpmsg_KCONFIG_ROOT ${MATTER_MODULE_ROOT}/config/nrfconnect/chip-module/Kconfig.hci_rpmsg.root)

# For prj.conf the CONF_FILE is empty. In other case extract the exact file name from the path string.
if(CONF_FILE)
    get_filename_component(CONFIG_FILE_NAME ${CONF_FILE} NAME)
endif()

if(CONFIG_FILE_NAME STREQUAL "prj_thread_wifi_switched.conf")
    set(PM_STATIC_YML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/configuration/${BOARD}/pm_static_thread_wifi_switched.yml)
elseif(NOT CONFIG_FILE_NAME STREQUAL "prj_no_dfu.conf")
    set(PM_STATIC_YML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/configuration/${BOARD}/pm_static_dfu.yml)
endif()

find_package(Zephyr HINTS $ENV{ZEPHYR_BASE})

project(matter-lock)

set(COMMON_ROOT ${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common)
set(NLIO_ROOT ${ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR}/third_party/nlio/repo)
include(${ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR}/config/nrfconnect/app/enable-gnu-std.cmake)
include(${ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR}/src/app/chip_data_model.cmake)

# NORDIC SDK APP START
target_include_directories(app PRIVATE
    src
    ${COMMON_ROOT}/src
    ${NLIO_ROOT}/include
    ${ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR}/zzz_generated/app-common
)

target_sources(app PRIVATE
    src/app_task.cpp
    src/bolt_lock_manager.cpp
    src/main.cpp
    src/zcl_callbacks.cpp
    src/zap-generated/callback-stub.cpp
    src/zap-generated/IMClusterCommandHandler.cpp
    ${COMMON_ROOT}/src/led_widget.cpp
)

if(CONFIG_CHIP_OTA_REQUESTOR OR CONFIG_MCUMGR_SMP_BT)
    target_sources(app PRIVATE ${COMMON_ROOT}/src/ota_util.cpp)
endif()

if(CONFIG_CHIP_NUS)
    target_sources(app PRIVATE ${COMMON_ROOT}/src/bt_nus_service.cpp)
endif()

if(CONFIG_MCUMGR_SMP_BT)
    target_sources(app PRIVATE ${COMMON_ROOT}/src/dfu_over_smp.cpp)
endif()

if(CONFIG_THREAD_WIFI_SWITCHING)
    target_sources(app PRIVATE
        src/software_images_swapper.cpp
        ${COMMON_ROOT}/src/ota_multi_image_processor_impl.cpp
        ${COMMON_ROOT}/src/ota_multi_image_downloader.cpp
    )
endif()

chip_configure_data_model(app
    INCLUDE_SERVER
    BYPASS_IDL
    GEN_DIR src/zap-generated
    ZAP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/lock.zap
)

if(CONFIG_THREAD_WIFI_SWITCHING)
    find_package(Python3)
    set(app_idx ${CONFIG_APPLICATION_IDX})
    add_custom_target(flash-external
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/src/flash-external.py
            --app ${APPLICATION_BINARY_DIR}/zephyr/app_update.bin
            --app-offset ${PM_APP_${app_idx}_CORE_APP_OFFSET}
            --app-size ${PM_APP_${app_idx}_CORE_APP_SIZE}
            --net ${APPLICATION_BINARY_DIR}/zephyr/net_core_app_update.bin
            --net-offset ${PM_APP_${app_idx}_CORE_NET_OFFSET}
            --net-size ${PM_APP_${app_idx}_CORE_NET_SIZE}
        USES_TERMINAL
        DEPENDS
            ${APPLICATION_BINARY_DIR}/zephyr/app_update.bin
            ${APPLICATION_BINARY_DIR}/zephyr/net_core_app_update.bin
    )
endif()
# NORDIC SDK APP END
