#ifndef ENTITYID
#define ENTITYID

#include <type_traits>
#include <functional>
#include <string>

namespace Pengin
{
	using EntityId = unsigned;
	constexpr EntityId NULL_ENTITY_ID{ UINT32_MAX };

	static_assert(std::is_unsigned_v<EntityId>, "Engine error: EntityId must be an unsigned integer type");

    inline std::string EntityIdToString(const EntityId& id)
    {
        if (id == NULL_ENTITY_ID)
            return "NULL";
        else
            return std::to_string(id);
    }

	static EntityId CURRENT_ID_COUNT{ 0 };
}

#endif