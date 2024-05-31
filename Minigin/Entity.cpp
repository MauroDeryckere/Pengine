#include "Entity.h"

#include "BodyComponent.h"

namespace Pengin
{
	void Entity::SetParent(Entity parentEntity, bool keepWorldPos)
	{
		assert(parentEntity.m_pScene == m_pScene && "entities must be in the same scene");

		SceneGraph::SetParent(m_EntityId, parentEntity.GetEntityId(), m_pScene->m_Ecs, keepWorldPos);
	}

	void Entity::SetLocalPosition(const glm::vec3& position)
	{
		auto& thisTransform = GetComponent<TransformComponent>();
		thisTransform.localPos = position;
		
		SceneGraph::SetPosDirty(thisTransform, m_pScene->m_Ecs);

		if (HasComponent<BodyComponent>())
		{
			GetComponent<BodyComponent>().currentPosition = position;
			GetComponent<BodyComponent>().lastPosition = position;
		}
	}
}


