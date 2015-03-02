project(amf)
cmake_minimum_required(VERSION 2.8)

include_directories(
    ${LIBUAU_INCLUDE_DIR}
    ${LIBUAU_AMF_SOURCE_DIR})

set(LIBUAU_AMF_SOURCES
    node.cpp
    actor.cpp
    message_queue.cpp
    core/abstract_actor_id.cpp
    core/master_actor.cpp)

add_library(${PROJECT_NAME} ${LIBUAU_AMF_SOURCES})

showFilesInIDE(
    actor_p.h
    message_queue.h
    "${LIBUAU_INCLUDE_DIR}/uau/amf/actor.h"
    "${LIBUAU_INCLUDE_DIR}/uau/amf/dispatcher.h"
    "${LIBUAU_INCLUDE_DIR}/uau/amf/core/abstract_actor.h"
    "${LIBUAU_INCLUDE_DIR}/uau/amf/core/abstract_actor_id.h"
    "${LIBUAU_INCLUDE_DIR}/uau/amf/core/envelope.h"
    "${LIBUAU_INCLUDE_DIR}/uau/amf/core/envelope_fwd.h"
    "${LIBUAU_INCLUDE_DIR}/uau/amf/core/master_actor.h"
    "${LIBUAU_INCLUDE_DIR}/uau/amf/core/messages/control_message.h"
    "${LIBUAU_INCLUDE_DIR}/uau/amf/core/messages/delete.h"
    "${LIBUAU_INCLUDE_DIR}/uau/amf/core/messages/system_message.h"
    "${LIBUAU_INCLUDE_DIR}/uau/amf/node.h")
