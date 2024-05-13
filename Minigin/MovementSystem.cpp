#include "MovementSystem.h"

#include "ECS.h"
#include "BodyComponent.h"

namespace Pengin
{
	void MovementSystem::FirstStep(const float stepTime)
	{
		auto bodyComps{ m_ECS.GetComponents<BodyComponent>() };

		for (auto it{ bodyComps.begin() }; auto & body : bodyComps)
		{
			body.velocity -= body.lastFrInputVelocity;
			body.velocity += body.inputVelocity;

			body.currentPosition += body.velocity * stepTime;


			++it;
		}
	}

	void MovementSystem::Step(const float stepTime)
	{
		auto bodyComps{ m_ECS.GetComponents<BodyComponent>() };

		for (auto it{ bodyComps.begin() }; auto& body : bodyComps)
		{
			body.currentPosition += body.velocity * stepTime;

			++it;
		}
	}
}


