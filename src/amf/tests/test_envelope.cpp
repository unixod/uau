#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "handlerset.h"
#include "core/envelope.h"


namespace amf = uau::amf;
namespace core = amf::core;


SCENARIO("") {
    GIVEN("Envelope of not inheritable message class") {
        std::unique_ptr<const core::Envelope<>> pElp{new core::Envelope<int>{5}};

        THEN("") {
            REQUIRE(uau::handlerSetMatcher<int>(pElp.get()));
        }

        int i;
        int &ref = i;
        core::Envelope<int> sd(3);
    }

    GIVEN("Envelope of inheritable message class") {
        class B {};
        class D : public B {};
        class Oth : public B {};

        std::unique_ptr<const core::Envelope<>> pElp{new core::Envelope<D>};

        THEN("") {
            REQUIRE(uau::handlerSetMatcher<B>(pElp.get()));
            REQUIRE(uau::handlerSetMatcher<D>(pElp.get()));
            REQUIRE_FALSE(uau::handlerSetMatcher<Oth>(pElp.get()));

//            AND_THEN("") {
//                REQUIRE(uau::handlerSetMatcher<B &>(pElp.get()));
//                REQUIRE(uau::handlerSetMatcher<D &>(pElp.get()));
//                REQUIRE_FALSE(uau::handlerSetMatcher<Oth &>(pElp.get()));
//            }

            AND_THEN("") {
                REQUIRE(uau::handlerSetMatcher<const B>(pElp.get()));
                REQUIRE(uau::handlerSetMatcher<const D>(pElp.get()));
                REQUIRE_FALSE(uau::handlerSetMatcher<const Oth>(pElp.get()));
            }

//            AND_THEN("") {
//                REQUIRE(uau::handlerSetMatcher<const B &>(pElp.get()));
//                REQUIRE(uau::handlerSetMatcher<const D &>(pElp.get()));
//                REQUIRE_FALSE(uau::handlerSetMatcher<const Oth &>(pElp.get()));
//            }
        }
    }
}

/*
#include "lest/lest.hpp"



const lest::test specification[] = {
    "Envelope traits", [] {
        class Some {};

        EXPECT(amf::core::is_envelope<core::Envelope<int>>::value);
        EXPECT(amf::core::is_envelope<core::Envelope<Some>>::value);

        // const
        EXPECT(amf::core::is_envelope<const core::Envelope<int>>::value);
        EXPECT(amf::core::is_envelope<const core::Envelope<Some>>::value);

        // pointers
        EXPECT(amf::core::is_envelope<core::Envelope<int> *>::value);
        EXPECT(amf::core::is_envelope<core::Envelope<Some> *>::value);

        // reference
        EXPECT(amf::core::is_envelope<core::Envelope<int> &>::value);
        EXPECT(amf::core::is_envelope<core::Envelope<Some> &>::value);
    },

    "handlerSetMatcher specialization", [] {
//        class A {
//        public:
//            virtual ~A(){}
//        };

//        class B : public A {};
//        class Oth {};

//        core::Envelope<> *ptrB = new core::Envelope<B>;
//        EXPECT(uau::handlerSetMatcher<core::Envelope<B> *>(ptrB));
//        EXPECT(!uau::handlerSetMatcher<core::Envelope<Oth> *>(ptrB));

//        class C : public B {};
//        core::Envelope<> *ptrC = new core::Envelope<C>;
//        EXPECT(uau::handlerSetMatcher<core::Envelope<B> *>(ptrC));
//        EXPECT(!uau::handlerSetMatcher<core::Envelope<Oth> *>(ptrC));

        //   A1  A2
        //  / \ /
        // B   C
        //
        // core::Envelope<> *pC = new core::Envelope<C>;
        //
        // handlerSetMatchr<core::Envelope<B> *>(pA);   // false
        // handlerSetMatcher<core::Envelope<B> *>(pB);  // true
        // handlerSetMatcher<core::Envelope<>>
        //
        struct A {};
        struct B : public A {};
        struct C : public B {};

        class Logger {
        public:
            void operator ()(const std::string &str) { _log = str; }
            std::string dump() const { auto tmp = _log; return tmp; }

        private:
            std::string _log;
        };

        Logger logIt;

        uau::HandlerSet<core::Envelope<>> handlers;
        handlers.setHandlerFor<core::Envelope<int>>(logIt, "int");
        handlers.setHandlerFor<core::Envelope<char>>(logIt, "char");
        handlers.setHandlerFor<core::Envelope<A>>(logIt, "A");
    }
};

int main() {
    return lest::run(specification);
}
*/
