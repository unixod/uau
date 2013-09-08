cmake_minimum_required(VERSION 2.8)

set(LIBUAU_AMF_INCLUDE_DIR
    "${LIBUAU_INCLUDE_DIR}/amf")

set(LIBUAU_AMF_SOURCE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/src")

add_subdirectory(src)

if(LIBUAU_BUILD_TESTS)
    add_subdirectory(tests)
endif()

showFilesInIDE(
    "${LIBUAU_AMF_INCLUDE_DIR}/actor.h"
    "${LIBUAU_AMF_INCLUDE_DIR}/actor_id.h"
    "${LIBUAU_AMF_INCLUDE_DIR}/dispatcher.h"
    "${LIBUAU_AMF_INCLUDE_DIR}/message.h"
    "${LIBUAU_AMF_INCLUDE_DIR}/node.h"
    "${LIBUAU_AMF_INCLUDE_DIR}/router.h"
    "${LIBUAU_AMF_INCLUDE_DIR}/core/actors_db.h"
    "${LIBUAU_AMF_INCLUDE_DIR}/core/actors_storage.h"
    "${LIBUAU_AMF_INCLUDE_DIR}/core/main_router.h")
