#include "Entity.h"

#include "SpriteComponent.h"

namespace Pengin
{
	void Entity::SetParent(Entity parentEntity, bool keepWorldPos)
	{
		assert(!parentEntity.m_pScene.expired() && !m_pScene.expired());

		auto& thisTransform = GetComponent<TransformComponent>();
		auto pScene{ m_pScene.lock() };

		if ((IsParentChildOfThis(thisTransform, parentEntity.m_EntityId) ||
			parentEntity.m_EntityId == m_EntityId ||
			thisTransform.relation.parent == parentEntity.m_EntityId) && parentEntity.m_EntityId != NULL_ENTITY_ID)
		{
			DEBUG_OUT("can not set entity " << parentEntity.GetEntityId() << " as parent for entity " << m_EntityId);
			return;
		}

		if (parentEntity.m_EntityId == NULL_ENTITY_ID)
		{
			if (keepWorldPos) //Keep worldPos upon deletion of a parent means we keep the children at their current world pos (and their relationships)
			{
				thisTransform.localPos = GetWorldPosition(thisTransform);
				SetPosDirty(thisTransform);

				if (thisTransform.relation.children > 0)
				{
					auto currChildId = thisTransform.relation.firstChild;

					for (size_t childIdx = 0; childIdx < thisTransform.relation.children; ++childIdx)
					{
						auto& currChildTransform = pScene->m_Ecs.GetComponent<TransformComponent>(currChildId);

						currChildTransform.localPos = thisTransform.localPos + currChildTransform.localPos; //set all childrens positions to the correct world position

						currChildTransform.relation.parent = NULL_ENTITY_ID;
						currChildId = currChildTransform.relation.nextSibling;
					}
				}
			}
			else //otherwise all children and relationships get destroyed
			{
				DestroyChildren(thisTransform);
			}
		}
		else
		{
			if (keepWorldPos)
			{
				auto& newParentTransform = parentEntity.GetComponent<TransformComponent>();
				thisTransform.localPos = GetWorldPosition(thisTransform) - GetWorldPosition(newParentTransform);
			}

			SetPosDirty(thisTransform);
		}

		RemoveSelfAsChild(thisTransform);
		thisTransform.relation.parent = parentEntity.m_EntityId;

		if (parentEntity.m_EntityId != NULL_ENTITY_ID)
		{
			auto& newParentTransform = parentEntity.GetComponent<TransformComponent>();
			AddSelfAsChild(thisTransform, newParentTransform);
		}
	}

	void Entity::SetLocalPosition(const glm::vec3& position)
	{
		auto& thisTransform = GetComponent<TransformComponent>();
		thisTransform.localPos = position;

		SetPosDirty(thisTransform);
	}

	void Entity::OutputEntityData() const noexcept
	{
		auto pScene = m_pScene.lock();

		assert(pScene);

		DEBUG_OUT("Printing entity data");

		if (pScene->m_Ecs.HasComponent<TransformComponent>(m_EntityId))
		{
			const auto& transform = pScene->m_Ecs.GetComponent<TransformComponent>(m_EntityId);
			DEBUG_OUT("Transform Component\n");

			DEBUG_OUT("World pos " << transform.worldPos.x << " " << transform.worldPos.y << " " << transform.worldPos.z);
			DEBUG_OUT("Local pos " << transform.localPos.x << " " << transform.localPos.y << " " << transform.localPos.z);
			DEBUG_OUT("Rotation " << transform.rotation.x << " " << transform.rotation.y << " " << transform.rotation.z);
			DEBUG_OUT("Scale " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z);

			DEBUG_OUT("Relation ");
			DEBUG_OUT("Children " << transform.relation.children);
			DEBUG_OUT("First Child  " << transform.relation.firstChild);
			DEBUG_OUT("Previous Sibling " << transform.relation.prevSibling);
			DEBUG_OUT("Parent " << transform.relation.parent);

			DEBUG_OUT("Is pos dirty " << transform.isPosDirty << "\n");
		}
		if (pScene->m_Ecs.HasComponent<SpriteComponent>(m_EntityId))
		{
			const auto& sprite = pScene->m_Ecs.GetComponent<SpriteComponent>(m_EntityId);
			DEBUG_OUT("Sprite Component\n");

			DEBUG_OUT("Is Visible " << sprite.isVisible);
			DEBUG_OUT("Source rect " << sprite.m_SourceRect.x << " " << sprite.m_SourceRect.y << " " << sprite.m_SourceRect.width << " " << sprite.m_SourceRect.height);
			DEBUG_OUT("Texture " << sprite.m_pTexture->GetPath() << "\n");
		}
	}

