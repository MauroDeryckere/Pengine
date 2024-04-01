#ifndef COMPONENTWRAPPER
#define COMPONENTWRAPPER

#include "SparseSet.h"

namespace Pengin
{
    template<typename ComponentType>
    class ComponentWrapper final
    {
    public:
        ComponentWrapper(SparseSet<ComponentType, EntityId>& set) : m_ComponentSet(&set) {}

        [[nodiscard]] ComponentType& GetComponent(const EntityId id)
        {
            if (m_ComponentSet->Contains(id))
            {
                return (*m_ComponentSet)[id];
            }

            return m_ComponentSet->Emplace(id, ComponentType{});
        }

        [[nodiscard]] const ComponentType& GetComponent(const EntityId id) const
        {
            if (m_ComponentSet->Contains(id))
            {
                return (*m_ComponentSet)[id];
            }

            throw std::out_of_range("Component not found for the given entity ID");
        }

        using iterator = typename SparseSet<ComponentType, EntityId>::iterator;
        using const_iterator = typename SparseSet<ComponentType, EntityId>::const_iterator;

        iterator begin() noexcept { return m_ComponentSet->begin(); }
        iterator end() noexcept { return m_ComponentSet->end(); }

        const_iterator cbegin() const noexcept { return m_ComponentSet->cbegin(); }
        const_iterator cend() const noexcept { return m_ComponentSet->cend(); }

        [[nodiscard]] const EntityId GetIdFromIterator(const_iterator it) const noexcept
        {
            return m_ComponentSet->GetKeyFromIterator(it);
        }

    private:
        SparseSet<ComponentType, EntityId>* m_ComponentSet;
    };
}

#endif
