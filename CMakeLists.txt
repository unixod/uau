cmake_minimum_required(VERSION 2.8)

project(uau)

include(cmake/showHeadersInIDE.cmake)

#======================================================================#
# Configuration
#======================================================================#
#set(CMAKE_BUILD_TYPE "Release")

#TEMPORARY HACK!!! (cmake stil not yet support switching to C++11)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
endif()

if(WITH_TESTS)
    enable_testing()
endif()

#======================================================================#
set(UAU_INCLUDE_DIR
    "${CMAKE_SOURCE_DIR}/uau")

set(3RDPARTY_LIB_DIR
    "${CMAKE_SOURCE_DIR}/lib")

add_subdirectory(src)

showHeadersInIDE(
    "${UAU_INCLUDE_DIR}/concurrency_algo.h"
    "${UAU_INCLUDE_DIR}/rangeset.h"
    "${UAU_INCLUDE_DIR}/threadpool.h")
