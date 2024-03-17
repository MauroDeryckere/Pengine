#ifndef CONSTCOMPONENTWRAPPER
#define CONSTCOMPONENTWRAPPER

#include "SparseSet.h"
#include "Entity.h"

namespace Pengin
{
    template<typename ComponentType>
    class ConstComponentWrapper final
    {
    public:
        ConstComponentWrapper(const SparseSet<ComponentType, EntityId>& set) : m_ComponentSet(&set) {}

        [[nodiscard]] const ComponentType& GetComponent(const EntityId& id) const
        {
            if (m_ComponentSet->Contains(id))
            {
                return (*m_ComponentSet)[id];
            }

            throw std::out_of_range("Component not found for the given entity ID");
        }

        using const_iterator = typename SparseSet<ComponentType, EntityId>::const_iterator;

        const_iterator begin() const noexcept { return m_ComponentSet->cbegin(); }
        const_iterator end() const noexcept { return m_ComponentSet->cend(); }

        const_iterator cbegin() const noexcept { return m_ComponentSet->cbegin(); }
        const_iterator cend() const noexcept { return m_ComponentSet->cend(); }

    private:
        const SparseSet<ComponentType, EntityId>* m_ComponentSet;
    };
}

#endif
