#include "EntityManager.h"
#include "UniqueTypeTracker.h"
#include "ComponentManager.h"
#include <algorithm>

namespace Pengin
{
	EntityManager::EntityManager(ComponentManager& componentManager):
		m_ComponentManagerRef{ componentManager },
		m_TypeBitMap{ InitTypeBitMapping() },
		m_TypeBitVector{InitTypeBitVector() }
	{
	
	}

	const EntityId& EntityManager::CreateEntity() noexcept
	{
		++CURRENT_ID_COUNT;
		m_EntityCompFlags.Emplace(CURRENT_ID_COUNT, Bitset{ m_TypeBitMap.size() });

		return CURRENT_ID_COUNT;
	}

	bool EntityManager::HasEntity(const EntityId& id) const noexcept
	{
		return m_EntityCompFlags.Contains(id);
	}

	bool EntityManager::DestroyEntity(const EntityId& id) noexcept
	{
		const auto& ownedCompTypes{ GetAllCompTypes(id) };
		
		for (const auto& comp : ownedCompTypes)
		{
			if (!m_ComponentManagerRef.RemoveComponent(comp, id))
			{
				return false;
			}
		}
				
		m_EntityCompFlags.Remove(id);

		return true;
	}

	const std::vector<std::type_index> EntityManager::GetAllCompTypes(const EntityId& id) const
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
			typeBitMap.insert({ type, bit });
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

