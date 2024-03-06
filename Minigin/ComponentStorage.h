#ifndef COMPONENTSTORAGE
#define COMPONENTSTORAGE

#include "Entity.h"

#include "SparseSet.h"
#include "ComponentWrapper.h"
#include "UniqueTypeTracker.h"

#include <memory>
#include <cassert>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <optional>

#include <unordered_set>

namespace Pengin
{
    using KeyType = EntityId;

    class ComponentStorage final
    {
    public:
        ComponentStorage()
        {
            std::cout << Pengin::GetGlobalUniqueTypesSet().size() << " types required \n";
        }

        ~ComponentStorage() = default;

        ComponentStorage(const ComponentStorage&) = delete;
        ComponentStorage& operator=(const ComponentStorage&) = delete;
        ComponentStorage(ComponentStorage&&) noexcept = delete;
        ComponentStorage& operator=(ComponentStorage&&) noexcept = delete;

        template<typename ComponentType, typename... Args>
        ComponentType& AddComponent(const EntityId& id, Args&&... args) 
        {
            [[maybe_unused]] UniqueTypesTracker<ComponentType> tracker; //TODO Change this

            SparseSet<ComponentType, KeyType>& compSet{ GetComponentSet<ComponentType>() };

            auto it{ compSet.Emplace(id, std::forward<Args>(args)...) };

            if (it != compSet.end()) {
                return *it;
            }

            throw std::runtime_error("Failed to add component");
        }

        template<typename ComponentType>
        void RemoveComponent(const EntityId& id) requires std::is_constructible_v<ComponentType>
        {
            SparseSet<ComponentType, KeyType>& compSet{ GetComponentSet<ComponentType>() };

            compSet.Remove(id);
        }

        template<typename ComponentType>
        [[nodiscard]] bool HasComponent(const EntityId& id) const
        {
            std::optional<const SparseSet<ComponentType, KeyType>*> compSetOptional{ GetOptComponentSet<ComponentType>() };

            if (compSetOptional.has_value()) 
            {
                return compSetOptional.value()->Contains(id);
            }

            return false;
        }

        template<typename ComponentType>
        [[nodiscard]] ComponentType& GetComponent(const EntityId& id)
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

        template<typename ComponentType>
        [[nodiscard]] const ComponentType& GetComponent(const EntityId& id) const
        {
            const SparseSet<ComponentType, KeyType>& compSet = GetComponentSet<ComponentType>();

            if (compSet.Contains(id))
            {
                return compSet[id];
            }

            throw std::out_of_range("Component not found for the given entity ID");
        }

        template<typename ComponentType>
        [[nodiscard]] ComponentWrapper<ComponentType> GetComponentWrapper()
        {
            SparseSet<ComponentType, KeyType>& compSet = GetComponentSet<ComponentType>();

            return ComponentWrapper<ComponentType>{ compSet };
        }

        template<typename ComponentType>
        [[nodiscard]] const ComponentWrapper<ComponentType> GetComponentWrapper() const
        {
            const SparseSet<ComponentType, KeyType>& compSet = GetComponentSet<ComponentType>();

            return ComponentWrapper<ComponentType>{ compSet };
        }

    private:
        std::unordered_map<std::type_index, std::shared_ptr<void>> m_ComponentSetsMap;

        template<typename ComponentType>
        [[nodiscard]] SparseSet<ComponentType, KeyType>& GetComponentSet()
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
        [[nodiscard]] const SparseSet<ComponentType, KeyType>& GetComponentSet() const
            requires std::is_constructible_v<ComponentType>&& std::is_default_constructible_v<KeyType>
        {
            const std::type_index compTypeIdx{ typeid(ComponentType) };

            auto it{ m_ComponentSetsMap.find(compTypeIdx) };

            if (it != m_ComponentSetsMap.end())
            {
                return *static_cast<const SparseSet<ComponentType, KeyType>*>(it->second.get());
            }

            throw std::runtime_error("Component set not found");
        }

        template<typename ComponentType>
        std::optional<const SparseSet<ComponentType, KeyType>*> GetOptComponentSet() const
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
