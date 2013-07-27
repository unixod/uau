cmake_minimum_required(VERSION 2.8)

include_directories(
    ${LIBUAU_INCLUDE_DIR})

set(LIBUAU_SOURCES
    threadpool.cpp)

add_library(${PROJECT_NAME} ${LIBUAU_SOURCES})

showFilesInIDE(
    "${LIBUAU_SOURCE_DIR}/threadpool_p.h")
