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

SCENARIO("Trying to get payload") {
    GIVEN("Envelope holds non-inheritable payload of type T") {
        int payload = 3;

        std::unique_ptr<const core::Envelope<>> pElp{
            new core::Envelope<int>(payload)
        };

        WHEN("Trying to get payload by value") {
            AND_WHEN("Assumption about payload type is met") {
                auto pPayload = pElp->payload<int>();

                THEN("Envelope::payload returns const reference to payload") {
                    REQUIRE(pPayload == payload);

                    AND_THEN("type qualifiers don't affect the matching") {
                        auto pPayload1 = pElp->payload<const int>();
                        auto pPayload2 = pElp->payload<volatile int>();
                        auto pPayload3 = pElp->payload<const volatile int>();

                        REQUIRE(pPayload1 == payload);
                        REQUIRE(pPayload2 == payload);
                        REQUIRE(pPayload3 == payload);
                    }
                }
            }

            AND_WHEN("Assumption about payload type is not met") {
                auto pPayload = pElp->payload<char>(111);

                THEN("Envelope::payload returns constructed assumed type") {
                    REQUIRE(pPayload == 111);

                    AND_THEN("type qualifiers don't affect the matching") {
                        auto pPayload1 = pElp->payload<const char>(123);
                        auto pPayload2 = pElp->payload<volatile char>(123);
                        auto pPayload3 = pElp->payload<const volatile char>(123);

                        REQUIRE(pPayload1 == 123);
                        REQUIRE(pPayload2 == 123);
                        REQUIRE(pPayload3 == 123);
                    }
                }
            }
        }

        WHEN("Trying to get reference to payload") {
            AND_WHEN("Assumption about payload type is met") {
                auto pPayload = pElp->payload<const int &>();

                THEN("Envelope::payload returns const reference to payload") {
                    REQUIRE(pPayload == payload);
                }
            }

            AND_WHEN("Assumption about payload type is not met") {
                auto pPayload = pElp->payload<const char &>(111);

                THEN("Envelope::payload returns reference to constructed assumed type") {
                    REQUIRE(pPayload == 111);
                }
            }
        }

        WHEN("Trying to get pointer to payload") {
            AND_WHEN("Assumption about payload type is met") {
                auto pPayload = pElp->payload<const int *>();

                THEN("Envelope::payload returns valid pointer") {
                    REQUIRE(pPayload != nullptr);
                    REQUIRE(*pPayload == payload);
                }
            }

            AND_WHEN("Assumption about payload type is not met") {
                auto pPayload = pElp->payload<const char *>();

                THEN("Envelope::payload returns nullptr") {
                    REQUIRE(pPayload == nullptr);
                }
            }
        }
    }
}

