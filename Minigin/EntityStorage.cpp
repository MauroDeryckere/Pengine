#include "EntityStorage.h"
#include <algorithm>

#include "ComponentBitSet.h"

namespace Pengin
{
	EntityStorage::EntityStorage()
	{
		//int* testArr = new int[UniqueTypes::GetConstSet().size()];

		ComponentBitSet bs{6};
		bs.PrintBitSet();

		bs.Set(0);
		bs.PrintBitSet();

		bs.Clear(0);
		bs.PrintBitSet();

		bool test = bs.Test(0);
		std::cout << test << "\n";
		
		ComponentBitSet bs2{ 6 };

		auto bs3 = bs & bs2;
		bs3.PrintBitSet();

		std::cout << "\n";

		m_EntityCompFlags.Emplace(0, 8);

		for (auto & flag : m_EntityCompFlags)
		{
			flag.PrintBitSet();
		}
	}

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


