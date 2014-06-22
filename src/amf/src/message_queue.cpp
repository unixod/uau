#include <stdexcept>
#include "message_queue.h"
#include "core/envelope.h"

namespace amf = uau::amf;
namespace core = amf::core;

uau::amf::MessageQueue::~MessageQueue()
{
    std::lock_guard<std::mutex> lck(_mx);
    _destruction = true;
    _cond.notify_all();
}

void amf::MessageQueue::push(Message msg)
{
    std::lock_guard<std::mutex> lck(_mx);

    _q.push(std::move(msg));
    _cond.notify_all();
}

amf::MessageQueue::Message amf::MessageQueue::waitAndPop()
{
    std::unique_lock<std::mutex> lck(_mx);

    _cond.wait(lck, [this]{
        return !_q.empty() || _destruction;
    });

    Message msg;
    if(!_destruction) {
        msg = std::move(_q.front());
        _q.pop();
    } else {
        throw std::out_of_range{"uau::amf::MessageQueue is empty"};
    }

    return msg;
}

amf::MessageQueue::Message amf::MessageQueue::tryToPop()
{
    std::lock_guard<std::mutex> lck(_mx);
    Message msg;

    if(!_q.empty()) {
        msg = std::move(_q.front());
        _q.pop();
    }

    return msg;
}

amf::MessageQueue::QueueType::size_type amf::MessageQueue::size() const
{
    std::lock_guard<std::mutex> lck(_mx);
    return _q.size();
}
