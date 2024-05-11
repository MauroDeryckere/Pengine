#ifndef COMPONENTWRAPPER
#define COMPONENTWRAPPER

#include "SparseSet.h"

namespace Pengin
{
    template<typename ComponentType>
    class ComponentWrapper final
    {
    public:
        ComponentWrapper(SparseSet<ComponentType, EntityId>* set) : m_ComponentSet(set) 
        { }

        [[nodiscard]] ComponentType& GetComponent(const EntityId id)
        {
            assert(m_ComponentSet);

            if (m_ComponentSet->Contains(id))
            {
                return (*m_ComponentSet)[id];
            }

            return (*m_ComponentSet->Emplace(id, ComponentType{}).first);
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

        iterator begin() noexcept
        { 
            if (m_ComponentSet)
            {
                return m_ComponentSet->begin();
            }
            return typename SparseSet<ComponentType, EntityId>::iterator{};
        }
        iterator end() noexcept
        { 
            if (m_ComponentSet)
            {
                return m_ComponentSet->end();
            }
            return typename SparseSet<ComponentType, EntityId>::iterator{};
        }

        const_iterator cbegin() const noexcept
        {
            if (m_ComponentSet)
            {
                return m_ComponentSet->cbegin(); 
            }
            return typename SparseSet<ComponentType, EntityId>::const_iterator{};
        }
        const_iterator cend() const noexcept
        { 
            if (m_ComponentSet)
            {
                return m_ComponentSet->cend();
            }
            return typename SparseSet<ComponentType, EntityId>::const_iterator{};
        }

        [[nodiscard]] const EntityId GetIdFromIterator(const_iterator it) const noexcept
        {
            return m_ComponentSet->GetKeyFromIterator(it);
        }

        ComponentWrapper(const ComponentWrapper&) = delete;
        ComponentWrapper& operator=(const ComponentWrapper&) = delete;
        ComponentWrapper(ComponentWrapper&&) noexcept = delete;
        ComponentWrapper& operator=(ComponentWrapper&&) noexcept = delete;

    private:
        SparseSet<ComponentType, EntityId>* m_ComponentSet;
    };
}

#endif
