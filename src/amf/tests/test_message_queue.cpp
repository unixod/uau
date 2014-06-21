#include <future>
#include <algorithm>
#include "lest/lest.hpp"
#include "message_queue.h"
#include "core/envelope.h"


namespace amf = uau::amf;
namespace core = amf::core;


class Msg {
public:
    Msg(int i) : i(i) {}

    int getNum() const {
        return i;
    }

private:
    int i;
};


const lest::test specification[] = {
    "push/pop from separate threads", []{
        for(int cnt = 0; cnt < 10; cnt++) {
            uau::amf::MessageQueue queue;

            int k = 50;

            std::async(std::launch::async, [&]{
                for(int i = 0; i < k/2; i++) {
                    queue.push(std::make_shared<core::Envelope<Msg>>(i));
                }
            });

            std::async(std::launch::async, [&]{
                for(int i = k/2; i < k; i++) {
                    queue.push(std::make_shared<core::Envelope<Msg>>(i));
                }
            });

            std::vector<int> v;
            int i = 0;
            std::generate_n(std::back_inserter(v), k, [&i](){return i++;});

//            for(int i = 0; i < k; i++) {
//                auto envelope = std::dynamic_pointer_cast<core::Envelope<Msg>>(queue.waitAndPop());
//                v.erase(std::lower_bound(v.begin(), v.end(), envelope->message().getNum()));
//            }

            EXPECT(v.size() == 0);
        }
    }
};


int main() {
    return lest::run(specification);
}
