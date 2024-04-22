#ifndef ENTITYMANAGER
#define ENTITYMANAGER

#include "SparseSet.h"
#include "Bitset.h"
#include "EntityId.h"

#include <unordered_map>
#include <typeindex>

namespace Pengin
{
    class ComponentManager;

    class EntityManager final
    {
    public:
        EntityManager(ComponentManager& componentManager);
        ~EntityManager() = default;

        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;
        EntityManager(EntityManager&&) noexcept = delete;
        EntityManager& operator=(EntityManager&&) noexcept = delete;

        const EntityId CreateEntity() noexcept;
        [[nodiscard]] bool HasEntity(const EntityId id) const noexcept;

        [[nodiscard]] bool DestroyEntity(const EntityId id) noexcept;

        [[nodiscard]] const std::vector<EntityId>& GetAllEntityIds() const noexcept;

        [[nodiscard]] const std::vector<std::type_index> GetAllCompTypes(const EntityId id) const;

        void AddComponent(std::type_index typeIdx, const EntityId id) noexcept
        {
            const size_t bit{ m_TypeBitMap.at(typeIdx)};

            m_EntityCompFlags[id].Set(bit);
        }

        void RemoveComponent(std::type_index typeIdx, const EntityId id) noexcept
        {
            const size_t bit{ m_TypeBitMap.at(typeIdx) };

            m_EntityCompFlags[id].Clear(bit);    
        }

    private:
        ComponentManager& m_ComponentManagerRef;

        const std::unordered_map<std::type_index, size_t> m_TypeBitMap;
        const std::vector<std::type_index> m_TypeBitVector; //rev mapping

        SparseSet<RunTimeBitset, EntityId> m_EntityCompFlags;

        [[nodiscard]] const std::unordered_map<std::type_index, size_t> InitTypeBitMapping() noexcept;
        [[nodiscard]] const std::vector<std::type_index> InitTypeBitVector() noexcept;
    };
}

#endif

