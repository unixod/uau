#include "actor.h"
#include "actor_p.h"
#include "core/messages/delete.h"


namespace amf = uau::amf;
namespace core = amf::core;


amf::Actor::Actor() :
    d_ptr(new ActorPrivate) {}

amf::Actor::Actor(std::unique_ptr<amf::ActorPrivate> d) :
    d_ptr(std::move(d)) {}

amf::Actor::~Actor() {}

void amf::Actor::pushToInput(std::shared_ptr<core::Envelope<>> msg) {
    d_ptr->inputQueue.push(msg);
}

std::shared_ptr<core::Envelope<>> amf::Actor::popFromOutput() {
    return d_ptr->outputQueue.waitAndPop();
}

std::shared_ptr<core::Envelope<>> amf::Actor::tryPopFromOutput() {
    return d_ptr->outputQueue.tryPop();
}

void amf::Actor::activate() {
    if(_handler.empty()) {           // actor in final state
        send(core::messages::Delete{});
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
        send(core::messages::Delete{});
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

void amf::Actor::sendEnvelope(std::unique_ptr<core::Envelope<>> msg) {
  d_ptr->outputQueue.push(std::move(msg));
}
