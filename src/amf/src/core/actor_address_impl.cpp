#include "actor_address_impl.h"
#include <stdexcept>

namespace uau {
namespace amf {
namespace core {

//std::ostream & operator << (std::ostream &out, const ActorAddress &addr)
//{
//    // TODO: need to implement
//}

//std::istream & operator >> (std::istream &, ActorAddress &)
//{
//    // TODO: need to implement
//}

bool operator < (const ActorAddress &addr1, const ActorAddress &addr2)
{
    return addr1->_hash < addr2->_hash;
}

bool operator == (const ActorAddress &addr1, const ActorAddress &addr2)
{
    return addr1->_hash == addr2->_hash;
}

std::size_t hash(const ActorAddress &addr)
{
    return addr->_hash;
}

} // namespace core
} // namespace amf
} // namespace uau
