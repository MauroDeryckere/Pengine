#ifndef ENTITY
#define ENTITY

#include "CoreIncludes.h"

#include "Scene.h"
#include <memory>
#include <cassert>

#include "TransformComponent.h"
#include "UUIDComponent.h"

namespace Pengin
{
	class Entity final
	{
	public:
		Entity(const EntityId id, Scene* pScene) noexcept :
			m_EntityId{ id },
			m_pScene{ pScene }
		{
			assert(pScene && "pScene is nullptr, can not have an entity in a nullptr scene");
		}

		~Entity() = default;

		template <typename ComponentType, typename... Args>
		ComponentType& AddComponent(Args&&... args)
		{
			return m_pScene->m_Ecs.AddComponent<ComponentType>(m_EntityId, std::forward<Args>(args)...);
		}

		template <typename ComponentType>
		void RemoveComponent() noexcept
		{
			m_pScene->m_Ecs.RemoveComponent<ComponentType>(m_EntityId);
		}

		template <typename ComponentType>
		[[nodiscard]] ComponentType& GetComponent()
		{
			return m_pScene->m_Ecs.GetComponent<ComponentType>(m_EntityId);
		}

		template <typename ComponentType>
		[[nodiscard]] const ComponentType& GetComponent() const
		{
			return m_pScene->m_Ecs.GetComponent<ComponentType>(m_EntityId);
		}

		template <typename ComponentType>
		[[nodiscard]] bool HasComponent() const noexcept
		{
			return m_pScene->m_Ecs.HasComponent<ComponentType>(m_EntityId);
		}

		void SetParent(Entity parentEntity, bool keepWorldPos = false);
		void SetLocalPosition(const glm::vec3& position);

		const EntityId GetEntityId() const noexcept { return m_EntityId; }
		const GameUUID& GetUUID() const 
		{
			assert(HasComponent<UUIDComponent>() && "ERROR - entity without UUIDComp");
			return GetComponent<UUIDComponent>().uuid; 
		}

		operator bool() const noexcept { return (m_EntityId != NULL_ENTITY_ID); }

		Entity(const Entity& other) = default;
		Entity(Entity&& other) = default;
		Entity& operator=(const Entity& other) = default;
		Entity& operator=(Entity&& other) = default;

	private:
		EntityId m_EntityId{ NULL_ENTITY_ID };
		Scene* m_pScene;
	};
}

#endif