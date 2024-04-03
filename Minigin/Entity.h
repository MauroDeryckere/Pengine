#ifndef ENTITY
#define ENTITY

#include "EntityId.h"
#include "Scene.h"
#include <memory>
#include <cassert>

#include "TransformComponent.h"

namespace Pengin
{
	class Entity final
	{
	public:
		Entity(const EntityId id, std::weak_ptr<Scene> pScene) :
			m_EntityId{ id },
			m_pScene{ pScene }
		{}

		~Entity() = default;

		template <typename ComponentType, typename... Args>
		ComponentType& AddComponent(Args&&... args)
		{
			if (auto pScene{ m_pScene.lock() }; pScene)
			{

				return pScene->GetECS().AddComponent<ComponentType>(m_EntityId, std::forward<Args>(args)...);
			}

			throw std::runtime_error("Attempted to add component to an entity with invalid scene pointer");
		}

		template <typename ComponentType>
		void RemoveComponent()
		{
			if (auto pScene{ m_pScene.lock() }; pScene)
			{
				pScene->GetECS().RemoveComponent<ComponentType>(m_EntityId);
			}

			throw std::runtime_error("Attempted to remove component of an entity with invalid scene pointer");
		}

		template <typename ComponentType>
		[[nodiscard]] ComponentType& GetComponent()
		{
			if (auto pScene{ m_pScene.lock() }; pScene)
			{
				return pScene->GetECS().GetComponent<ComponentType>(m_EntityId);
			}

			throw std::runtime_error("Attempted to get component of an entity with invalid scene pointer");
		}

		template <typename ComponentType>
		[[nodiscard]] const ComponentType& GetComponent() const
		{
			if (auto pScene{ m_pScene.lock() }; pScene)
			{
				return pScene->GetECS().GetComponent<ComponentType>(m_EntityId);
			}

			throw std::runtime_error("Attempted to get component of an entity with invalid scene pointer");
		}

		template <typename ComponentType>
		[[nodiscard]] bool HasComponent()
		{
			if (auto pScene{ m_pScene.lock() }; pScene)
			{
				return pScene->GetECS().HasComponent<ComponentType>(m_EntityId);
			}

			throw std::runtime_error("Attempted to check existance of a component for an entity with invalid scene pointer");
		}

		void SetParent(Entity parentEntity, bool keepWorldPos = false)
		{
			assert(!parentEntity.m_pScene.expired() && !m_pScene.expired());

			auto& thisTransform = GetComponent<TransformComponent>();
			auto& newParentTransform = parentEntity.GetComponent<TransformComponent>();

			auto pScene{ m_pScene.lock() };

			if (IsParentChildOfThis(thisTransform, parentEntity.m_EntityId) || 
				parentEntity.m_EntityId == m_EntityId || 
				thisTransform.relation.parent == parentEntity.m_EntityId)
			{
				std::cerr << "can not set entity " << parentEntity.GetEntityId() << " as parent \n";
				return;
			}

			if (parentEntity.m_EntityId == NULL_ENTITY_ID)
			{
				thisTransform.localPos = GetWorldPosition(thisTransform);
				SetPosDirty(thisTransform);
			}
			else
			{
				if (keepWorldPos)
				{
					thisTransform.localPos = GetWorldPosition(thisTransform) - GetWorldPosition(newParentTransform);
				}

				SetPosDirty(thisTransform);
			}

			RemoveSelfAsChild(thisTransform);

			thisTransform.relation.parent = parentEntity.m_EntityId;

			AddSelfAsChild(thisTransform, newParentTransform);
		}

		void SetLocalPosition(const glm::vec3& position)
		{
			auto& thisTransform = GetComponent<TransformComponent>();
			thisTransform.localPos = position;

			SetPosDirty(thisTransform);
		}

		const EntityId GetEntityId() const { return m_EntityId; }
		operator bool() const { return (m_EntityId != NULL_ENTITY_ID && !m_pScene.expired()); }

		Entity(const Entity& other) = default;
		Entity(Entity&& other) = default;
		Entity& operator=(const Entity& other) = default;
		Entity& operator=(Entity&& other) = default;

	private:
		EntityId m_EntityId{ NULL_ENTITY_ID };
		std::weak_ptr<Scene> m_pScene;

