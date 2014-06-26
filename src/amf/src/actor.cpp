#include "actor.h"
#include "actor_p.h"
#include "core/messages/delete.h"
#include "core/abstract_actor_id.h"

namespace amf = uau::amf;
namespace core = amf::core;

amf::Actor::Actor() :
    d_ptr(new ActorPrivate) {}

amf::Actor::Actor(std::unique_ptr<amf::ActorPrivate> d) :
    d_ptr(std::move(d)) {}

amf::Actor::~Actor() {}

void amf::Actor::push(Id /*not_used*/, AbstractActor::Message msg)
{
    d_ptr->inputQueue.push(msg);
}

core::AbstractActor::Message amf::Actor::pull()
{
    return d_ptr->outputQueue.waitAndPop();
}

core::AbstractActor::Message amf::Actor::tryToPull()
{
    return d_ptr->outputQueue.tryToPop();
}

void amf::Actor::activate()
{
    if(_handler.empty()) {           // actor in final state
        if (d_ptr->outputQueue.empty()) {
            send(core::messages::Delete{});
        }
    } else {
        d_ptr->message = std::move(d_ptr->inputQueue.waitAndPop());
        HandlerSet<core::Envelope<>> h = std::move(_handler);

        if(not h.handle(d_ptr->message.get())) {
            _handler = std::move(h);
        }
    }
}

bool amf::Actor::tryActivate()
{
    bool activated = false;

    if(_handler.empty()) {           // actor in final state
        if (d_ptr->outputQueue.empty()) {
            send(core::messages::Delete{});
            activated = true;
        }
    } else {
        if(d_ptr->message = std::move(d_ptr->inputQueue.tryToPop())) {
            HandlerSet<core::Envelope<>> h = std::move(_handler);

            if(not h.handle(d_ptr->message.get())) {
                _handler = std::move(h);
            }
        }
        activated = static_cast<bool>(d_ptr->message);
    }

    return activated;
}

core::AbstractActor::Message amf::Actor::message() const
{
    return d_ptr->message;
}

void amf::Actor::sendEnvelope(AbstractActor::Message &&msg)
{
  d_ptr->outputQueue.push(std::move(msg));
}
