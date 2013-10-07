#include <string>
#include <future>
#include <random>
#include <deque>
#include "lest/lest.hpp"
#include "actor.h"
#include "core/envelope.h"
#include "message.h"


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
    "transitions", []{
        class A : public uau::amf::Message {};
        class B : public uau::amf::Message {};

        /*
         *                              __B__
         * \                          /       \
         *  V                         V       /
         * (initialState) ---A---> (secondState) ---A--> ((finalState))
         *
         *  this finite automata represents following regular expresion:
         *  AB*A
         *
         */
        class StateMachine : public uau::amf::Actor {
        public:
            // initialState
            StateMachine() {
                on<A>(&StateMachine::secondState);
            }

            std::string getLog() const {
                return _log;
            }

        private:
            void secondState() {
                if(message()->is<A>())
                    _log.append("A");
                else
                    _log.append("B");

                on<B>(&StateMachine::secondState);
                on<A>(&StateMachine::finalState);
            }

            void finalState() {
                _log.append("A");
            }

            std::string _log;
        };

        StateMachine sm;
        auto f = std::async(std::launch::async, [&sm]{
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 1);

            while(!sm.pendingForDeletion()) {
                if(dis(gen))
                    sm.pushToInput(std::make_shared<A>());
                else
                    sm.pushToInput(std::make_shared<B>());
            }
        });


        while(!sm.pendingForDeletion())
            sm.activate();

        f.wait();

        auto result = sm.getLog();

        EXPECT(result.front() == 'A');
        EXPECT(result.back() == 'A');
        result.pop_back();
        result.erase(0, 1);
        EXPECT(result.find('A') == std::string::npos);
        EXPECT(result.empty() || (result.compare(std::string(result.size(), 'B')) == 0));
    }
};

int main() {
    return lest::run(specification);
}