	bool Entity::IsParentChildOfThis(const TransformComponent& thisTransform, const EntityId parentId) const
	{
		auto pScene{ m_pScene.lock() };
		auto& ecs{ pScene->GetECS() };

		if (thisTransform.relation.children == 0)
		{
			return false;
		}

		auto currChildId = thisTransform.relation.firstChild;
		assert(currChildId != NULL_ENTITY_ID);

		for (size_t childIdx{ 0 }; childIdx < thisTransform.relation.children; ++childIdx)
		{
			currChildId = ecs.GetComponent<TransformComponent>(currChildId).relation.nextSibling;

			if (currChildId == parentId)
			{
				return true;
			}
		}

		return false;
	}

	void Entity::SetPosDirty(TransformComponent& transform)
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
	void Entity::DestroyChildren(TransformComponent& transform)
	{
		if (transform.relation.children != 0)
		{
			auto pScene = m_pScene.lock();
			assert(pScene);

			auto currChildId = transform.relation.firstChild;

			for (size_t childIdx{ 0 }; childIdx < transform.relation.children; ++childIdx)
			{
				auto& childTransform = pScene->m_Ecs.GetComponent<TransformComponent>(currChildId);
				const auto nextSiblingId = childTransform.relation.nextSibling;

				DestroyChildren(childTransform);

				pScene->m_Ecs.DestroyEntity(currChildId);
				currChildId = nextSiblingId;
			}
		}
	}
	const glm::vec3& Entity::GetWorldPosition(TransformComponent& transform)
	{
		if (transform.isPosDirty)
		{
			UpdateWorldPosition(transform);
		}

		return transform.worldPos;
	}
	void Entity::UpdateWorldPosition(TransformComponent& transform)
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

	void Entity::RemoveSelfAsChild(TransformComponent& self)
	{
		auto pScene = m_pScene.lock();
		assert(pScene);

		//Remove yourself as a child
		if (self.relation.parent != NULL_ENTITY_ID)
		{
			auto& oldParentTransform{ pScene->m_Ecs.GetComponent<TransformComponent>(self.relation.parent) };

			//Only child
			if (oldParentTransform.relation.children == 1)
			{
				--oldParentTransform.relation.children;

				oldParentTransform.relation.firstChild = NULL_ENTITY_ID;
			}

			//First child and there are other children
			else if (oldParentTransform.relation.firstChild == m_EntityId)
			{
				--oldParentTransform.relation.children;

				auto prevSibId = self.relation.prevSibling;
				auto nexSibId = self.relation.nextSibling;

				auto& nextSibTransform{ pScene->m_Ecs.GetComponent<TransformComponent>(nexSibId) };
				nextSibTransform.relation.prevSibling = prevSibId;

				oldParentTransform.relation.firstChild = nexSibId;
			}
			//Other children and not first child
			else
			{
				--oldParentTransform.relation.children;

				auto prevSibId = self.relation.prevSibling;
				auto nexSibId = self.relation.nextSibling;

				if (nexSibId != NULL_ENTITY_ID)
				{
					auto& nextSibTransform{ pScene->m_Ecs.GetComponent<TransformComponent>(nexSibId) };
					nextSibTransform.relation.prevSibling = prevSibId;
				}
				if (prevSibId != NULL_ENTITY_ID)
				{
					auto& prevSibTransform{ pScene->m_Ecs.GetComponent<TransformComponent>(prevSibId) };
					prevSibTransform.relation.nextSibling = nexSibId;
				}
			}
		}
	}

	void Entity::AddSelfAsChild(TransformComponent& self, TransformComponent& parent)
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
}


