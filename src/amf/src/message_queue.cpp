#include "message_queue.h"
#include "message.h"


void uau::amf::MessageQueue::push(std::unique_ptr<Message> msg){
    std::lock_guard<std::mutex> lck(mx);

    q.push(std::move(msg));
    cond.notify_all();
}

std::unique_ptr<uau::amf::Message> uau::amf::MessageQueue::waitAndPop(){
    std::unique_lock<std::mutex> lck(mx);

    cond.wait(lck, [this]{
        return !q.empty();
    });

    std::unique_ptr<Message> msg = std::move(q.front());
    q.pop();

    return msg;
}


