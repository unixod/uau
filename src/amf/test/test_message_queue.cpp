#include "lest/lest.hpp"
#include "message_queue.h"
#include "message.h"
#include <future>
#include <algorithm>

class Msg : public uau::amf::Message {
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
                    std::unique_ptr<Msg> msg(new Msg(i));
                    queue.push(std::move(msg));
                }
            });

            std::async(std::launch::async, [&]{
                for(int i = k/2; i < k; i++) {
                    std::unique_ptr<Msg> msg(new Msg(i));
                    queue.push(std::move(msg));
                }
            });

            std::vector<int> v;
            int i = 0;
            std::generate_n(std::back_inserter(v), k, [&i](){return i++;});

            for(int i = 0; i < k; i++) {
                std::unique_ptr<Msg> msg(static_cast<Msg*>(queue.waitAndPop().release()));
                v.erase(std::lower_bound(v.begin(), v.end(), msg->getNum()));
            }

            EXPECT(v.size() == 0);
        }
    }
};


int main(){
    return lest::run(specification);
}
