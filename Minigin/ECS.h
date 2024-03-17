#ifndef ENTITYCOMPONENTSYSTEM
#define ENTITYCOMPONENTSYSTEM

#include "Entity.h"

#include "Components.h"
#include "ComponentManager.h"
#include "ComponentWrapper.h"

#include "EntityManager.h"

/*ECS Documentation
* The templated functions that do not cast down to a typeid are templated because the return type is required. The others for uniformity
* 
* TODO: 
* HasComponent<Types ...>()
* GetComponents<Type, Type>()  - return All entities with 2 given componennts
* 
* Other communication methods (?)
* 
* Serialization
*/

namespace Pengin
{
    class ECS
    {
    public:
        ECS():
            m_ComponentManager{},
            m_EntityManager{m_ComponentManager}
        { }

        const EntityId& CreateEntity() noexcept
        {
            return m_EntityManager.CreateEntity();
        }

        [[nodiscard]] bool Exists(const EntityId& id) noexcept
        {
            return m_EntityManager.HasEntity(id);
        }
         
        bool DestroyEntity(const EntityId& id)
        {
            return m_EntityManager.DestroyEntity(id);
        }

        template<typename ComponentType, typename... Args>
        ComponentType& AddComponent(const EntityId& id, Args&&... args)
        {
            m_EntityManager.AddComponent(typeid(ComponentType), id);
            return m_ComponentManager.AddComponent<ComponentType>(id, std::forward<Args>(args)...);
        }

        template<typename ComponentType>
        void RemoveComponent(const EntityId& id)
        {
            m_EntityManager.RemoveComponent(typeid(ComponentType), id);
            m_ComponentManager.RemoveComponent(typeid(ComponentType), id);
        }

        template<typename ComponentType>
        [[nodiscard]] bool HasComponent(const EntityId& id)
        {
            return m_ComponentManager.HasComponent(typeid(ComponentType), id); 
        }

        //TODO  entitymanager for multiple types

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
