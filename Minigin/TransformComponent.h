#ifndef TRANSFORMCOMPONENT
#define TRANSFORMCOMPONENT

#include "glm/glm.hpp"

namespace Pengin
{
	struct TransformComponent final
	{
		TransformComponent() = default;
		TransformComponent(const glm::vec3& pos, const glm::vec3& scale = {1, 1, 1} ) : 
			m_Position{ pos },
			m_Scale{ scale }
		{}

		~TransformComponent() = default;

		glm::vec3 m_Position{ 0, 0, 0 };
		glm::vec3 m_Scale{ 1, 1, 1 };
	};
}

#endif

