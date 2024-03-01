#ifndef ENTITY
#define ENTITY

#include <type_traits>

namespace Pengin
{
    using EntityId = unsigned;

    static_assert(std::is_unsigned_v<EntityId>, "EntityId must be an unsigned integer type");

    struct Entity
    {
        Entity(const EntityId& id) : entityId{ id }{}

        EntityId entityId;
    };
}

#endif