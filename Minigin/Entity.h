#ifndef ENTITY
#define ENTITY

#include <type_traits>
#include <functional>

namespace Pengin
{
    using EntityId = unsigned;
    constexpr EntityId NULL_ENTITY_ID{0};


    static_assert(std::is_unsigned_v<EntityId>, "EntityId must be an unsigned integer type");

    struct Entity
    {
        Entity(const EntityId& id) : entityId{ id }{}
        ~Entity() = default;

        EntityId entityId;

        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;
        Entity(Entity&&) noexcept = delete;
        Entity& operator=(Entity&&) noexcept = delete;

    };

    static EntityId CURRENT_ID_COUNT{ 0 };
}

#endif