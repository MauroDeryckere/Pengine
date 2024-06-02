#include "EntityManager.h"
#include "UniqueTypeTracker.h"
#include "ComponentManager.h"

#include <algorithm>

namespace Pengin
{
	EntityManager::EntityManager():
		m_TypeBitMap{ InitTypeBitMapping() },
		m_TypeBitVector{ InitTypeBitVector() }
	{ }

	const EntityId EntityManager::CreateEntity() noexcept
	{
		const EntityId id{ CURRENT_ID_COUNT };
		m_EntityCompFlags.Emplace(id, RunTimeBitset{ m_TypeBitMap.size() });

		++CURRENT_ID_COUNT;

		return id;
	}

	bool EntityManager::HasEntity(const EntityId id) const noexcept
	{
		return m_EntityCompFlags.Contains(id);
	}

	void EntityManager::DestroyEntity(const EntityId id) noexcept
	{		
		m_EntityCompFlags.Remove(id);
	}

	const std::vector<EntityId>& EntityManager::GetAllEntityIds() const noexcept
	{
		return m_EntityCompFlags.GetAllKeys();
	}

	const std::vector<std::type_index> EntityManager::GetAllCompTypes(const EntityId id) const
	{
		const auto& bitSet{ m_EntityCompFlags[id] };

		std::vector<std::type_index> compTypes{};

		for (size_t bit{ 0 }; bit < bitSet.Size(); ++bit)
		{
			if (bitSet.Test(bit))
			{
				compTypes.emplace_back(m_TypeBitVector[bit]);
			}
		}

		return compTypes;
	}

	const std::unordered_map<std::type_index, size_t> EntityManager::InitTypeBitMapping() noexcept
	{
		size_t bit{ 0 };
		std::unordered_map<std::type_index, size_t> typeBitMap;

		for (const auto& type : UniqueTypes::GetConstSet())
		{
			typeBitMap.emplace(type, bit );
			++bit;
		}

		return typeBitMap;

	}
	const std::vector<std::type_index> EntityManager::InitTypeBitVector() noexcept
	{
		const auto& uniqueTypeSet{ UniqueTypes::GetConstSet() };

		std::vector<std::type_index> typeBitVec{};
		typeBitVec.reserve(uniqueTypeSet.size());
		
		for (const auto& type : uniqueTypeSet)
		{
			typeBitVec.emplace_back(type);
		}

		return typeBitVec;
	}
}


