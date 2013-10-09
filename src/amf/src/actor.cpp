#include "actor.h"
#include "actor_p.h"


namespace amf = uau::amf;
namespace core = amf::core;


amf::Actor::Actor() :
    d_ptr(new ActorPrivate) {}

amf::Actor::Actor(std::unique_ptr<amf::ActorPrivate> d) :
    d_ptr(std::move(d)) {}

amf::Actor::~Actor() {}

std::shared_ptr<core::Envelope<>> amf::Actor::popFromOutput() {
    return d_ptr->outputQueue.waitAndPop();
}

void amf::Actor::pushToInput(std::shared_ptr<core::Envelope<>> msg) {
    d_ptr->inputQueue.push(msg);
}

void amf::Actor::activate() {
    if(_handler.empty()) {           // actor in final state
        deleteLater();
    } else {
        d_ptr->message = std::move(d_ptr->inputQueue.waitAndPop());
        HandlerSet<core::Envelope<>> h = std::move(_handler);

        if(not h.handle(d_ptr->message.get())) {
            _handler = std::move(h);
        }
    }
}

void amf::Actor::tryActivate() {
    if(_handler.empty()) {           // actor in final state
        deleteLater();
    } else {
        if(d_ptr->message = std::move(d_ptr->inputQueue.tryPop())) {
            HandlerSet<core::Envelope<>> h = std::move(_handler);

            if(not h.handle(d_ptr->message.get())) {
                _handler = std::move(h);
            }
        }
    }
}

std::shared_ptr<const core::Envelope<>> amf::Actor::message() const {
    return d_ptr->message;
}

void amf::Actor::deleteLater() {
    d_ptr->pendingForDeletion = true;
}

void amf::Actor::sendEnvelope(std::unique_ptr<core::Envelope<>> msg) {
  d_ptr->outputQueue.push(std::move(msg));
}

bool amf::Actor::pendingForDeletion() const noexcept {
    return d_ptr->pendingForDeletion;
}
