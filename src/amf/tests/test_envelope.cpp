#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "handlerset.h"
#include "core/envelope.h"


namespace amf = uau::amf;
namespace core = amf::core;


SCENARIO("Casting of Envelop which constructs from non inheritable message type") {
    GIVEN("Non inheritable message class Msg, and Envelop constructed from it") {
        typedef int Msg;

        std::unique_ptr<const core::Envelope<>> pElp{new core::Envelope<Msg>{5}};
        std::unique_ptr<const core::Envelope<>> pElpConstMsg{new core::Envelope<const Msg>{5}};
        std::unique_ptr<const core::Envelope<>> pElpVolatileMsg{new core::Envelope<volatile Msg>{5}};
        std::unique_ptr<const core::Envelope<>> pElpCVMsg{new core::Envelope<const volatile Msg>{5}};

        WHEN("try to cast to Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<Msg>(pElp.get()));
                REQUIRE(uau::handlerSetMatcher<const Msg>(pElpConstMsg.get()));
                REQUIRE(uau::handlerSetMatcher<volatile Msg>(pElpVolatileMsg.get()));
                REQUIRE(uau::handlerSetMatcher<const volatile Msg>(pElpCVMsg.get()));
            }
        }

        WHEN("try to cast to Msg with different qualifiers") {
            REQUIRE_FALSE(uau::handlerSetMatcher<const Msg>(pElp.get()));
            REQUIRE_FALSE(uau::handlerSetMatcher<volatile Msg>(pElp.get()));
            REQUIRE_FALSE(uau::handlerSetMatcher<const volatile Msg>(pElp.get()));

            REQUIRE_FALSE(uau::handlerSetMatcher<Msg>(pElpConstMsg.get()));
            REQUIRE_FALSE(uau::handlerSetMatcher<volatile Msg>(pElpConstMsg.get()));
            REQUIRE_FALSE(uau::handlerSetMatcher<const volatile Msg>(pElpConstMsg.get()));

            REQUIRE_FALSE(uau::handlerSetMatcher<const Msg>(pElpVolatileMsg.get()));
            REQUIRE_FALSE(uau::handlerSetMatcher<Msg>(pElpVolatileMsg.get()));
            REQUIRE_FALSE(uau::handlerSetMatcher<const volatile Msg>(pElpVolatileMsg.get()));

            REQUIRE_FALSE(uau::handlerSetMatcher<const Msg>(pElpCVMsg.get()));
            REQUIRE_FALSE(uau::handlerSetMatcher<volatile Msg>(pElpCVMsg.get()));
            REQUIRE_FALSE(uau::handlerSetMatcher<Msg>(pElpCVMsg.get()));
        }

        WHEN("try to cast to other non iheritable message type") {
            typedef char OtherMsg;

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("try to cast to iheritable (non polymorphic) message type") {
            class OtherMsg {};

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("try to cast to iheritable polymorphic message type") {
            class OtherMsg {
            public:
                virtual ~OtherMsg(){}
            };

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }
    }
}

SCENARIO("Casting of Envelop which constructs from inheritable (non polymorphic) message type") {
    GIVEN("Non inheritable message class Msg, and Envelop constructed from it") {
        class Msg {};

        std::unique_ptr<const core::Envelope<>> pElp{new core::Envelope<Msg>{}};
        std::unique_ptr<const core::Envelope<>> pElpConstMsg{new core::Envelope<const Msg>{}};
        std::unique_ptr<const core::Envelope<>> pElpVolatileMsg{new core::Envelope<volatile Msg>{}};
        std::unique_ptr<const core::Envelope<>> pElpCVMsg{new core::Envelope<const volatile Msg>{}};

        WHEN("try to cast to Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<Msg>(pElp.get()));
                REQUIRE(uau::handlerSetMatcher<const Msg>(pElpConstMsg.get()));
                REQUIRE(uau::handlerSetMatcher<volatile Msg>(pElpVolatileMsg.get()));
                REQUIRE(uau::handlerSetMatcher<const volatile Msg>(pElpCVMsg.get()));
            }
        }

        WHEN("try to cast to Msg with different qualifiers") {
            REQUIRE(uau::handlerSetMatcher<const Msg>(pElp.get()));
            REQUIRE(uau::handlerSetMatcher<volatile Msg>(pElp.get()));
            REQUIRE(uau::handlerSetMatcher<const volatile Msg>(pElp.get()));

            REQUIRE(uau::handlerSetMatcher<Msg>(pElpConstMsg.get()));
            REQUIRE(uau::handlerSetMatcher<volatile Msg>(pElpConstMsg.get()));
            REQUIRE(uau::handlerSetMatcher<const volatile Msg>(pElpConstMsg.get()));

            REQUIRE(uau::handlerSetMatcher<const Msg>(pElpVolatileMsg.get()));
            REQUIRE(uau::handlerSetMatcher<Msg>(pElpVolatileMsg.get()));
            REQUIRE(uau::handlerSetMatcher<const volatile Msg>(pElpVolatileMsg.get()));

            REQUIRE(uau::handlerSetMatcher<const Msg>(pElpCVMsg.get()));
            REQUIRE(uau::handlerSetMatcher<volatile Msg>(pElpCVMsg.get()));
            REQUIRE(uau::handlerSetMatcher<Msg>(pElpCVMsg.get()));
        }

        WHEN("try to cast to other non iheritable message type") {
            typedef char OtherMsg;

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("try to cast to iheritable (non polymorphic) message type") {
            class OtherMsg {};

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("try to cast to iheritable polymorphic message type") {
            class OtherMsg {
            public:
                virtual ~OtherMsg(){}
            };

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }
    }
}

SCENARIO("Casting of Envelop which constructs from inheritable polymorphic message type") {
    GIVEN("Non inheritable message class Msg, and Envelop constructed from it") {
        class BaseMsg {
        public:
            virtual ~BaseMsg() {}
        };

        class Msg : public BaseMsg {};

        std::unique_ptr<const core::Envelope<>> pElp{new core::Envelope<Msg>{}};
        std::unique_ptr<const core::Envelope<>> pElpConstMsg{new core::Envelope<const Msg>{}};
        std::unique_ptr<const core::Envelope<>> pElpVolatileMsg{new core::Envelope<volatile Msg>{}};
        std::unique_ptr<const core::Envelope<>> pElpCVMsg{new core::Envelope<const volatile Msg>{}};

        WHEN("try to cast to Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<Msg>(pElp.get()));
                REQUIRE(uau::handlerSetMatcher<const Msg>(pElpConstMsg.get()));
                REQUIRE(uau::handlerSetMatcher<volatile Msg>(pElpVolatileMsg.get()));
                REQUIRE(uau::handlerSetMatcher<const volatile Msg>(pElpCVMsg.get()));
            }
        }

        WHEN("try to cast to class of the Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<BaseMsg>(pElp.get()));
                REQUIRE(uau::handlerSetMatcher<const BaseMsg>(pElpConstMsg.get()));
                REQUIRE(uau::handlerSetMatcher<volatile BaseMsg>(pElpVolatileMsg.get()));
                REQUIRE(uau::handlerSetMatcher<const volatile BaseMsg>(pElpCVMsg.get()));
            }
        }

        WHEN("try to cast to Msg with different qualifiers") {
            REQUIRE(uau::handlerSetMatcher<const Msg>(pElp.get()));
            REQUIRE(uau::handlerSetMatcher<volatile Msg>(pElp.get()));
            REQUIRE(uau::handlerSetMatcher<const volatile Msg>(pElp.get()));

            REQUIRE(uau::handlerSetMatcher<Msg>(pElpConstMsg.get()));
            REQUIRE(uau::handlerSetMatcher<volatile Msg>(pElpConstMsg.get()));
            REQUIRE(uau::handlerSetMatcher<const volatile Msg>(pElpConstMsg.get()));

            REQUIRE(uau::handlerSetMatcher<const Msg>(pElpVolatileMsg.get()));
            REQUIRE(uau::handlerSetMatcher<Msg>(pElpVolatileMsg.get()));
            REQUIRE(uau::handlerSetMatcher<const volatile Msg>(pElpVolatileMsg.get()));

            REQUIRE(uau::handlerSetMatcher<const Msg>(pElpCVMsg.get()));
            REQUIRE(uau::handlerSetMatcher<volatile Msg>(pElpCVMsg.get()));
            REQUIRE(uau::handlerSetMatcher<Msg>(pElpCVMsg.get()));
        }

        WHEN("try to cast to base class of Msg with different qualifiers") {
            REQUIRE(uau::handlerSetMatcher<const BaseMsg>(pElp.get()));
            REQUIRE(uau::handlerSetMatcher<volatile BaseMsg>(pElp.get()));
            REQUIRE(uau::handlerSetMatcher<const volatile BaseMsg>(pElp.get()));

            REQUIRE(uau::handlerSetMatcher<BaseMsg>(pElpConstMsg.get()));
            REQUIRE(uau::handlerSetMatcher<volatile BaseMsg>(pElpConstMsg.get()));
            REQUIRE(uau::handlerSetMatcher<const volatile BaseMsg>(pElpConstMsg.get()));

            REQUIRE(uau::handlerSetMatcher<const BaseMsg>(pElpVolatileMsg.get()));
            REQUIRE(uau::handlerSetMatcher<BaseMsg>(pElpVolatileMsg.get()));
            REQUIRE(uau::handlerSetMatcher<const volatile BaseMsg>(pElpVolatileMsg.get()));

            REQUIRE(uau::handlerSetMatcher<const BaseMsg>(pElpCVMsg.get()));
            REQUIRE(uau::handlerSetMatcher<volatile BaseMsg>(pElpCVMsg.get()));
            REQUIRE(uau::handlerSetMatcher<BaseMsg>(pElpCVMsg.get()));
        }

        WHEN("try to cast to other non iheritable message type") {
            typedef char OtherMsg;

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("try to cast to iheritable (non polymorphic) message type") {
            class OtherMsg {};

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("try to cast to iheritable polymorphic message type") {
            class OtherMsg {
            public:
                virtual ~OtherMsg(){}
            };

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }
    }
}

