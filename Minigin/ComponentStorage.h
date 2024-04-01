#ifndef COMPONENTSTORAGE
#define COMPONENTSTORAGE

#include "SparseSet.h"

#include "UniqueTypeTracker.h"
#include "BaseComponentStorage.h"

namespace Pengin
{
    template<typename ComponentType>
    class ComponentStorage final :
        public BaseComponentStorage
    {
    public:
        ComponentStorage() : BaseComponentStorage{},
            m_ComponentSet{} {}

        virtual void RemoveComponent(const EntityId id) noexcept override
        {
            m_ComponentSet.Remove(id);
        }

        virtual bool HasComponent(const EntityId id) noexcept override
        {
            return m_ComponentSet.Contains(id);
        }

        SparseSet<ComponentType, EntityId>& GetSet() { return m_ComponentSet; }
        const SparseSet<ComponentType, EntityId>& GetSet() const { return m_ComponentSet; }

        ~ComponentStorage() override = default;

        ComponentStorage(const ComponentStorage&) = delete;
        ComponentStorage& operator=(const ComponentStorage&) = delete;
        ComponentStorage(ComponentStorage&&) noexcept = delete;
        ComponentStorage& operator=(ComponentStorage&&) noexcept = delete;

    private:
        SparseSet<ComponentType, EntityId> m_ComponentSet;
    };
}

#endif
