#ifndef ENTITYCOMPONENTSYSTEM
#define ENTITYCOMPONENTSYSTEM

#include "ComponentManager.h"
#include "ComponentWrapper.h"
#include "ConstComponentWrapper.h"
#include "EntityId.h"

#include "EntityManager.h"

/*ECS Documentation
* It is recommended to use the Get function whenever you need to acces a specific component because references could become invalid frame - frame as the container changes
* 
* TODO: 
* Update the sparseSet structure to use a sparse array

* HasComponent<Types ...>()
* GetComponents<Type, Type>()  - return All entities with 2 given componennts* 
*/

namespace Pengin
{   
    class ECS final
    {
    public:
        ECS() :
            m_ComponentManager{ },
            m_EntityManager{ }
        { }

        ~ECS()
        { }

        void CleanUpDestroys() noexcept
        {
            std::for_each(m_EntitiesToDestroy.begin(), m_EntitiesToDestroy.end(), [this](const EntityId id) 
                { 
                    const auto ownedCompTypes{ m_EntityManager.GetAllCompTypes(id) };
                    for (const auto& comp : ownedCompTypes)
                    {
                        m_ComponentManager.RemoveComponent(comp, id);
                    }

                    m_EntityManager.DestroyEntity(id); 
                });
            m_EntitiesToDestroy.clear();

            std::for_each(m_ComponentsToRemove.begin(), m_ComponentsToRemove.end(), [this](const CompRemovalData& rmData) 
                {            
                    m_EntityManager.RemoveComponent(rmData.typeId, rmData.id);
                    m_ComponentManager.RemoveComponent(rmData.typeId, rmData.id);
                });
            m_ComponentsToRemove.clear();
        }

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

        void DestroyEntity(const EntityId id) noexcept
        {
            m_EntitiesToDestroy.emplace_back(id);
        }

        template<typename ComponentType, typename... Args>
        ComponentType& AddComponent(const EntityId id, Args&&... args)
        {
            m_EntityManager.AddComponent(typeid(ComponentType), id);
            auto pair = m_ComponentManager.AddComponent<ComponentType>(id, std::forward<Args>(args)...);

            return pair.first;
        }

        template<typename ComponentType>
        void RemoveComponent(const EntityId id) noexcept
        {
            m_ComponentsToRemove.emplace_back(id, typeid(ComponentType));
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
        
        ECS(const ECS&) = delete;
        ECS(ECS&&) = delete;
        ECS& operator=(const ECS&) = delete;
        ECS& operator=(ECS&&) = delete;

    private:
        ComponentManager m_ComponentManager;
        EntityManager m_EntityManager;

        struct CompRemovalData final
        {
            const EntityId id;
            std::type_index typeId;

            CompRemovalData(EntityId _id, std::type_index _typeId) :
                id{ _id },
                typeId{ _typeId }
            {}
        };

        std::vector<EntityId> m_EntitiesToDestroy{};
        std::vector<CompRemovalData> m_ComponentsToRemove{};
    };

}
#endif
