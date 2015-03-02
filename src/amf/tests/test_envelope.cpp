#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "uau/handlerset.h"
#include "uau/amf/core/envelope.h"


namespace amf = uau::amf;
namespace core = amf::core;


SCENARIO("Casting an Envelop of non-inheritable payload type") {
    GIVEN("Envelope of non-inheritable PayloadType") {
        typedef int PayloadType;
        PayloadType payload = 5;

        std::unique_ptr<const core::Envelope<>> pElp{
            new core::Envelope<PayloadType>{payload}
        };

        WHEN("Trying to cast to PayloadType") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<PayloadType>(pElp.get()));
            }
        }

        WHEN("Trying to cast to non-inheritable OtherPayloadType") {
            typedef char OtherPayloadType;

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherPayloadType>(pElp.get()));
            }
        }

        WHEN("Trying to cast to inheritable (non-polymorphic) OtherPayloadType") {
            class OtherPayloadType {};

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherPayloadType>(pElp.get()));
            }
        }

        WHEN("Trying to cast to inheritable (polymorphic) OtherPayloadType") {
            class OtherPayloadType {
            public:
                virtual ~OtherPayloadType(){}
            };

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherPayloadType>(pElp.get()));
            }
        }
    }
}

SCENARIO("Casting an Envelop of inheritable payload (non-polymorphic) type") {
    GIVEN("Envelope of inheritable (non-polymorphic) PayloadType") {
        class PayloadType {};

        std::unique_ptr<const core::Envelope<>> pElp{
            new core::Envelope<PayloadType>{}
        };

        WHEN("Trying to cast to PayloadType") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<PayloadType>(pElp.get()));
            }
        }

        WHEN("Trying to cast to non-inheritable OtherPayloadType") {
            typedef char OtherPayloadType;

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherPayloadType>(pElp.get()));
            }
        }

        WHEN("Trying to cast to inheritable (non-polymorphic) OtherPayloadType") {
            class OtherPayloadType {};

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherPayloadType>(pElp.get()));
            }
        }

        WHEN("Trying to cast to inheritable (polymorphic) OtherPayloadType") {
            class OtherPayloadType {
            public:
                virtual ~OtherPayloadType(){}
            };

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherPayloadType>(pElp.get()));
            }
        }
    }
}

SCENARIO("Casting an Envelop of inheritable payload (polymorphic) type") {
    GIVEN("Envelope of inheritable (polymorphic) PayloadType") {
        class BasePayloadType {
        public:
            virtual ~BasePayloadType() {}
        };

        class PayloadType : public BasePayloadType {};

        std::unique_ptr<const core::Envelope<>> pElp{
            new core::Envelope<PayloadType>{}
        };

        WHEN("Trying to cast to PayloadType") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<PayloadType>(pElp.get()));
            }
        }

        WHEN("Trying to cast to base class of PayloadType") {
            THEN("uau::handlerSetMatcher returns true") {
                REQUIRE(uau::handlerSetMatcher<BasePayloadType>(pElp.get()));
            }
        }

        WHEN("Trying to cast to non-inheritable OtherPayloadType") {
            typedef char OtherPayloadType;

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherPayloadType>(pElp.get()));
            }
        }

        WHEN("Trying to cast to inheritable (non-polymorphic) OtherPayloadType") {
            class OtherPayloadType {};

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherPayloadType>(pElp.get()));
            }
        }

        WHEN("Trying to cast to inheritable (polymorphic) OtherPayloadType") {
            class OtherPayloadType {
            public:
                virtual ~OtherPayloadType(){}
            };

            THEN("uau::handlerSetMatcher returns false") {
                REQUIRE_FALSE(uau::handlerSetMatcher<OtherPayloadType>(pElp.get()));
            }
        }
    }
}

SCENARIO("Getting a payload") {
    GIVEN("Envelope of non-inheritable PayloadType") {
        typedef int PayloadType;
        PayloadType payload = 3;

        std::unique_ptr<const core::Envelope<>> pElp{
            new core::Envelope<PayloadType>(payload)
        };

        WHEN("Trying to get payload by value") {
            AND_WHEN("Assumption about payload type is met") {
                auto pPayload = pElp->payload<PayloadType>();

                THEN("Envelope::payload returns payload by value") {
                    REQUIRE(pPayload == payload);

                    AND_THEN("type qualifiers don't affect the matching") {
                        auto pPayload1 = pElp->payload<const PayloadType>();
                        auto pPayload2 = pElp->payload<volatile PayloadType>();
                        auto pPayload3 = pElp->payload<const volatile PayloadType>();

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
                auto pPayload = pElp->payload<const PayloadType &>();

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
                auto pPayload = pElp->payload<const PayloadType *>();

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

    GIVEN("Envelope of inheritable (non-polymorphic) PayloadType") {
        struct PayloadType {
            int data;
        };
        PayloadType payload{3};

        std::unique_ptr<const core::Envelope<>> pElp{
            new core::Envelope<PayloadType>(payload)
        };

        WHEN("Trying to get reference to payload") {
            AND_WHEN("Assumption about payload type is met") {
                const PayloadType & refPayload = pElp->payload<const PayloadType &>();

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
                auto pPayload = pElp->payload<const PayloadType *>();

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

    GIVEN("Envelope of inheritable (polymorphic) PayloadType") {
        class BasePayloadType {
        public:
            BasePayloadType(int data)
                : _data{data} {}

            virtual ~BasePayloadType() {}

            int data() const {
                return _data;
            }

        private:
            int _data;
        };

        struct PayloadType : public BasePayloadType {
            PayloadType(int data) :
                BasePayloadType(data) {}
        };

        PayloadType payload{3};

        std::unique_ptr<const core::Envelope<>> pElp{
            new core::Envelope<PayloadType>(payload)
        };

        WHEN("Trying to get reference to payload") {
            AND_WHEN("Assumption about payload type is met") {
                const PayloadType & refPayload = pElp->payload<const PayloadType &>();
                const BasePayloadType & refToBasePayload = pElp->payload<const BasePayloadType&>();

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
                auto pPayload = pElp->payload<const PayloadType *>();
                auto pBasePayload = pElp->payload<const BasePayloadType *>();

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

