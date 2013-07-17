cmake_minimum_required(VERSION 2.8)

project(amf)

set(amf_SOURCES
    node.cpp
    actor.cpp
    message_queue.cpp)

include_directories(${UAU_INCLUDE_DIR}
    ${AMF_INCLUDE_DIR}
    ${AMF_SOURCE_DIR})

add_library(${PROJECT_NAME}
    ${amf_SOURCES})

# include directory content for convenience in editing in IDE
add_custom_target(_FOR_IDE_AMF_SOURCES ALL SOURCES
    "${AMF_SOURCE_DIR}/actor_p.h"
    "${AMF_SOURCE_DIR}/message_handler.h"
    "${AMF_SOURCE_DIR}/message_queue.h")
