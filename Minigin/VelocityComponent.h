#ifndef VELOCITYCOMPONENT
#define VELOCITYCOMPONENT

#include "glm/glm.hpp"

namespace Pengin
{
	struct VelocityComponent final
	{
		VelocityComponent() = default;

		VelocityComponent(const glm::vec3& vel) :
			m_Velocity{ vel }
		{}

		~VelocityComponent() = default;

		glm::vec3 m_Velocity{ 0, 0, 0 };
	};
}

#endif