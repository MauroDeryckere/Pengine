#include "EntityStorage.h"
#include <algorithm>

namespace Pengin
{
	const EntityId& Pengin::EntityStorage::CreateEntity()
	{
		const EntityId nextId{ CURRENT_ID_COUNT + 1 };

		m_Entities.emplace(nextId);

		++CURRENT_ID_COUNT;
	}
	bool EntityStorage::HasEntity(const EntityId& id) const
	{
		return m_Entities.find(id) != m_Entities.end();
	}
}


