#include "core/master_actor.h"
#include "core/abstract_actor_id.h"


namespace core = uau::amf::core;


void core::MasterActor::pushToInput(std::shared_ptr<Envelope<> >) {
}

std::shared_ptr<core::Envelope<>> core::MasterActor::popFromOutput() {
}

std::shared_ptr<core::Envelope<>> core::MasterActor::tryPopFromOutput() {
}

void core::MasterActor::activate() {
}

bool core::MasterActor::tryActivate() {
}

core::AbstractActor::Id core::MasterActor::nextPendingForDeletion() {
}
