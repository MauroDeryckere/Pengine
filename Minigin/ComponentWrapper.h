#ifndef COMPONENTWRAPPER
#define COMPONENTWRAPPER

#include "Entity.h"

#include "SparseSet.h"
#include <type_traits>

namespace Pengin
{
    using KeyType = EntityId;

    template<typename ComponentType>
    requires std::is_default_constructible_v<ComponentType>
    class ComponentWrapper
    {
    public:
        ComponentWrapper(SparseSet<ComponentType, KeyType>& set) : m_ComponentSet{ set } {}

        [[nodiscard]] ComponentType& GetComponent(const EntityId& id)
        {
            if (m_ComponentSet.Contains(id))
            {
                return m_ComponentSet[id];
            }

            return m_ComponentSet.Emplace(id, ComponentType{});
        }

        [[nodiscard]] const ComponentType& GetComponent(const EntityId& id) const
        {
            if (m_ComponentSet.Contains(id))
            {
                return m_ComponentSet[id];
            }

            throw std::out_of_range("Component not found for the given entity ID");
        }

        using iterator = typename SparseSet<ComponentType, KeyType>::iterator;
        using const_iterator = typename SparseSet<ComponentType, KeyType>::const_iterator;

        iterator begin() { return m_ComponentSet.begin(); }
        iterator end() { return m_ComponentSet.end(); }

        const_iterator cbegin() const { return m_ComponentSet.cbegin(); }
        const_iterator cend() const { return m_ComponentSet.cend(); }

    private:
        SparseSet<ComponentType, KeyType>& m_ComponentSet;
    };
}

#endif