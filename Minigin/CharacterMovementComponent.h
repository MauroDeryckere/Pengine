#ifndef CHARACTERMOVEMENTCOMPONENT
#define CHARACTERMOVEMENTCOMPONENT

#include "ECS.h"
#include "TransformComponent.h"
#include "Time.h"

namespace Pengin
{
	class CharacterMovementComponent final //Separate class because we can handle directions this way (mutliple binds, diagonal 'boosting')
	{
	public:
		CharacterMovementComponent(float movementSpeed, EntityId id) :
			m_MovementSpeed{ movementSpeed },
			m_Id{ id }
		{}

		void Move(const glm::vec3& dir) const
		{
			//In future: move ID out, create view for comp types
			auto& tranform{ Pengin::ECS::GetInstance().GetComponent<TransformComponent>(m_Id) };
			auto vel = dir * m_MovementSpeed;
			tranform.m_Position += (vel * Time::GetInstance().GetElapsedSec());
		}

	private:
		const float m_MovementSpeed;
		const EntityId m_Id;
	};
}

#endif