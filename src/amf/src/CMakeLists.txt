cmake_minimum_required(VERSION 2.8)

project(amf)

set(amf_SOURCES
    actor.cpp
    message_queue.cpp
    node.cpp)

include_directories(${UAU_INCLUDE_DIR}
    ${AMF_INCLUDE_DIR})

add_library(${PROJECT_NAME}
    ${amf_SOURCES}
    ${amf_HEADERS})
