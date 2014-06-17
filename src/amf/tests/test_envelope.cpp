#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "handlerset.h"
#include "core/envelope.h"


namespace amf = uau::amf;
namespace core = amf::core;


SCENARIO("Casting of Envelop which holds non inheritable payload type") {
    GIVEN("Non inheritable payload type Msg, and Envelop constructed from it") {
        typedef int Msg;

        std::unique_ptr<const core::Envelope<>> pElp{new core::Envelope<Msg>{5}};

        WHEN("Try to cast to Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<Msg>(pElp.get()));
            }
        }

        WHEN("Try to cast to other non iheritable message type") {
            typedef char OtherMsg;

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("Try to cast to iheritable (non polymorphic) message type") {
            class OtherMsg {};

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("Try to cast to iheritable polymorphic message type") {
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

SCENARIO("Casting of Envelop which holds inheritable (non polymorphic) payload type") {
    GIVEN("Inheritable (not polymorphic) payload type Msg, and Envelop constructed from it") {
        class Msg {};

        std::unique_ptr<const core::Envelope<>> pElp{new core::Envelope<Msg>{}};

        WHEN("Try to cast to Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<Msg>(pElp.get()));
            }
        }

        WHEN("Try to cast to other non iheritable message type") {
            typedef char OtherMsg;

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("Try to cast to iheritable (non polymorphic) message type") {
            class OtherMsg {};

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("Try to cast to iheritable polymorphic message type") {
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

SCENARIO("Casting of Envelop which holds from inheritable polymorphic payload type") {
    GIVEN("Inheritable payload type  Msg, and Envelop constructed from it") {
        class BaseMsg {
        public:
            virtual ~BaseMsg() {}
        };

        class Msg : public BaseMsg {};

        std::unique_ptr<const core::Envelope<>> pElp{new core::Envelope<Msg>{}};

        WHEN("Try to cast to Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<Msg>(pElp.get()));
            }
        }

        WHEN("Try to cast to class of the Msg") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<BaseMsg>(pElp.get()));
            }
        }

        WHEN("Try to cast to other non iheritable message type") {
            typedef char OtherMsg;

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("Try to cast to iheritable (non polymorphic) message type") {
            class OtherMsg {};

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherMsg>(pElp.get()));
            }
        }

        WHEN("Try to cast to iheritable polymorphic message type") {
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

SCENARIO("Getting a payload") {
    GIVEN("Envelope of non-inheritable payload type") {
        typedef int Msg;
        Msg payload = 3;

        std::unique_ptr<const core::Envelope<>> pElp{
            new core::Envelope<Msg>(payload)
        };

        WHEN("Trying to get payload by value") {
            AND_WHEN("Assumption about payload type is met") {
                auto pPayload = pElp->payload<Msg>();

                THEN("Envelope::payload returns payload by value") {
                    REQUIRE(pPayload == payload);

                    AND_THEN("type qualifiers don't affect the matching") {
                        auto pPayload1 = pElp->payload<const Msg>();
                        auto pPayload2 = pElp->payload<volatile Msg>();
                        auto pPayload3 = pElp->payload<const volatile Msg>();

                        REQUIRE(pPayload1 == payload);
                        REQUIRE(pPayload2 == payload);
                        REQUIRE(pPayload3 == payload);
                    }
                }
            }

            AND_WHEN("Assumption about payload type is not met") {
                auto pPayload = pElp->payload<char>(111);

                THEN("Envelope::payload returns instance of expected type") {
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
                auto pPayload = pElp->payload<const Msg &>();

                THEN("Envelope::payload returns const reference to payload") {
                    REQUIRE(pPayload == payload);
                }
            }

            AND_WHEN("Assumption about payload type is not met") {
                THEN("Envelope::payload throws the std::bad_cast exception") {
                    REQUIRE_THROWS_AS(pElp->payload<const char &>(), std::bad_cast);
                }
            }
        }

        WHEN("Trying to get pointer to payload") {
            AND_WHEN("Assumption about payload type is met") {
                auto pPayload = pElp->payload<const Msg *>();

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

    GIVEN("Envelope of inheritable (non polymorphic) payload type") {
        struct Msg {
            int data;
        };
        Msg payload{3};

        std::unique_ptr<const core::Envelope<>> pElp{
            new core::Envelope<Msg>(payload)
        };

        WHEN("Trying to get reference to payload") {
            AND_WHEN("Assumption about payload type is met") {
                const Msg & refPayload = pElp->payload<const Msg &>();

                THEN("Envelope::payload returns const reference to payload") {
                    REQUIRE(refPayload.data == payload.data);
                }
            }

            AND_WHEN("Assumption about payload type is not met") {
                THEN("Envelope::payload throws the std::bad_cast exception") {
                    REQUIRE_THROWS_AS(pElp->payload<const char &>(), std::bad_cast);
                }
            }
        }

        WHEN("Trying to get pointer to payload") {
            AND_WHEN("Assumption about payload type is met") {
                auto pPayload = pElp->payload<const Msg *>();

                THEN("Envelope::payload returns valid pointer") {
                    REQUIRE(pPayload != nullptr);
                    REQUIRE(pPayload->data == payload.data);
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

    GIVEN("Envelope of inheritable (polymorphic) payload type") {
        class BaseMsg {
        public:
            BaseMsg(int data)
                : _data{data} {}

            virtual ~BaseMsg() {}

            int data() const {
                return _data;
            }

        private:
            int _data;
        };

        struct Msg : public BaseMsg {
            Msg(int data) :
                BaseMsg(data) {}
        };

        Msg payload{3};

        std::unique_ptr<const core::Envelope<>> pElp{
            new core::Envelope<Msg>(payload)
        };

        WHEN("Trying to get reference to payload") {
            AND_WHEN("Assumption about payload type is met") {
                const Msg & refPayload = pElp->payload<const Msg &>();
                const BaseMsg & refToBasePayload = pElp->payload<const BaseMsg&>();

                THEN("Envelope::payload returns const reference to payload") {
                    REQUIRE(refPayload.data() == payload.data());
                    REQUIRE(refToBasePayload.data() == payload.data());
                }
            }

            AND_WHEN("Assumption about payload type is not met") {
                THEN("Envelope::payload throws the std::bad_cast exception") {
                    REQUIRE_THROWS_AS(pElp->payload<const char &>(), std::bad_cast);
                }
            }
        }

        WHEN("Trying to get pointer to payload") {
            AND_WHEN("Assumption about payload type is met") {
                auto pPayload = pElp->payload<const Msg *>();
                auto pBasePayload = pElp->payload<const BaseMsg *>();

                THEN("Envelope::payload returns valid pointer") {
                    REQUIRE(pPayload != nullptr);
                    REQUIRE(pBasePayload != nullptr);
                    REQUIRE(pPayload->data() == payload.data());
                    REQUIRE(pBasePayload->data() == payload.data());
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

