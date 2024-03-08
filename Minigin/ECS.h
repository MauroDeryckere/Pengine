#ifndef ENTITYCOMPONENTSYSTEM
#define ENTITYCOMPONENTSYSTEM

#include "Entity.h"

#include "SparseSet.h"

#include "Components.h"
#include "ComponentManager.h"
#include "ComponentWrapper.h"

#include "EntityManager.h"

#include <vector>
#include <cassert>
#include <limits>

#include <iostream>
namespace Pengin
{
    class ECS
    {
    public:
        ECS():
            m_ComponentManager{},
            m_EntityManager{}
        { }

        const EntityId& CreateEntity()
        {
            return m_EntityManager.CreateEntity();
        }

        [[nodiscard]] bool Exists(const EntityId& id)
        {
            return m_EntityManager.HasEntity(id);
        }
         
        bool DestroyEntity(const EntityId& id) //TODO
        {
            id;
            return false;
        }
        //Destroy entity

        template<typename ComponentType, typename... Args>
        ComponentType& AddComponent(const EntityId& id, Args&&... args)
        {
            return m_ComponentManager.AddComponent<ComponentType>(id, std::forward<Args>(args)...);
        }

        template<typename ComponentType>
        void RemoveComponent(const EntityId& id)
        {
            m_ComponentManager.RemoveComponent<ComponentType>(id);
        }

        template<typename ComponentType>
        [[nodiscard]] bool HasComponent(const EntityId& id)
        {
            return m_ComponentManager.HasComponent<ComponentType>(id);
        }

        template<typename ComponentType>
        [[nodiscard]] ComponentType& GetComponent(const EntityId& id)
        {
            return m_ComponentManager.GetComponent<ComponentType>(id);
        }

        template<typename ComponentType>
        const ComponentType& GetComponent(const EntityId& id) const
        {
            return m_ComponentManager.GetComponent<ComponentType>(id);
        }

        template<typename ComponentType>
        auto GetComponents()
        {
            return m_ComponentManager.GetComponentWrapper<ComponentType>();
        }

        template<typename ComponentType>
        auto GetComponents() const
        {
            return m_ComponentManager.GetComponentWrapper<ComponentType>();
        }
        
    private:
        ComponentManager m_ComponentManager;
        EntityManager m_EntityManager;
    };

}
#endif

//struct relationship {
//std::size_t children{};
//entt::entity first{ entt::null };
//entt::entity prev{ entt::null };
//entt::entity next{ entt::null };
//entt::entity parent{ entt::null };
// ... other data members ...
//};
//struct transform {
//    static constexpr auto in_place_delete = true;
//
//    transform* parent;
//    // ... other data members ...
//};

