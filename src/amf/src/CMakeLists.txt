project(amf)
cmake_minimum_required(VERSION 2.8)

include_directories(
    ${LIBUAU_INCLUDE_DIR}
    ${LIBUAU_AMF_INCLUDE_DIR}
    "${LIBUAU_AMF_INCLUDE_DIR}/core"
    ${LIBUAU_AMF_SOURCE_DIR})

set(LIBUAU_AMF_SOURCES
    node.cpp
    actor.cpp
    actor_id.cpp
    message_queue.cpp
    "core/actors_storage.cpp"
    "core/main_router.cpp")

add_library(${PROJECT_NAME} ${LIBUAU_AMF_SOURCES})

showFilesInIDE(
    actor_p.h
    message_queue.h)
