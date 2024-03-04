#include "EntityStorage.h"
#include <algorithm>

namespace Pengin
{
	const EntityId& Pengin::EntityStorage::CreateEntity()
	{
		++CURRENT_ID_COUNT;

		m_Entities.emplace(CURRENT_ID_COUNT);

		return CURRENT_ID_COUNT;

	}

	bool EntityStorage::HasEntity(const EntityId& id) const noexcept
	{
		return m_Entities.find(id) != m_Entities.end();
	}
}


