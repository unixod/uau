#include "core/main_router.h"
#include "core/abstract_actor_id.h"

namespace amf = uau::amf;
namespace core = uau::amf::core;

std::pair<std::vector<core::AbstractActor::Id>, bool> core::MainRouter::rout(core::AbstractActor::Id, std::shared_ptr<core::Envelope<>>)
{
}
