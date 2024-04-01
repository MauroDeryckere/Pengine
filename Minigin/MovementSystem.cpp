#include "MovementSystem.h"

#include "ECS.h"
#include "Entity.h"

#include "Time.h"

#include "TransformComponent.h"
#include "VelocityComponent.h"

namespace Pengin
{
	void MovementSystem::Update()
	{
		auto velocityComps{ m_ECS.GetComponents<VelocityComponent>() };

		for (auto it{ velocityComps.begin() }; auto& entity : velocityComps)
		{
			auto id = velocityComps.GetIdFromIterator(it);
			auto& transform = m_ECS.GetComponent<TransformComponent>(id);
			
			transform.localPos += entity.m_Velocity * Time::GetInstance().GetElapsedSec();

			SetPosDirty(transform);

			entity.m_Velocity = {};

			++it;
		}
	}
	void MovementSystem::SetPosDirty(TransformComponent& transform)
	{
		transform.isPosDirty = true;

		if (transform.relation.children != 0)
		{
			auto currChildId = transform.relation.firstChild;

			for (size_t childIdx{ 0 }; childIdx < transform.relation.children; ++childIdx)
			{
				auto& currChildTransform = m_ECS.GetComponent<TransformComponent>(currChildId);

				SetPosDirty(currChildTransform);
				currChildId = currChildTransform.relation.nextSibling;
			}
		}
	}
}


