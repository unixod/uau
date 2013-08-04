#include "message_queue.h"
#include "message.h"


void uau::amf::MessageQueue::push(std::shared_ptr<Message> msg) {
    std::lock_guard<std::mutex> lck(_mx);

    _q.push(std::move(msg));
    _cond.notify_all();
}

std::shared_ptr<uau::amf::Message> uau::amf::MessageQueue::waitAndPop() {
    std::unique_lock<std::mutex> lck(_mx);

    _cond.wait(lck, [this]{
        return !_q.empty();
    });

    std::shared_ptr<Message> msg = std::move(_q.front());
    _q.pop();

    return msg;
}

std::shared_ptr<uau::amf::Message> uau::amf::MessageQueue::tryPop() {
    std::lock_guard<std::mutex> lck(_mx);

    std::shared_ptr<Message> msg;

    if(!_q.empty()) {
        msg = std::move(_q.front());
        _q.pop();
    }

    return msg;
}
