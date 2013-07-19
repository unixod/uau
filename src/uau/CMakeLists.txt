cmake_minimum_required(VERSION 2.8)

#set(UAU_SOURCE_DIR
#    "${CMAKE_CURRENT_LIST_DIR}/src")

#add_subdirectory(src)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

#showHeadersInIDE(
#    "${AMF_INCLUDE_DIR}/actor.h"
#    "${AMF_INCLUDE_DIR}/dispatcher.h"
#    "${AMF_INCLUDE_DIR}/message.h"
#    "${AMF_INCLUDE_DIR}/node.h"
#    "${AMF_INCLUDE_DIR}/router.h"
#    "${AMF_INCLUDE_DIR}/core/actors_db.h"
#    "${AMF_INCLUDE_DIR}/core/actors_storage.h"
#    "${AMF_INCLUDE_DIR}/core/main_router.h")
