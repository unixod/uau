cmake_minimum_required(VERSION 2.8)

include_directories(STATUS ${3RDPARTY_LIB_DIR}
    ${UAU_INCLUDE_DIR}
    ${AMF_INCLUDE_DIR}
    ${AMF_SOURCE_DIR})

# FIXME move find_package to uau part
find_package(Threads)

set(TEST_NAME "uau::amf::MessageQueue")
add_executable(message_queue.test test_message_queue.cpp)
# FIXME remove ${CMAKE_THREAD_LIBS_INIT} after moving find_package
target_link_libraries(message_queue.test amf ${CMAKE_THREAD_LIBS_INIT})
add_test("${TEST_NAME}" message_queue.test)
