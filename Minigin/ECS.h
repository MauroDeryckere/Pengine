#ifndef ENTITYCOMPONENTSYSTEM
#define ENTITYCOMPONENTSYSTEM

#include "SparseSet.h"

#include "Components.h"
#include "ComponentStorage.h"

#include <vector>
#include <cassert>
#include <limits>

#include <iostream>
namespace Pengin
{
    using Entity = unsigned;

    class ECS
    {
    public:
        ECS():
            m_ComponentStorage{}
        {

            m_ComponentStorage.AddComponent<TestComponent>(1);
            std::cout << m_ComponentStorage.HasComponent<TestComponent>(1) << "\n";
            m_ComponentStorage.AddComponent<TestComponent>(2, 4);
            std::cout << m_ComponentStorage.HasComponent<TestComponent>(2) << "\n";

            auto& comp = m_ComponentStorage.GetComponent<TestComponent>(2);
            std::cout << comp.randomintfortesting << "\n";

        };

        //Create Entity -- possible overload for entities within a range
        //Destroy entity
        //Release for entities that dont have any components - optional

        //IsValid(entity id)

        //AddComponent<Type>(id)
        //AddComponent<Type>(id, constructor) 
        //RemoveComponent<Type>(id) 
        //HasComponent<Type>(id)
        //GetComponent<Type>(id)

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

