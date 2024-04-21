#ifndef VELOCITYCOMPONENT
#define VELOCITYCOMPONENT

#include "glm/glm.hpp"

namespace Pengin
{
	struct VelocityComponent final
	{
		VelocityComponent() = default;

		VelocityComponent(const glm::vec3& vel) :
			velocity{ vel }
		{}

		~VelocityComponent() = default;

		glm::vec3 velocity{ 0, 0, 0 };
	};
}

#endif