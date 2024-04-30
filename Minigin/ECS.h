#ifndef ENTITYCOMPONENTSYSTEM
#define ENTITYCOMPONENTSYSTEM

#include "ComponentManager.h"
#include "ComponentWrapper.h"
#include "ConstComponentWrapper.h"
#include "EntityId.h"

#include "EntityManager.h"

/*ECS Documentation
* The templated functions that do not cast down to a typeid are templated because the return type is required. The others for uniformity
* It is recommended to use the Get function whenever you need to acces a specific component because references could become invalid frame - frame as the container changes
* 
* 
* TODO: 
* HasComponent<Types ...>()
* GetComponents<Type, Type>()  - return All entities with 2 given componennts
* 
* Update the sparseSet structure to use a sparse array
* 
* Other communication methods (?) Get might not be the most optimal in all scenarios, other methods could be nice (lower prio)
*   -> messaging system
*/

namespace Pengin
{   
    class ECS final
    {
    public:
        ECS() :
            m_ComponentManager{},
            m_EntityManager{ m_ComponentManager }
        { }

        ~ECS() = default;

        [[nodiscard]] const std::vector<EntityId>& GetAllEntities() const noexcept
        {
            return m_EntityManager.GetAllEntityIds();
        }

        [[nodiscard]] const EntityId CreateEntity() noexcept
        {
            return m_EntityManager.CreateEntity();
        }

        [[nodiscard]] bool Exists(const EntityId id) const noexcept
        {
            return m_EntityManager.HasEntity(id);
        }
         
        [[nodiscard]] const std::vector<std::type_index> GetAllComponents(const EntityId id)
        {
            return m_EntityManager.GetAllCompTypes(id);
        }

        bool DestroyEntity(const EntityId id) 
        {
            return m_EntityManager.DestroyEntity(id);
        }

        template<typename ComponentType, typename... Args>
        ComponentType& AddComponent(const EntityId id, Args&&... args)
        {
            m_EntityManager.AddComponent(typeid(ComponentType), id);
            auto pair = m_ComponentManager.AddComponent<ComponentType>(id, std::forward<Args>(args)...);

            return pair.first;
        }

        template<typename ComponentType>
        void RemoveComponent(const EntityId id)
        {
            m_EntityManager.RemoveComponent(typeid(ComponentType), id);
            m_ComponentManager.RemoveComponent(typeid(ComponentType), id);
        }

        template<typename ComponentType>
        [[nodiscard]] bool HasComponent(const EntityId id) const
        {
            return m_ComponentManager.HasComponent(typeid(ComponentType), id); 
        }

        [[nodiscard]] bool HasComponent(const EntityId id, const std::type_index typeIdx) const
        {
            return m_ComponentManager.HasComponent(typeIdx, id);
        }

        template<typename ComponentType>
        [[nodiscard]] ComponentType& GetComponent(const EntityId id)
        {
            return m_ComponentManager.GetComponent<ComponentType>(id);
        }

        template<typename ComponentType>
        [[nodiscard]] const ComponentType& GetComponent(const EntityId id) const
        {
            return m_ComponentManager.GetConstComponent<ComponentType>(id);
        }

        template<typename ComponentType>
        [[nodiscard]] ComponentWrapper<ComponentType> GetComponents() noexcept
        {
            return m_ComponentManager.GetComponentWrapper<ComponentType>();
        }

        template<typename ComponentType>
        [[nodiscard]] const ConstComponentWrapper<ComponentType> GetComponents() const noexcept
        {
            return m_ComponentManager.GetConstComponentWrapper<ComponentType>();
        } 
        
        //TODO Clear

        ECS(const ECS&) = delete;
        ECS(ECS&&) = delete;
        ECS& operator=(const ECS&) = delete;
        ECS& operator=(ECS&&) = delete;

    private:
        ComponentManager m_ComponentManager;
        EntityManager m_EntityManager;
    };

}
#endif
