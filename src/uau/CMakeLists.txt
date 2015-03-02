cmake_minimum_required(VERSION 2.8)

set(LIBUAU_SOURCE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/src")

add_subdirectory(src)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

