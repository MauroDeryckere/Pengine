#ifndef TRANSFORMCOMPONENT
#define TRANSFORMCOMPONENT

#include "glm/vec3.hpp"
#include "EntityID.h"

#include "ECS.h"

namespace Pengin
{
	struct TransformComponent final
	{
		glm::vec3 worldPos{ 0, 0, 0 };
		glm::vec3 localPos{ 0, 0, 0 };
		glm::vec3 rotation{ 0, 0, 0 };
		glm::vec3 scale{ 1, 1, 1 };

		bool isPosDirty{ false };

		struct Relationship final
		{
			uint32_t children{ 0 };
			EntityId firstChild{ NULL_ENTITY_ID };
			EntityId prevSibling{ NULL_ENTITY_ID };
			EntityId nextSibling{ NULL_ENTITY_ID };
			EntityId parent{ NULL_ENTITY_ID };
		};

		Relationship relation { };

		TransformComponent(const glm::vec3& pos = { 0, 0, 0 }, const glm::vec3& rotation = { 0, 0, 0 }, const glm::vec3& scale = { 1, 1, 1 }) :
			worldPos{ pos },
			localPos{ pos },
			rotation{ rotation },
			scale{ scale },
			isPosDirty{ false },
			relation{ }
		{ }

		~TransformComponent() = default;
	};

	//TODO: consider iterative over recursive
	class SceneGraph final //Static class for easy interface with transform component scene graph
	{
	public:
		[[nodiscard]] static const glm::vec3& GetWorldPosition(TransformComponent& transform, ECS& ecs) noexcept
		{
			if (transform.isPosDirty)
			{
				UpdateWorldPosition(transform, ecs);
			}

			return transform.worldPos;
		}

		static void SetPosDirty(TransformComponent& transform, ECS& ecs) noexcept
		{
			transform.isPosDirty = true;

			if (transform.relation.children != 0)
			{
				EntityId currChildId{ transform.relation.firstChild };

				for (uint32_t childIdx{ 0 }; childIdx < transform.relation.children; ++childIdx)
				{
					auto& currChildTransform{ ecs.GetComponent<TransformComponent>(currChildId) };

					SetPosDirty(currChildTransform, ecs);
					currChildId = currChildTransform.relation.nextSibling;
				}
			}
		}

		static void SetParent(const EntityId entityId, const EntityId parentId, ECS& ecs, bool keepWorldPos) noexcept
		{
			TransformComponent& transform{ ecs.GetComponent<TransformComponent>(entityId) };

			if ((IsTransformChildOfId(transform, ecs, parentId) ||
				parentId == entityId ||
				transform.relation.parent == parentId) && parentId != NULL_ENTITY_ID)
			{
				DEBUG_OUT("can not set entity " << parentId << " as parent for entity " << entityId);
				return;
			}

			if (parentId == NULL_ENTITY_ID)
			{
				if (keepWorldPos) //Keep worldPos upon deletion of a parent means we keep the children at their current world pos (and their relationships)
				{
					transform.localPos = GetWorldPosition(transform, ecs);
					SceneGraph::SetPosDirty(transform, ecs);

					if (transform.relation.children > 0)
					{
						auto currChildId = transform.relation.firstChild;

						for (size_t childIdx = 0; childIdx < transform.relation.children; ++childIdx)
						{
							auto& currChildTransform = ecs.GetComponent<TransformComponent>(currChildId);

							currChildTransform.localPos = transform.localPos + currChildTransform.localPos; //set all childrens positions to the correct world position

							currChildTransform.relation.parent = NULL_ENTITY_ID;
							currChildId = currChildTransform.relation.nextSibling;
						}
					}
				}
				else
				{
					DestroyChildren(transform, ecs);
				}
			}
			else
			{
				if (keepWorldPos)
				{
					auto& newParentTransform = ecs.GetComponent<TransformComponent>(parentId);
					transform.localPos = SceneGraph::GetWorldPosition(transform, ecs) - SceneGraph::GetWorldPosition(newParentTransform, ecs);
				}

				SetPosDirty(transform, ecs);
			}

			RemoveSelfAsChild(transform, entityId, ecs);
			transform.relation.parent = parentId;

			if (parentId != NULL_ENTITY_ID)
			{
				auto& newParentTransform = ecs.GetComponent<TransformComponent>(parentId);
				AddSelfAsChild(transform, entityId, newParentTransform, ecs);
			}
		}

