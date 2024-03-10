#ifndef COMPONENTMANAGER
#define COMPONENTMANAGER

#include "Entity.h"
#include "SparseSet.h"

#include "BaseComponentStorage.h"
#include "ComponentStorage.h"

#include "UniqueTypeTracker.h"

#include "ComponentWrapper.h"

#include <memory>
#include <cassert>
#include <unordered_map>
#include <typeindex>
#include <type_traits>

namespace Pengin
{    
    class ComponentManager final
    {
    public:
        ComponentManager() :
            m_TypeBitMap{ InitTypeBitMapping() }
        {
            m_ComponentStorage.resize(m_TypeBitMap.size());
        }

        ~ComponentManager() = default;

        ComponentManager(const ComponentManager&) = delete;
        ComponentManager& operator=(const ComponentManager&) = delete;
        ComponentManager(ComponentManager&&) noexcept = delete;
        ComponentManager& operator=(ComponentManager&&) noexcept = delete;

        template<typename ComponentType, typename... Args>
        ComponentType& AddComponent(const EntityId& id, Args&&... args)
        {
            [[maybe_unused]] UniqueTypesTracker<ComponentType> tracker; //TODO Change this

            const size_t idx{ m_TypeBitMap.at(typeid(ComponentType))};

            auto* basePtr{ m_ComponentStorage[idx].get()};

            if (!basePtr)
            {
                m_ComponentStorage[idx].reset(static_cast<BaseComponentStorage*>(new ComponentStorage<ComponentType>{}));
                basePtr = m_ComponentStorage[idx].get();
            }

            ComponentStorage<ComponentType>* storage{ dynamic_cast<ComponentStorage<ComponentType>*>(basePtr) };

            assert(storage);

            auto& set{ storage->GetSet() };
            auto it{ set.Emplace(id, std::forward<Args>(args)...) };
            if (it != set.end())
            {
                return *it; 
            }

            throw std::runtime_error("Failed to add component");
        }

        bool RemoveComponent(std::type_index typeIdx, const EntityId& id) noexcept
        {
            const auto it{ m_TypeBitMap.find(typeIdx) };

            if (it == m_TypeBitMap.end())
            {
                std::cerr << "Checking for a component that is not ever added to the typeTracker \n";
                return false;
            }

            const size_t idx{ it->second };

            auto* basePtr{ m_ComponentStorage[idx].get() };
            if (!basePtr)
            {
                return false;
            }

            basePtr->RemoveComponent(id);
            return true;
        }

        [[nodiscard]] bool HasComponent(std::type_index typeIdx, const EntityId& id) const
        {
            const auto it{ m_TypeBitMap.find(typeIdx) };

            if (it == m_TypeBitMap.end())
            {
                std::cerr << "Checking for a component that is not ever added to the typeTracker \n";
                return false;
            }

            const size_t idx{ it->second };

            auto* const basePtr{ m_ComponentStorage[idx].get() };

            if (!basePtr)
            {
                return false;
            }
            
            return basePtr->HasComponent(id);
        }

        template<typename ComponentType>
        [[nodiscard]] ComponentType& GetComponent(const EntityId& id)
            requires std::is_default_constructible_v<ComponentType>
        {
            const auto typeIt{ m_TypeBitMap.find(typeid(ComponentType)) };

            if (typeIt == m_TypeBitMap.end())
            {
                throw std::out_of_range("Checking for a component that is not ever added to the typeTracker");
            }

            const size_t idx{ typeIt->second };

            auto* basePtr{ m_ComponentStorage[idx].get() };

            if (!basePtr)
            {
                m_ComponentStorage[idx].reset(static_cast<BaseComponentStorage*>(new ComponentStorage<ComponentType>{}));
                basePtr = m_ComponentStorage[idx].get();

                ComponentStorage<ComponentType>* const storage{ dynamic_cast<ComponentStorage<ComponentType>*>(basePtr) };
                auto& set{ storage->GetSet() };

                auto it{ set.Emplace(id) };

                return *it;
            }

            ComponentStorage<ComponentType>* storage{ dynamic_cast<ComponentStorage<ComponentType>*>(basePtr) };
            auto& set{ storage->GetSet() };

            if (!set.Contains(id))
            {
                auto it{ set.Emplace(id) };

                if (it != set.end())
                {
                    return *it;
                }
            }

            return set[id];
        }

        template<typename ComponentType>
        [[nodiscard]] const ComponentType& GetComponent(const EntityId& id) const
        {
            const auto it{ m_TypeBitMap.find(typeid(ComponentType)) };

            if (it == m_TypeBitMap.end())
            {
                throw std::out_of_range("Checking for a component that is not ever added to the typeTracker");
            }

            const size_t idx{ it->second };
            auto* basePtr{ m_ComponentStorage[idx].get() };

            if (!basePtr)
            {
                throw std::out_of_range("Component not found for the given entity ID");
            }

            ComponentStorage<ComponentType>* const storage{ dynamic_cast<ComponentStorage<ComponentType>*>(basePtr) };
            const auto& set{ storage->GetSet() };

            if (set.Contains(id))
            {
                return set[id];
            }
            throw std::out_of_range("Component not found for the given entity ID");
        }

        template<typename ComponentType>
        [[nodiscard]] ComponentWrapper<ComponentType> GetComponentWrapper()
        {
            const auto it{ m_TypeBitMap.find(typeid(ComponentType)) };

            if (it == m_TypeBitMap.end())
            {
                throw std::out_of_range("Checking for a component that is not ever added to the typeTracker");
            }

            const size_t idx{ it->second };
            auto* basePtr{ m_ComponentStorage[idx].get() };

            ComponentStorage<ComponentType>* storage{ dynamic_cast<ComponentStorage<ComponentType>*>(basePtr) };

            auto& set{ storage->GetSet() };
            return ComponentWrapper<ComponentType>{ set };
        }

        template<typename ComponentType>
        [[nodiscard]] const ComponentWrapper<ComponentType> GetComponentWrapper() const
        {
            const auto it{ m_TypeBitMap.find(typeid(ComponentType)) };

            if (it == m_TypeBitMap.end())
            {
                throw std::out_of_range("Checking for a component that is not ever added to the typeTracker");
            }

            const size_t idx{ it->second };

            auto* basePtr{ m_ComponentStorage[idx].get() };

            ComponentStorage<ComponentType>* storage{ dynamic_cast<ComponentStorage<ComponentType>*>(basePtr) };

            const auto& set{ storage->GetSet() };
            return ComponentWrapper<ComponentType>{ set };
        }

    private:
        std::vector<std::unique_ptr<BaseComponentStorage>> m_ComponentStorage;

        const std::unordered_map<std::type_index, size_t> m_TypeBitMap;

        [[nodiscard]] const std::unordered_map<std::type_index, size_t> InitTypeBitMapping() noexcept
        {
            size_t bit{ 0 };
            std::unordered_map<std::type_index, size_t> typeBitMap;

            for (const auto& type : UniqueTypes::GetConstSet())
            {
                typeBitMap.insert({ type, bit });
                ++bit;
            }

            return typeBitMap;
        }
    };
}

#endif
