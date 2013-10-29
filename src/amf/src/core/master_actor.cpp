#include "core/master_actor.h"
#include "core/abstract_actor_id.h"
#include "core/messages/delete.h"
#include "core/envelope.h"


namespace core = uau::amf::core;


void core::MasterActor::pushToInput(Id, std::shared_ptr<Envelope<>> msg) {
//    if(msg->is<core::messages::SystemMessage>()) {
//        _messages.push(std::make_shared<core::Envelope<std::pair<Id, core::messages::SystemMessage>);
//    }
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
