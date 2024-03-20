#include "CharacterMovementComponent.h"

#include "TransformComponent.h"
#include "Time.h"

namespace Pengin
{
	void CharacterMovementComponent::Move(const glm::vec3& dir) const
	{
		//In future: move ID out, create view for comp types
		auto& tranform{ Pengin::ECS::GetInstance().GetComponent<TransformComponent>(m_Id) };
		auto vel = dir * m_MovementSpeed;

		tranform.m_Position += (vel * Time::GetInstance().GetElapsedSec());
	}

}
