#ifndef TRANSFORMCOMPONENT
#define TRANSFORMCOMPONENT

#include "glm/glm.hpp"

namespace Pengin
{
	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const glm::vec3& pos) : m_Position{ pos }
		{}

		~TransformComponent() = default;

		glm::vec3 m_Position{ 0, 0, 0 };
	};
}

#endif

