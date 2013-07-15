#include "actor.h"
#include "actor_p.h"


uau::amf::Actor::Actor() :
    d_ptr(new ActorPrivate) {}

uau::amf::Actor::Actor(uau::amf::ActorPrivate *d) :
    d_ptr(d) {}

uau::amf::Actor::~Actor() {}

std::unique_ptr<uau::amf::Message> uau::amf::Actor::popFromOutput() {

}

void uau::amf::Actor::pushToInput(std::unique_ptr<uau::amf::Message>) {

}
