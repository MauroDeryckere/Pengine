#ifndef CHARACTERMOVEMENTCOMPONENT
#define CHARACTERMOVEMENTCOMPONENT

#include "ECS.h"

#include "glm/glm.hpp"

namespace Pengin
{
	class CharacterMovementComponent final
	{
	public:
		CharacterMovementComponent(float movementSpeed, EntityId id) :
			m_MovementSpeed{ movementSpeed },
			m_Id{ id }
		{}

		void Move(const glm::vec3& dir) const;

	private:
		const float m_MovementSpeed;
		const EntityId m_Id;
	};
}

#endif