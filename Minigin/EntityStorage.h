#ifndef ENTITYSTORAGE
#define ENTITYSTORAGE

#include "Entity.h"

#include "SparseSet.h"
#include <unordered_set>

namespace Pengin
{
    class EntityStorage 
    {
    public:
        EntityStorage() = default;

        EntityStorage(const EntityStorage&) = delete;
        EntityStorage& operator=(const EntityStorage&) = delete;
        EntityStorage(EntityStorage&&) noexcept = delete;
        EntityStorage& operator=(EntityStorage&&) noexcept = delete;

        const EntityId& CreateEntity();
        [[nodiscard]] bool HasEntity(const EntityId& id) const noexcept;


    private:
        std::unordered_set<EntityId> m_Entities;
        //TODO; Map to bind component flags to entity id 
    };
}

#endif

