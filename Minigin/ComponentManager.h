#ifndef COMPONENTMANAGER
#define COMPONENTMANAGER

#include "SparseSet.h"
#include "EntityId.h"

#include "BaseComponentStorage.h"
#include "ComponentStorage.h"

#include "UniqueTypeTracker.h"

#include "ComponentWrapper.h"
#include "ConstComponentWrapper.h"

#include "DebugOutput.h"

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
        std::pair<ComponentType&, bool> AddComponent(const EntityId id, Args&&... args)
        {
            [[maybe_unused]] UniqueTypesTracker<ComponentType> tracker;

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

            auto pair{ set.Emplace(id, std::forward<Args>(args)...) };
            if (pair.first != set.end())
            {
                return {*pair.first, pair.second};
            }

            throw std::runtime_error("Failed to add component");
        }

        bool RemoveComponent(std::type_index typeIdx, const EntityId id) noexcept
        {
            const auto it{ m_TypeBitMap.find(typeIdx) };

            if (it == m_TypeBitMap.end())
            {
                DEBUG_OUT("Checking for a component that is not ever added to the typeTracker");
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

        [[nodiscard]] bool HasComponent(std::type_index typeIdx, const EntityId id) const
        {
            const auto it{ m_TypeBitMap.find(typeIdx) };

            if (it == m_TypeBitMap.end())
            {
                DEBUG_OUT("Checking for a component that is not ever added to the typeTracker");
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
        [[nodiscard]] ComponentType& GetComponent(const EntityId id)
        {
            const auto it{ m_TypeBitMap.find(typeid(ComponentType)) };

            if (it == m_TypeBitMap.end()) [[unlikely]]
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
            auto& set{ storage->GetSet() };

            if (set.Contains(id))
            {
                return set[id];
            }

            throw std::out_of_range("Component not found for the given entity ID");
        }


        template<typename ComponentType>
        [[nodiscard]] const ComponentType& GetConstComponent(const EntityId id) const
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
        [[nodiscard]] ComponentWrapper<ComponentType> GetComponentWrapper() noexcept
        {
            const auto it{ m_TypeBitMap.find(typeid(ComponentType)) };

            if (it == m_TypeBitMap.end())
            {
                DEBUG_OUT("Checking for a component that is not ever added to the typeTracker");
                return ComponentWrapper<ComponentType> { nullptr };
            }

            const size_t idx{ it->second };
            auto* basePtr{ m_ComponentStorage[idx].get() };

            ComponentStorage<ComponentType>* storage{ dynamic_cast<ComponentStorage<ComponentType>*>(basePtr) };

            if (!storage)
            {
                return ComponentWrapper<ComponentType> { nullptr };
            }

            auto& set{ storage->GetSet() };
            return ComponentWrapper<ComponentType>{ &set };
        }

        template<typename ComponentType>
        [[nodiscard]] const ConstComponentWrapper<ComponentType> GetConstComponentWrapper() const noexcept
        {
            const auto it{ m_TypeBitMap.find(typeid(ComponentType)) };

            if (it == m_TypeBitMap.end())
            {
                DEBUG_OUT("Checking for a component that is not ever added to the typeTracker");
                return ConstComponentWrapper<ComponentType> { nullptr };
            }

            const size_t idx{ it->second };

            auto* basePtr{ m_ComponentStorage[idx].get() };

            ComponentStorage<ComponentType>* storage{ dynamic_cast<ComponentStorage<ComponentType>*>(basePtr) };

            if (!storage)
            {
                return ConstComponentWrapper<ComponentType> { nullptr };
            }

            const auto& set{ storage->GetSet() };
            
            return ConstComponentWrapper<ComponentType>{&set};
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
