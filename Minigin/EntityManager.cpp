#include "EntityManager.h"
#include "UniqueTypeTracker.h"
#include "ComponentManager.h"
#include "ECSEventInter.h"
#include <algorithm>

namespace Pengin
{
	EntityManager::EntityManager(ComponentManager& componentManager):
		m_ComponentManagerRef{ componentManager },
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

	bool EntityManager::DestroyEntity(const EntityId id, EcsEventInter& eventInter) noexcept
	{
		const auto& ownedCompTypes{ GetAllCompTypes(id) };
		
		for (const auto& comp : ownedCompTypes)
		{
			eventInter.SetObserverDirty(id, comp); //This does result in some redundant calls upon remove if a component does not have an observer but plan on moving away from observers inside components anyway (well be depracated)

			if (!m_ComponentManagerRef.RemoveComponent(comp, id))
			{
				return false;
			}
		}
				
		m_EntityCompFlags.Remove(id);

		return true;
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


