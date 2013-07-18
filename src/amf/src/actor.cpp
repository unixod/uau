#include "actor.h"
#include "actor_p.h"


uau::amf::Actor::Actor() :
    d_ptr(new ActorPrivate) {}

uau::amf::Actor::Actor(std::unique_ptr<uau::amf::ActorPrivate> d) :
    d_ptr(std::move(d)) {}

uau::amf::Actor::~Actor() {}

inline std::shared_ptr<uau::amf::Message> uau::amf::Actor::popFromOutput() {
    return d_ptr->outputQueue.waitAndPop();
}

inline void uau::amf::Actor::pushToInput(std::shared_ptr<Message> msg) {
    d_ptr->inputQueue.push(msg);
}

void uau::amf::Actor::activate() {
    // TODO: prepare actor for deletion in final state (handlers is empty)
//    if(handler.empty()) deleteLater();

    d_ptr->message = std::move(d_ptr->inputQueue.waitAndPop());
    uau::amf::MessageHandler<> h = std::move(handler);

    if(not h.handle(d_ptr->message.get()))
        handler = std::move(h);
}

void uau::amf::Actor::send(std::unique_ptr<uau::amf::Message> msg) {
    d_ptr->outputQueue.push(std::move(msg));
}

std::weak_ptr<const uau::amf::Message> uau::amf::Actor::message() const {
    return d_ptr->message;
}

void uau::amf::Actor::deleteLater() {
}
