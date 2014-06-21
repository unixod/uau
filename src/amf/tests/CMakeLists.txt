cmake_minimum_required(VERSION 2.8)

include(addTest)

include_directories(
    ${3RDPARTY_LIB_DIR}
    "${3RDPARTY_LIB_DIR}/Catch/include"
    ${LIBUAU_INCLUDE_DIR}
    ${LIBUAU_AMF_INCLUDE_DIR}
    ${LIBUAU_AMF_SOURCE_DIR})

addTest("uau::amf::MessageQueue" message_queue uau amf)
addTest("uau::amf::Actor" actor amf uau)
addTest("uau::amf::MessageHandler" message_handler amf)
addTest("uau::amf::core::Envelope" envelope uau)