		[[nodiscard]] bool IsParentChildOfThis(const TransformComponent& thisTransform, const EntityId parentId) const
		{
			auto pScene{ m_pScene.lock() };
			auto& ecs { pScene->GetECS() };

			if (thisTransform.relation.children == 0)
			{
				return false;
			}

			auto currChildId = thisTransform.relation.firstChild;
			assert(currChildId != NULL_ENTITY_ID);

			for (size_t childIdx {0}; childIdx  < thisTransform.relation.children; ++childIdx)
			{
				currChildId = ecs.GetComponent<TransformComponent>(currChildId).relation.nextSibling;

				if (currChildId == parentId)
				{
					return true;
				}
			}

			return false;
		}

		void SetPosDirty(TransformComponent& transform)
		{
			transform.isPosDirty = true;

			if (transform.relation.children != 0)
			{
				auto currChildId = transform.relation.firstChild;

				for (size_t childIdx{ 0 }; childIdx < transform.relation.children; ++childIdx)
				{
					auto& currChildTransform = m_pScene.lock()->m_Ecs.GetComponent<TransformComponent>(currChildId);

					SetPosDirty(currChildTransform);
					currChildId = currChildTransform.relation.nextSibling;
				}
			}
		}

		const glm::vec3& GetWorldPosition(TransformComponent& transform)
		{
			if (transform.isPosDirty)
			{
				UpdateWorldPosition(transform);
			}

			return transform.worldPos;
		}

		void UpdateWorldPosition(TransformComponent& transform)
		{
			auto scene = m_pScene.lock();
			assert(scene);

			if (transform.isPosDirty)
			{
				if (transform.relation.parent == NULL_ENTITY_ID)
				{
					transform.worldPos = transform.localPos;
				}
				else
				{
					transform.worldPos = GetWorldPosition(scene->m_Ecs.GetComponent<TransformComponent>(transform.relation.parent)) + transform.localPos;
				}
			}
			transform.isPosDirty = false;
		}

		void RemoveSelfAsChild(TransformComponent& self)
		{
			auto pScene = m_pScene.lock();
			assert(pScene);

			//Remove yourself as a child
			if (self.relation.parent != NULL_ENTITY_ID)
			{
				auto& oldParentTransform{ pScene->m_Ecs.GetComponent<TransformComponent>(self.relation.parent) };

				--oldParentTransform.relation.children;

				//Only child
				if (oldParentTransform.relation.children == 0)
				{
					oldParentTransform.relation.firstChild = NULL_ENTITY_ID;
				}
				//First child and there are other children
				else if (oldParentTransform.relation.firstChild == m_EntityId)
				{
					auto prevSibId = self.relation.prevSibling;
					auto nexSibId = self.relation.nextSibling;

					auto& nextSibTransform{ pScene->m_Ecs.GetComponent<TransformComponent>(nexSibId) };
					nextSibTransform.relation.prevSibling = prevSibId;

					oldParentTransform.relation.firstChild = nexSibId;
				}
				//Other children and not first child
				else
				{
					auto prevSibId = self.relation.prevSibling;
					auto nexSibId = self.relation.nextSibling;

					if (nexSibId != NULL_ENTITY_ID)
					{
						auto& nextSibTransform{ pScene->m_Ecs.GetComponent<TransformComponent>(nexSibId) };
						nextSibTransform.relation.prevSibling = prevSibId;
					}

					auto& prevSibTransform{ pScene->m_Ecs.GetComponent<TransformComponent>(prevSibId) };
					prevSibTransform.relation.nextSibling = nexSibId;
				}
			}
		}

		void AddSelfAsChild(TransformComponent& self, TransformComponent& parent)
		{
			auto pScene = m_pScene.lock();
			assert(pScene);

			//Add yourself as a child
			if (self.relation.parent != NULL_ENTITY_ID)
			{
				++parent.relation.children;

				auto firstChildId{ parent.relation.firstChild };

				parent.relation.firstChild = m_EntityId;

				if (parent.relation.children != 1)
				{
					auto& firstChildTransfom{ pScene->m_Ecs.GetComponent<TransformComponent>(firstChildId) };
					firstChildTransfom.relation.prevSibling = m_EntityId;
				}

				self.relation.prevSibling = NULL_ENTITY_ID;
				self.relation.nextSibling = firstChildId;
			}
		}
	};
}

#endif