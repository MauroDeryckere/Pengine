#ifndef COMPONENTSTORAGE
#define COMPONENTSTORAGE

#include "SparseSet.h"
#include "ComponentSetHandler.h"
#include "Components.h"

#include <memory>
#include <unordered_map>
#include <typeindex>

namespace Pengin
{
    class ComponentStorage final
    {
    public:
        ComponentStorage()
        {
            //testing
            m_componentStorage[typeid(TestComponent)] = std::make_unique<TypedComponentSetHandler<TestComponent, unsigned>>();
        };
        ~ComponentStorage() = default;

        ComponentStorage(const ComponentStorage&) = delete;
        ComponentStorage& operator=(const ComponentStorage&) = delete;
        ComponentStorage(ComponentStorage&&) noexcept = delete;
        ComponentStorage& operator=(ComponentStorage&&) noexcept = delete;

        template<typename ComponentType, typename Key>
        requires std::is_default_constructible_v<Key> && std::is_constructible_v<ComponentType>
        SparseSet<ComponentType, Key>* getComponentSet()  const //Private in future, just testing 
        {
            const auto it{ m_componentStorage.find(typeid(ComponentType)) };
            if (it != m_componentStorage.end()) 
            {
                const auto handler{ static_cast<TypedComponentSetHandler<ComponentType, Key>*>(it->second.get()) };
                return &(handler->m_ComponentSet);
            }
            else 
            {
                return nullptr;
            }
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<ComponentSetHandler>> m_componentStorage;
    };
}

#endif
