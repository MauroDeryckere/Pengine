#ifndef CONSTCOMPONENTWRAPPER
#define CONSTCOMPONENTWRAPPER

#include "SparseSet.h"

namespace Pengin
{
    template<typename ComponentType>
    class ConstComponentWrapper final
    {
    public:
        ConstComponentWrapper(const SparseSet<ComponentType, EntityId>* set) : m_ComponentSet(set) 
        { }

        [[nodiscard]] const ComponentType& GetComponent(const EntityId id) const
        {
            assert(m_ComponentSet);
            assert(!m_ComponentSet.Empty());

            if (m_ComponentSet->Contains(id))
            {
                return (*m_ComponentSet)[id];
            }

            throw std::out_of_range("Component not found for the given entity ID");
        }

        using const_iterator = typename SparseSet<ComponentType, EntityId>::const_iterator;

        const_iterator begin() const noexcept
        {
            if (m_ComponentSet)
            {
                return m_ComponentSet->cbegin();
            }
            return typename SparseSet<ComponentType, EntityId>::const_iterator{};
        }
        const_iterator end() const noexcept
        {
            if (m_ComponentSet)
            {
                return m_ComponentSet->cend();
            }
            return typename SparseSet<ComponentType, EntityId>::const_iterator{};
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

        ConstComponentWrapper(const ConstComponentWrapper&) = delete;
        ConstComponentWrapper& operator=(const ConstComponentWrapper&) = delete;
        ConstComponentWrapper(ConstComponentWrapper&&) noexcept = delete;
        ConstComponentWrapper& operator=(ConstComponentWrapper&&) noexcept = delete;

    private:
        const SparseSet<ComponentType, EntityId>* const m_ComponentSet;
    };
}

#endif
