# SPDX-License-Identifier: Apache-2.0

list(APPEND ZEPHYR_EXTRA_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/opt3001_module)

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(ootd2)
#add_subdirectory(drivers)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
