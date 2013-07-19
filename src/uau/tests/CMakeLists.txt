cmake_minimum_required(VERSION 2.8)

include(addTest)

include_directories(
    ${3RDPARTY_LIB_DIR}
    ${LIBUAU_INCLUDE_DIR})

addTest("uau::TypeSet" typeset)
