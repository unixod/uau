#include "lest/lest.hpp"
#include "core/envelope.h"
#include "handlerset.h"


namespace amf = uau::amf;
namespace core = amf::core;


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
