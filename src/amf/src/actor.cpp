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

inline void uau::amf::Actor::pushToInput(std::unique_ptr<uau::amf::Message> msg) {
    d_ptr->inputQueue.push(std::move(msg));
}

void uau::amf::Actor::activate() {
    message = std::move(d_ptr->inputQueue.waitAndPop());
    uau::amf::MessageHandler<> h = std::move(handler);
    h.handle(message.get());
    // TODO does actor in final state
}

void uau::amf::Actor::send(std::unique_ptr<uau::amf::Message> msg){
    d_ptr->outputQueue.push(std::move(msg));
}
