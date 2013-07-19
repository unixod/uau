project(uau)
cmake_minimum_required(VERSION 2.8)

#======================================================================#
# Configuration
#======================================================================#
#set(CMAKE_BUILD_TYPE "Release")

#TEMPORARY HACK!!! (cmake stil not yet support switching to C++11)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
endif()

option(UAU_BUILD_TESTS  "Enable testing" OFF)
if(UAU_BUILD_TESTS)
    enable_testing()
endif()

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#======================================================================#

include(showHeadersInIDE)

set(UAU_INCLUDE_DIR
    "${CMAKE_SOURCE_DIR}/uau")

set(3RDPARTY_LIB_DIR
    "${CMAKE_SOURCE_DIR}/lib")

add_subdirectory(src)

showHeadersInIDE(
    "${UAU_INCLUDE_DIR}/concurrency_algo.h"
    "${UAU_INCLUDE_DIR}/rangeset.h"
    "${UAU_INCLUDE_DIR}/threadpool.h"
    "${UAU_INCLUDE_DIR}/typeset.h")
