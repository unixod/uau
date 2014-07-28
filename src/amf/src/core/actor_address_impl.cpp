#include "actor_address_impl.h"

namespace uau {
namespace amf {
namespace core {

std::ostream & operator << (std::ostream &, const ActorAddress &)
{

}

std::istream & operator >> (std::istream &, ActorAddress &)
{

}

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
