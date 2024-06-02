#ifndef ENTITYID
#define ENTITYID

#include <type_traits>
#include <functional>
#include <string>

namespace Pengin
{
	using EntityId = uint32_t;
	constexpr EntityId NULL_ENTITY_ID{ UINT32_MAX };

	static_assert(std::is_unsigned_v<EntityId>, "Engine error: EntityId must be an unsigned integer type");

    inline std::string EntityIdToString(const EntityId id) noexcept
    {
		return (id == NULL_ENTITY_ID) ? "NULL" : std::to_string(id);
    }
}

#endif