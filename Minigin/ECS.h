#ifndef ENTITYCOMPONENTSYSTEM
#define ENTITYCOMPONENTSYSTEM

#include "Entity.h"

#include "SparseSet.h"

#include "Components.h"
#include "ComponentStorage.h"
#include "ComponentWrapper.h"

#include "EntityStorage.h"

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
            m_ComponentStorage{},
            m_EntityStorage{}
        {


        };

        const EntityId& CreateEntity()
        {
            return m_EntityStorage.CreateEntity();
        }
        bool Exists(const EntityId& id)
        {
            return m_EntityStorage.HasEntity(id);
        }
         
        bool DestroyEntity(const EntityId& id) //TODO
        {
            id;
            return false;
        }
        //Destroy entity

        template<typename ComponentType, typename... Args>
        bool AddComponent(const EntityId& id, Args&&... args) //return added component here TOOD
        {
            return m_ComponentStorage.AddComponent<ComponentType>(id, std::forward<Args>(args)...);
        }

        template<typename ComponentType>
        void RemoveComponent(const EntityId& id)
        {
            m_ComponentStorage.RemoveComponent<ComponentType>(id);
        }

        template<typename ComponentType>
        bool HasComponent(const EntityId& id)
        {
            return m_ComponentStorage.HasComponent<ComponentType>(id);
        }

        template<typename ComponentType>
        ComponentType& GetComponent(const EntityId& id)
        {
            return m_ComponentStorage.GetComponent<ComponentType>(id);
        }

        template<typename ComponentType>
        ComponentType& GetComponent(const EntityId& id) const
        {
            return m_ComponentStorage.GetComponent<ComponentType>(id);
        }

        template<typename ComponentType>
        auto GetComponents()
        {
            return m_ComponentStorage.GetComponentWrapper<ComponentType>();
        }

        template<typename ComponentType>
        auto GetComponents() const
        {
            return m_ComponentStorage.GetComponentWrapper<ComponentType>();
        }

        //HasAnyOf<Types>
        //HasAllOf<Types>

        //possibly allow for replacing components (updating with new component)

        //GetAllEntitiesContaining<Type>()
        //GetAllEntitiesContaining<Type,Type>()
        
        
    private:
        ComponentStorage m_ComponentStorage;
        EntityStorage m_EntityStorage;
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

