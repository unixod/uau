#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <queue>
#include "actor.h"

namespace amf = uau::amf;
namespace core = amf::core;

SCENARIO("Retriving messages") {
    GIVEN("Actor, which subscribed to receive message of types T and U") {
        // Message types
        class T {};
        typedef int U;

        enum class MessageTypeTag {T, U};

        class SimpleActor : public amf::Actor {
        public:
            SimpleActor() {
                on<T>(&SimpleActor::handleTs);
                on<U>(&SimpleActor::handleUs, "some data", 5);
            }

            std::queue<MessageTypeTag> _invocationLog;

        private:
            void handleTs() {
                _invocationLog.push(MessageTypeTag::T);
            }

            void handleUs(const std::string &str, int k) {
                _invocationLog.push(MessageTypeTag::U);
            }
        };

        SimpleActor simpleActor;

        WHEN("There is no messages in actor's inbox") {
            amf::core::AbstractActor &actor = simpleActor;
            REQUIRE(simpleActor._invocationLog.size() == 0);

            THEN("Activation does not lead to anything") {
                actor.activate();

                REQUIRE(simpleActor._invocationLog.size() == 0);
            }
        }

        WHEN("There is only one message in actor's inbox") {
            amf::core::AbstractActor &actor = simpleActor;
            auto message = std::make_shared<const amf::core::Envelope<T>>();

            AND_WHEN("Message type is T") {
                actor.push({}, message);

                THEN("Activation of actor entails an invocation of subscribed function") {
                    FAIL();
                }
            }

            AND_WHEN("Message type is U") {
                THEN("Activation of actor entails an invocation of subscribed function") {
                    FAIL();
                }
            }

            AND_WHEN("Message type does not match not T nor U") {
                THEN("Activation does not lead to anything") {
                    FAIL();
                }
            }
        }

        WHEN("There are several messages in actor's inbox") {
            AND_WHEN("First of them has type T, second has type U") {
                THEN("Activation of actor entails an invocation only the function which subscribed to T") {
                    AND_THEN("After invocation, inbox remains only one message, which type is U") {
                        FAIL();
                    }
                }
            }
        }
    }
}
