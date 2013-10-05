#include "actor.h"
#include "actor_p.h"


uau::amf::Actor::Actor() :
    d_ptr(new ActorPrivate) {}

uau::amf::Actor::Actor(std::unique_ptr<uau::amf::ActorPrivate> d) :
    d_ptr(std::move(d)) {}

uau::amf::Actor::~Actor() {}

std::shared_ptr<uau::amf::Message> uau::amf::Actor::popFromOutput() {
    return d_ptr->outputQueue.waitAndPop();
}

void uau::amf::Actor::pushToInput(std::shared_ptr<Message> msg) {
    d_ptr->inputQueue.push(msg);
}

void uau::amf::Actor::activate() {
    if(_handler.empty()) {           // actor in final state
        deleteLater();
    } else {
        d_ptr->message = std::move(d_ptr->inputQueue.waitAndPop());
        uau::amf::TypedActionMap<Message> h = std::move(_handler);

        if(not h.handle(d_ptr->message.get())) {
            _handler = std::move(h);
        }
    }
}

void uau::amf::Actor::tryActivate() {
    if(_handler.empty()) {           // actor in final state
        deleteLater();
    } else {
        if(d_ptr->message = std::move(d_ptr->inputQueue.tryPop())) {
            uau::amf::TypedActionMap<Message> h = std::move(_handler);

            if(not h.handle(d_ptr->message.get())) {
                _handler = std::move(h);
            }
        }
    }
}

void uau::amf::Actor::send(std::unique_ptr<uau::amf::Message> msg) {
    d_ptr->outputQueue.push(std::move(msg));
}

std::shared_ptr<const uau::amf::Message> uau::amf::Actor::message() const {
    return d_ptr->message;
}

void uau::amf::Actor::deleteLater() {
    d_ptr->pendingForDeletion = true;
}


bool uau::amf::Actor::pendingForDeletion() const noexcept {
    return d_ptr->pendingForDeletion;
}
