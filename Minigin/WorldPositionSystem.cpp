#include "WorldPositionSystem.h"

#include "ECS.h"
#include "TransformComponent.h"
#include "EntityId.h"

#include <iostream>

namespace Pengin
{
	void WorldPositionSystem::Update() 
	{
		auto transforms{ m_ECS.GetComponents<TransformComponent>() };

		for (auto& entity : transforms)
		{
			if (entity.isPosDirty)
			{
				const auto& relation{ entity.relation };

				if (relation.parent == NULL_ENTITY_ID) 
				{
					entity.worldPos = entity.localPos;
					entity.isPosDirty = false; 
					continue;
				}

				auto& nextTransform{ m_ECS.GetComponent<TransformComponent>(relation.parent) };

				if (nextTransform.isPosDirty)
				{
					//Recursively update parent's position
					UpdateWorldPosition(nextTransform);
				}

				entity.worldPos = nextTransform.worldPos + entity.localPos;
				entity.isPosDirty = false;
			}
		}
	}

	void WorldPositionSystem::UpdateWorldPosition(TransformComponent& transform)
	{
		const auto& relation{ transform.relation };

		if (relation.parent == NULL_ENTITY_ID)
		{
			transform.worldPos = transform.localPos;
			transform.isPosDirty = false;
			return;
		}

		auto& nextTransform{ m_ECS.GetComponent<TransformComponent>(relation.parent) };

		if (nextTransform.isPosDirty)
		{
			UpdateWorldPosition(nextTransform);
		}

		transform.worldPos = nextTransform.worldPos + transform.localPos;
		transform.isPosDirty = false;
	}
}
