#ifndef ENTITYMANAGER
#define ENTITYMANAGER

#include "Entity.h"
#include "SparseSet.h"
#include "Bitset.h"
#include <unordered_set>

namespace Pengin
{
    class EntityManager final
    {
    public:
        EntityManager();

        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;
        EntityManager(EntityManager&&) noexcept = delete;
        EntityManager& operator=(EntityManager&&) noexcept = delete;

        const EntityId& CreateEntity();
        [[nodiscard]] bool HasEntity(const EntityId& id) const noexcept;


    private:
        std::unordered_set<EntityId> m_Entities;

        SparseSet<Bitset, EntityId> m_EntityCompFlags;
    };
}

#endif

