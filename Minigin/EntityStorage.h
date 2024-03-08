#ifndef ENTITYSTORAGE
#define ENTITYSTORAGE

#include "Entity.h"
#include "SparseSet.h"

#include "UniqueTypeTracker.h"
#include "ComponentBitSet.h"

#include <unordered_set>

namespace Pengin
{
    class EntityStorage 
    {
    public:
        EntityStorage();

        EntityStorage(const EntityStorage&) = delete;
        EntityStorage& operator=(const EntityStorage&) = delete;
        EntityStorage(EntityStorage&&) noexcept = delete;
        EntityStorage& operator=(EntityStorage&&) noexcept = delete;

        const EntityId& CreateEntity();
        [[nodiscard]] bool HasEntity(const EntityId& id) const noexcept;


    private:
        std::unordered_set<EntityId> m_Entities;

        SparseSet<ComponentBitSet, EntityId> m_EntityCompFlags;
    };
}

#endif

