#ifndef COMPONENTSTORAGE
#define COMPONENTSTORAGE

#include "Entity.h"

#include "SparseSet.h"
#include "ComponentWrapper.h"

#include <memory>
#include <cassert>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <optional>

namespace Pengin
{
    using KeyType = EntityId;
    class ComponentStorage final
    {
    public:
        ComponentStorage() = default;
        ~ComponentStorage() = default;

        ComponentStorage(const ComponentStorage&) = delete;
        ComponentStorage& operator=(const ComponentStorage&) = delete;
        ComponentStorage(ComponentStorage&&) noexcept = delete;
        ComponentStorage& operator=(ComponentStorage&&) noexcept = delete;
        
        template<typename ComponentType, typename... Args>
        bool AddComponent(const EntityId& id, Args&&... args) 
        requires std::is_constructible_v<ComponentType, Args...>
        {
            SparseSet<ComponentType, KeyType>& compSet{ GetComponentSet<ComponentType>() };
                      
            return compSet.Emplace(id, std::forward<Args>(args)...);
        }

        template<typename ComponentType>
        void RemoveComponent(const EntityId& id) requires std::is_constructible_v<ComponentType>
        {
            SparseSet<ComponentType, KeyType>& compSet{ GetComponentSet<ComponentType>() };

            compSet.Remove(id);
        }

        template<typename ComponentType>
        bool HasComponent(const EntityId& id) const
        {
            std::optional<const SparseSet<ComponentType, KeyType>*> compSetOptional{ GetComponentSet<ComponentType>() };

            if (compSetOptional.has_value()) 
            {
                return compSetOptional.value()->Contains(id);
            }

            return false;
        }

        template<typename ComponentType>
        ComponentType& GetComponent(const EntityId& id)
        requires std::is_default_constructible_v<ComponentType>
        {
            SparseSet<ComponentType, KeyType>& compSet = GetComponentSet<ComponentType>();

            if (compSet.Contains(id))
            {
                return compSet[id];
            }

            compSet.Emplace(id, ComponentType{});
            return compSet[id];
        }

        //const ComponentType& GetComponent(const EntityId& id) const
        //{
        //    if (m_ComponentSet.Contains(id))
        //    {
        //        return m_ComponentSet[id];
        //    }

        //    throw std::out_of_range("Component not found for the given entity ID");
        //}

        template<typename ComponentType>
        ComponentWrapper<ComponentType> GetComponentWrapper()
        {
            SparseSet<ComponentType, KeyType>& compSet = GetComponentSet<ComponentType>();

            return ComponentWrapper<ComponentType>{ compSet };
        }

        template<typename ComponentType>
        const ComponentWrapper<ComponentType> GetComponentWrapper() const
        {
            const SparseSet<ComponentType, KeyType>& compSet = GetComponentSet<ComponentType>();

            return ComponentWrapper<ComponentType>{ compSet };
        }

    private:
        std::unordered_map<std::type_index, std::shared_ptr<void>> m_ComponentSetsMap;

        template<typename ComponentType>
        SparseSet<ComponentType, KeyType>& GetComponentSet()
        requires std::is_constructible_v<ComponentType> && std::is_default_constructible_v<KeyType>
        {
            const std::type_index compTypeIdx{ typeid(ComponentType) };

            auto it{ m_ComponentSetsMap.find(compTypeIdx) };

            if (it != m_ComponentSetsMap.end())
            {
                return *static_cast<SparseSet<ComponentType, KeyType>*>(it->second.get());
            }

            auto set{ std::make_shared<SparseSet<ComponentType, KeyType>>() };
            m_ComponentSetsMap[compTypeIdx] = set;
            return *set;
        }

        template<typename ComponentType>
        const SparseSet<ComponentType, KeyType>& GetComponentSet() const
            requires std::is_constructible_v<ComponentType>&& std::is_default_constructible_v<KeyType>
        {
            const std::type_index compTypeIdx{ typeid(ComponentType) };

            auto it{ m_ComponentSetsMap.find(compTypeIdx) };

            if (it != m_ComponentSetsMap.end())
            {
                return *static_cast<const SparseSet<ComponentType, KeyType>*>(it->second.get());
            }

            // If the component set doesn't exist, you may throw an exception or handle the case differently based on your requirements.
            throw std::runtime_error("Component set not found");
        }

        template<typename ComponentType>
        std::optional<const SparseSet<ComponentType, KeyType>*> GetComponentSet() const
        requires std::is_constructible_v<ComponentType>&& std::is_default_constructible_v<KeyType>
        {
            const std::type_index compTypeIdx{ typeid(ComponentType) };

            auto it{ m_ComponentSetsMap.find(compTypeIdx) };

            if (it != m_ComponentSetsMap.end())
            {
                return static_cast<const SparseSet<ComponentType, KeyType>*>(it->second.get());
            }

            return std::nullopt;
        }
    };
}

#endif
