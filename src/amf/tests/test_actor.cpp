#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "actor.h"

namespace amf = uau::amf;
namespace core = amf::core;

SCENARIO("Retriving messages") {
    GIVEN("Actor, which subscribed to receive message of types T and U") {
        class SimpleActor : public amf::Actor {
        public:
            class T {};
            typedef int U;

        public:
            SimpleActor() {
                on<T>(&SimpleActor::handleTs);
                on<U>(&SimpleActor::handleUs, "some data", 5);
            }

        private:
            void handleTs() {
            }

            void handleUs(const std::string &str, int k) {
            }

        private:

        };

        SimpleActor simpleActor;

        WHEN("There is no messages in actor's inbox") {
            THEN("Activation does not lead to anything") {
                amf::core::AbstractActor &actor = simpleActor;
                actor.activate();

            }
        }

        WHEN("There is only one message in actor's inbox") {
            AND_WHEN("Message type is T") {
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
