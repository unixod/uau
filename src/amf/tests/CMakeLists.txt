cmake_minimum_required(VERSION 2.8)

include(addTest)

include_directories(
    ${3RDPARTY_LIB_DIR}
    ${LIBUAU_INCLUDE_DIR}
    ${LIBUAU_AMF_INCLUDE_DIR}
    ${LIBUAU_AMF_SOURCE_DIR})

# FIXME move this find_package to uau part and remove ${CMAKE_THREAD_LIBS_INIT} from addTest invokation
find_package(Threads)
addTest("uau::amf::MessageQueue" message_queue amf ${CMAKE_THREAD_LIBS_INIT})

addTest("uau::amf::Actor" actor amf ${CMAKE_THREAD_LIBS_INIT})
addTest("uau::amf::MessageHandler" message_handler amf)

