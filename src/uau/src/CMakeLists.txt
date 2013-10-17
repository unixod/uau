project(uau)
cmake_minimum_required(VERSION 2.8)

include_directories(
    ${LIBUAU_INCLUDE_DIR})

set(LIBUAU_SOURCES
    threadpool.cpp)

add_library(${PROJECT_NAME} ${LIBUAU_SOURCES})

find_package(Threads)
target_link_libraries(${PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT})

showFilesInIDE(
    threadpool_p.h
    "${LIBUAU_INCLUDE_DIR}/concurrency_algo.h"
    "${LIBUAU_INCLUDE_DIR}/handlerset.h"
    "${LIBUAU_INCLUDE_DIR}/rangeset.h"
    "${LIBUAU_INCLUDE_DIR}/threadpool.h"
    "${LIBUAU_INCLUDE_DIR}/typeset.h")
