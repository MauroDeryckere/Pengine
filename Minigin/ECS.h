#ifndef ENTITYCOMPONENTSYSTEM
#define ENTITYCOMPONENTSYSTEM

#include "Entity.h"

#include "SparseSet.h"

#include "Components.h"
#include "ComponentStorage.h"
#include "ComponentWrapper.h"

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
            m_ComponentStorage{}
        {

            AddComponent<TestComponent>(1);
            std::cout << HasComponent<TestComponent>(1) << "\n";
            AddComponent<TestComponent>(2, 4);
            std::cout << HasComponent<TestComponent>(2) << "\n";

            auto& comp = GetComponent<TestComponent>(2);
            std::cout << comp.randomintfortesting << "\n";

            auto componentWrapper(GetComponents<TestComponent>());

            for (auto& component : componentWrapper) 
            {
                std::cout << component.randomintfortesting << "\n";
            }

        };

        //Create Entity -- possible overload for entities within a range
        //Destroy entity
        //Release for entities that dont have any components - optional

        //IsValid(entity id)

        template<typename ComponentType, typename... Args>
        void AddComponent(const EntityId& id, Args&&... args)
        {
            m_ComponentStorage.AddComponent<ComponentType>(id, std::forward<Args>(args)...);
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
        auto GetComponents()
        {
            return m_ComponentStorage.GetComponentWrapper<ComponentType>();
        }

        //HasAnyOf<Types>
        //HasAllOf<Types>

        //possibly allow for replacing components (updating with new component)

        //GetAllComponents<Type>

        //GetAllEntitiesContaining<Type>()
        //GetAllEntitiesContaining<Type,Type>()
        
        
    private:
        ComponentStorage m_ComponentStorage;
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

