#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <queue>
#include "actor.h"
#include "core/abstract_actor_id.h"

namespace amf = uau::amf;
namespace core = amf::core;

SCENARIO("Retriving messages") {
    GIVEN("Actor, which subscribed to receive message of types T and U") {
        // Message types
        class T {};
        typedef int U;
        class W {};

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
                REQUIRE_FALSE(actor.tryActivate());

                REQUIRE(simpleActor._invocationLog.size() == 0);

                AND_THEN("actor's outbox is empty") {
                    REQUIRE(actor.tryToPull() == nullptr);
                }
            }
        }

        WHEN("There is only one message in actor's inbox") {
            amf::core::AbstractActor &actor = simpleActor;

            AND_WHEN("Message type is T") {
                core::AbstractActor::Message message = std::make_shared<const amf::core::Envelope<T>>();
                REQUIRE(message->payload<const T *>() != nullptr);
                actor.push({""}, message);

                THEN("Activation of actor entails an invocation of subscribed function") {
                    REQUIRE(actor.tryActivate());

                    REQUIRE(simpleActor._invocationLog.size() == 1);
                    REQUIRE(simpleActor._invocationLog.front() == MessageTypeTag::T);

                    AND_THEN("actor's outbox is empty") {
                        REQUIRE(actor.tryToPull() == nullptr);
                    }
                }
            }

            AND_WHEN("Message type is U") {
                core::AbstractActor::Message message = std::make_shared<const amf::core::Envelope<U>>();
                REQUIRE(message->payload<const U *>() != nullptr);
                actor.push({""}, message);

                THEN("Activation of actor entails an invocation of subscribed function") {
                    REQUIRE(actor.tryActivate());

                    REQUIRE(simpleActor._invocationLog.size() == 1);
                    REQUIRE(simpleActor._invocationLog.front() == MessageTypeTag::U);

                    AND_THEN("actor's outbox is empty") {
                        REQUIRE(actor.tryToPull() == nullptr);
                    }
                }
            }

            AND_WHEN("Message type does not match not T nor U") {
                core::AbstractActor::Message message = std::make_shared<const amf::core::Envelope<W>>();
                REQUIRE(message->payload<const W *>() != nullptr);
                actor.push({""}, message);

                THEN("Activation does not lead to anything") {
                    REQUIRE(actor.tryActivate());

                    REQUIRE(simpleActor._invocationLog.size() == 0);

                    AND_THEN("actor's outbox is empty") {
                        REQUIRE(actor.tryToPull() == nullptr);
                    }
                }
            }
        }

        WHEN("There are several messages in actor's inbox") {
            amf::core::AbstractActor &actor = simpleActor;
            core::AbstractActor::Message tMsg = std::make_shared<const amf::core::Envelope<T>>();
            core::AbstractActor::Message uMsg = std::make_shared<const amf::core::Envelope<U>>();

            AND_WHEN("First of them has type T, second has type U") {
                REQUIRE(tMsg->payload<const T *>() != nullptr);
                actor.push({""}, tMsg);
                REQUIRE(uMsg->payload<const U *>() != nullptr);
                actor.push({""}, uMsg);

                THEN("Activation of actor entails an invocation only the function which subscribed to T") {
                    REQUIRE(actor.tryActivate());
                    REQUIRE(simpleActor._invocationLog.size() == 1);
                    REQUIRE(simpleActor._invocationLog.front() == MessageTypeTag::T);

                    AND_THEN("Subsequent invocations does not lead to anything, because there is no subscriptions in 'handleTs") {
                        simpleActor._invocationLog.pop(); // clean

                        REQUIRE(actor.tryActivate());
                        REQUIRE(simpleActor._invocationLog.size() == 0);
                    }
                }
            }
        }
    }
}
