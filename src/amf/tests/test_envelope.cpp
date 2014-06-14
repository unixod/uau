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

        WHEN("try to cast to Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<Msg>(pElp.get()));
            }
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

        WHEN("try to cast to Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<Msg>(pElp.get()));
            }
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

        WHEN("try to cast to Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<Msg>(pElp.get()));
            }
        }

        WHEN("try to cast to class of the Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<BaseMsg>(pElp.get()));
            }
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

SCENARIO("Trying to get valid payload") {
    GIVEN("Envelope which holds non-inheritable type, with trivial constructor") {
        int payload = 3;

        std::unique_ptr<const core::Envelope<>> pElp{new core::Envelope<int>(payload)};

        WHEN("Trying to cast to same type") {
            THEN("it's ok") {
                REQUIRE(pElp->payload<int>() == payload);
            }
        }


        WHEN("Trying to cast to other type") {
            THEN("result is default constructed type") {
                REQUIRE_FALSE(pElp->payload<char>() == payload);
            }
        }

        REQUIRE(pElp->payload<const int>() == payload);
        REQUIRE(pElp->payload<const volatile int>() == payload);
        REQUIRE(pElp->payload<const int &>() == payload);
//        REQUIRE(pElp->payload<int &>() == payload);
    }
}

