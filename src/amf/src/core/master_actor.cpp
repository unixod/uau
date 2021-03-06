#include "uau/amf/core/master_actor.h"
#include "uau/amf/core/abstract_actor_id.h"
#include "uau/amf/core/messages/delete.h"
#include "uau/amf/core/envelope.h"

namespace core = uau::amf::core;

void core::MasterActor::push(Id id, core::AbstractActor::Message msg)
{
//    const core::messages::SystemMessage d;
//    core::envelope_cast<const core::messages::SystemMessage &>(d);
    msg->payload<const core::messages::SystemMessage *>();
//    if(core::envelope_cast<core::messages::SystemMessage>(msg)) {

//    }
//    if(envelope_cast<core::messages::SystemMessage>(msg)) {
//        _messages.push(std::make_shared<core::Envelope<std::pair<Id, core::Envelope<>>>({std::move(id), msg}));
//    }
//    if(msg->is<core::messages::SystemMessage>()) {
//        _messages.push(std::make_shared<core::Envelope<std::pair<Id, core::messages::SystemMessage>);
//    }
}

core::AbstractActor::Message core::MasterActor::pull()
{
}

core::AbstractActor::Message core::MasterActor::tryToPull()
{
}

void core::MasterActor::activate()
{

}

bool core::MasterActor::tryActivate()
{
}

core::AbstractActor::Id core::MasterActor::nextPendingForDeletion()
{
}
