#include "lest/lest.hpp"
#include "actor.h"
#include "message.h"
#include <string>


class Msg1 : public uau::amf::Message {};
class Msg2 : public uau::amf::Message {};
class Msg3 : public uau::amf::Message {};
class Msg4 : public uau::amf::Message {};


class TestActor : public uau::amf::Actor {
public:
    void someState() {

        on<Msg1, Msg2>(&TestActor::someState);
        on<Msg3>(&TestActor::stateWithParams, 1, "hello");
        on<Msg4>([]{/*same state*/});
    }

    void stateSimple() {
    }

    void stateWithParams(int, const std::string &) {
    }
};


const lest::test specification[] = {
    "stub", []{
        TestActor testActor;


    }
};

int main() {
    return lest::run(specification);
}