		[[nodiscard]] static bool IsTransformChildOfId(const TransformComponent& transform, ECS& ecs, const EntityId entity) noexcept
		{
			if (transform.relation.children == 0)
			{
				return false;
			}

			EntityId currChildId{ transform.relation.firstChild };
			assert(currChildId != NULL_ENTITY_ID);

			for (size_t childIdx{ 0 }; childIdx < transform.relation.children; ++childIdx)
			{
				currChildId = ecs.GetComponent<TransformComponent>(currChildId).relation.nextSibling;

				if (currChildId == entity)
				{
					return true;
				}
			}

			return false;
		}

		static void DestroyChildren(TransformComponent& transform, ECS& ecs) noexcept
		{
			if (transform.relation.children != 0)
			{
				EntityId currChildId{ transform.relation.firstChild };

				for (uint32_t childIdx{ 0 }; childIdx < transform.relation.children; ++childIdx)
				{
					auto& childTransform{ ecs.GetComponent<TransformComponent>(currChildId) };
					const EntityId nextSiblingId{ childTransform.relation.nextSibling };

					DestroyChildren(childTransform, ecs);

					ecs.DestroyEntity(currChildId);
					currChildId = nextSiblingId;
				}
			}
		}



	private:
		static void UpdateWorldPosition(TransformComponent& transform, ECS& ecs) noexcept
		{
			if (transform.isPosDirty)
			{
				if (transform.relation.parent == NULL_ENTITY_ID)
				{
					transform.worldPos = transform.localPos;
				}
				else
				{
					transform.worldPos = GetWorldPosition(ecs.GetComponent<TransformComponent>(transform.relation.parent), ecs) + transform.localPos;
				}
			}

			transform.isPosDirty = false;
		}

		static void AddSelfAsChild(TransformComponent& self, const EntityId selfId, TransformComponent& parent, ECS& ecs) noexcept
		{
			if (self.relation.parent != NULL_ENTITY_ID)
			{
				++parent.relation.children;

				const EntityId firstChildId{ parent.relation.firstChild };

				parent.relation.firstChild = selfId;

				if (parent.relation.children != 1)
				{
					auto& firstChildTransfom{ ecs.GetComponent<TransformComponent>(firstChildId) };
					firstChildTransfom.relation.prevSibling = selfId;
				}

				self.relation.prevSibling = NULL_ENTITY_ID;
				self.relation.nextSibling = firstChildId;
			}
		}

		static void RemoveSelfAsChild(TransformComponent& self, const EntityId selfId, ECS& ecs) noexcept
		{
			if (self.relation.parent != NULL_ENTITY_ID)
			{
				auto& oldParentTransform{ ecs.GetComponent<TransformComponent>(self.relation.parent) };

				//Only child
				if (oldParentTransform.relation.children == 1)
				{
					--oldParentTransform.relation.children;

					oldParentTransform.relation.firstChild = NULL_ENTITY_ID;
				}

				//First child and there are other children
				else if (oldParentTransform.relation.firstChild == selfId)
				{
					--oldParentTransform.relation.children;

					auto prevSibId = self.relation.prevSibling;
					auto nexSibId = self.relation.nextSibling;

					auto& nextSibTransform{ ecs.GetComponent<TransformComponent>(nexSibId) };
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
						auto& nextSibTransform{ ecs.GetComponent<TransformComponent>(nexSibId) };
						nextSibTransform.relation.prevSibling = prevSibId;
					}
					if (prevSibId != NULL_ENTITY_ID)
					{
						auto& prevSibTransform{ ecs.GetComponent<TransformComponent>(prevSibId) };
						prevSibTransform.relation.nextSibling = nexSibId;
					}
				}
			}
		}

	};
}

#endif

