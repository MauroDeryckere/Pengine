#ifndef ENTITYCOMPONENTSYSTEM
#define ENTITYCOMPONENTSYSTEM

#include "Singleton.h"

#include "Entity.h"

#include "ComponentManager.h"
#include "ComponentWrapper.h"
#include "ConstComponentWrapper.h"

#include "EntityManager.h"

#include "EcsEventInter.h"

/*ECS Documentation
* The templated functions that do not cast down to a typeid are templated because the return type is required. The others for uniformity
* It is recommended to use the Get function whenever you need to acces a specific component because references could become invalid frame - frame as the container changes
* 
* TODO: 
* HasComponent<Types ...>()
* GetComponents<Type, Type>()  - return All entities with 2 given componennts
* 
* Other communication methods (?) Get might not be the most optimal in all scenarios, oother methods could be nice (lower prio)
*   -> messaging system
* Serialization
*/

namespace Pengin
{   
    class ECS final : public dae::Singleton<ECS>
    {
    public:
        const EntityId CreateEntity() noexcept
        {
            return m_EntityManager.CreateEntity();
        }

        [[nodiscard]] bool Exists(const EntityId id) noexcept
        {
            return m_EntityManager.HasEntity(id);
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

            if (pair.second)
            {
                m_ECSEventInter.SetObserverDirty(id, typeid(ComponentType));
            }

            return pair.first;
        }

        template<typename ComponentType>
        void RemoveComponent(const EntityId id)
        {
            const auto wrapper{ m_ComponentManager.GetConstComponentWrapper<ComponentType>() };
            const auto it{ std::prev(wrapper.cend()) };

            const EntityId lastId{ wrapper.GetIdFromIterator(it) };

            m_EntityManager.RemoveComponent(typeid(ComponentType), id);
            m_ComponentManager.RemoveComponent(typeid(ComponentType), id);

            m_ECSEventInter.SetObserverDirty(lastId, typeid(ComponentType)); //Moved in memory
        }

        template<typename ComponentType>
        [[nodiscard]] bool HasComponent(const EntityId id)
        {
            return m_ComponentManager.HasComponent(typeid(ComponentType), id); 
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
        [[nodiscard]] ComponentWrapper<ComponentType> GetComponents()
        {
            return m_ComponentManager.GetComponentWrapper<ComponentType>();
        }

        template<typename ComponentType>
        [[nodiscard]] const ConstComponentWrapper<ComponentType> GetComponents() const
        {
            return m_ComponentManager.GetConstComponentWrapper<ComponentType>();
        }
        
        //TODO Clear

        ECS(const ECS&) = delete;
        ECS(ECS&&) = delete;
        ECS& operator=(const ECS&) = delete;
        ECS& operator=(ECS&&) = delete;

    private:
        ECS() :
            m_ComponentManager{},
            m_EntityManager{ m_ComponentManager }
        { }
        ~ECS() = default;

        friend class dae::Singleton<ECS>;

        ComponentManager m_ComponentManager;
        EntityManager m_EntityManager;

        EcsEventInter m_ECSEventInter;
    };

}
#endif
