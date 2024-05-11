#include "PhysicsSystem.h"

#include "GameTime.h"

#include "MovementSystem.h"
#include "CollisionSystem.h"

#include "TransformComponent.h"
#include "BodyComponent.h"

#include <cassert>

namespace Pengin
{
	PhysicsSystem::PhysicsSystem(ECS& ecs, uint32_t numIterations):
		BaseSystem{ },
		m_ECS{ ecs },

		m_NumIterations{ numIterations },

		m_pMovementSystem{ std::make_unique<MovementSystem>(ecs) },
		m_pCollisionSystem{ std::make_unique<CollisionSystem>(ecs) },
		m_pWorldposSystem{ std::make_unique<WorldPositionSystem>(ecs) }
	{
		assert(numIterations != 0 && "can not have 0 physics iterations");
	}

	void PhysicsSystem::FixedUpdate()
	{
		const auto stepTime = GameTime::GetInstance().GetFixedTimeStep() / m_NumIterations;

		auto bodyComps{ m_ECS.GetComponents<BodyComponent>() };

		for (uint32_t iteration { 0 }; iteration < m_NumIterations; ++iteration)
		{
			(iteration == 0 ? 
				m_pMovementSystem->FirstStep(stepTime) : 
				m_pMovementSystem->Step(stepTime));

			m_pCollisionSystem->Step();

			if (iteration == m_NumIterations - 1)
			{
				for (auto it{ bodyComps.begin() }; auto & body : bodyComps)
				{
					auto id = bodyComps.GetIdFromIterator(it);
					auto& transform = m_ECS.GetComponent<TransformComponent>(id);

					transform.localPos += body.currentPosition - body.lastPosition;

					SceneGraph::SetPosDirty(transform, m_ECS);

					body.velocity -= body.inputVelocity;
					body.inputVelocity = {};

					body.lastPosition = body.currentPosition;

					++it;
				}

				m_pCollisionSystem->BroadCastCollisionsEvents();
				m_pWorldposSystem->FixedUpdate(); //technically this could also be done with e GetPosition but then the worldpos would remain invalid until Get is called.
			}
		}
	}
}
