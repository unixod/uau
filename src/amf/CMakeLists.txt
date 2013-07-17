cmake_minimum_required(VERSION 2.8)

set(AMF_INCLUDE_DIR
    "${UAU_INCLUDE_DIR}/amf")

set(AMF_SOURCE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/src")

add_subdirectory(src)

if(WITH_TESTS)
    add_subdirectory(tests)
    endif()

# include directory content for convenience in editing in IDE
add_custom_target(_FOR_IDE_AMF_INCLUDES ALL SOURCES
    "${AMF_INCLUDE_DIR}/actor.h"
    "${AMF_INCLUDE_DIR}/dispatcher.h"
    "${AMF_INCLUDE_DIR}/message.h"
    "${AMF_INCLUDE_DIR}/node.h"
    "${AMF_INCLUDE_DIR}/router.h")
