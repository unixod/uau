#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "actor.h"

namespace amf = uau::amf;
namespace core = amf::core;

SCENARIO("Retriving messages") {
    GIVEN("Actor, which subscribed to receive message of type T and U") {
        WHEN("There is no messages in actor's inbox") {
            THEN("Activation does not lead to anything") {
            }
        }

        WHEN("There is only one message in actor's inbox") {
            AND_WHEN("Message type is T") {
                THEN("Activation of actor entails an invocation of subscribed function") {
                }
            }

            AND_WHEN("Message type is U") {
                THEN("Activation of actor entails an invocation of subscribed function") {
                }
            }

            AND_WHEN("Message type does not match not T nor U") {
                THEN("Activation does not lead to anything") {
                }
            }
        }

        WHEN("There are several messages in actor's inbox") {
            AND_WHEN("First of them has type T, second has type U") {
                THEN("Activation of actor entails an invocation only the function which subscribed to T") {
                    AND_THEN("After invocation, inbox remains only one message, which type is U") {

                    }
                }
            }
        }
    }
}
