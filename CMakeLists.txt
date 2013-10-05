cmake_minimum_required(VERSION 2.8)

#===============================================================================
# CMake configuration
#===============================================================================
#set(CMAKE_BUILD_TYPE "Release")

option(LIBUAU_BUILD_TESTS  "Enable testing" OFF)

if(LIBUAU_BUILD_TESTS)
    enable_testing()
endif()

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#===============================================================================
# Setup compiler flags
#===============================================================================
include(configCXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LIBUAU_WALL_FLAG} ${LIBUAU_CXX11_FLAG}")

#===============================================================================
# Setup project
#===============================================================================
include(showFilesInIDE)

set(LIBUAU_INCLUDE_DIR
    "${CMAKE_SOURCE_DIR}/include")

set(3RDPARTY_LIB_DIR
    "${CMAKE_SOURCE_DIR}/libs")

add_subdirectory(src)

showFilesInIDE(
    "${LIBUAU_INCLUDE_DIR}/concurrency_algo.h"
    "${LIBUAU_INCLUDE_DIR}/handlerset.h"
    "${LIBUAU_INCLUDE_DIR}/rangeset.h"
    "${LIBUAU_INCLUDE_DIR}/threadpool.h"
    "${LIBUAU_INCLUDE_DIR}/typeset.h")
