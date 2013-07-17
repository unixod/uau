#include "message_queue.h"
#include "message.h"


void uau::amf::MessageQueue::push(std::shared_ptr<Message> msg){
    std::lock_guard<std::mutex> lck(mx);

    q.push(std::move(msg));
    cond.notify_all();
}

std::shared_ptr<uau::amf::Message> uau::amf::MessageQueue::waitAndPop(){
    std::unique_lock<std::mutex> lck(mx);

    cond.wait(lck, [this]{
        return !q.empty();
    });

    std::shared_ptr<Message> msg = std::move(q.front());
    q.pop();

    return msg;
}

size_t uau::amf::MessageQueue::size() const {
    std::lock_guard<std::mutex> lck(mx);

    auto sz = q.size();
    cond.notify_all();

    return sz;
}



