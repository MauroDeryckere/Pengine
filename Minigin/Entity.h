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
		Entity(const EntityId id, std::weak_ptr<Scene> pScene) noexcept :
			m_EntityId{ id },
			m_pScene{ pScene }
		{}

		~Entity() = default;

		template <typename ComponentType, typename... Args>
		ComponentType& AddComponent(Args&&... args)
		{
			if (auto pScene{ m_pScene.lock() }; pScene)
			{
				return pScene->m_Ecs.AddComponent<ComponentType>(m_EntityId, std::forward<Args>(args)...);
			}
			throw std::runtime_error("Attempted to add component to an entity with invalid scene pointer");
		}

		template <typename ComponentType>
		void RemoveComponent()
		{
			if (auto pScene{ m_pScene.lock() }; pScene)
			{
				pScene->m_Ecs.RemoveComponent<ComponentType>(m_EntityId);
			}
			throw std::runtime_error("Attempted to remove component of an entity with invalid scene pointer");
		}

		template <typename ComponentType>
		[[nodiscard]] ComponentType& GetComponent()
		{
			if (auto pScene{ m_pScene.lock() }; pScene)
			{
				return pScene->m_Ecs.GetComponent<ComponentType>(m_EntityId);
			}
			throw std::runtime_error("Attempted to get component of an entity with invalid scene pointer");
		}

		template <typename ComponentType>
		[[nodiscard]] const ComponentType& GetComponent() const
		{
			if (auto pScene{ m_pScene.lock() }; pScene)
			{
				return pScene->m_Ecs.GetComponent<ComponentType>(m_EntityId);
			}
			throw std::runtime_error("Attempted to get component of an entity with invalid scene pointer");
		}

		template <typename ComponentType>
		[[nodiscard]] bool HasComponent() const
		{
			if (auto pScene{ m_pScene.lock() }; pScene)
			{
				return pScene->m_Ecs.HasComponent<ComponentType>(m_EntityId);
			}
			throw std::runtime_error("Attempted to check existance of a component for an entity with invalid scene pointer");
		}

		void SetParent(Entity parentEntity, bool keepWorldPos = false);
		void SetLocalPosition(const glm::vec3& position);

		const EntityId GetEntityId() const noexcept { return m_EntityId; }
		const GameUUID& GetUUID() const 
		{
			assert(HasComponent<UUIDComponent>() && "ERROR - entity without UUIDComp");
			return GetComponent<UUIDComponent>().uuid; 
		}

		operator bool() const noexcept { return (m_EntityId != NULL_ENTITY_ID && !m_pScene.expired()); }

		Entity(const Entity& other) = default;
		Entity(Entity&& other) = default;
		Entity& operator=(const Entity& other) = default;
		Entity& operator=(Entity&& other) = default;

	private:
		EntityId m_EntityId{ NULL_ENTITY_ID };
		std::weak_ptr<Scene> m_pScene;

		[[nodiscard]] bool IsParentChildOfThis(const TransformComponent& thisTransform, const EntityId parentId) const;

		void SetPosDirty(TransformComponent& transform);
		void DestroyChildren(TransformComponent& transform);

		const glm::vec3& GetWorldPosition(TransformComponent& transform); //&& update
		void UpdateWorldPosition(TransformComponent& transform);

		void RemoveSelfAsChild(TransformComponent& self);
		void AddSelfAsChild(TransformComponent& self, TransformComponent& parent);
	};
}

#endif