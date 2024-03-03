#include "Transform.h"

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::Transform::Rotate(float angle, const glm::vec3& axis)
{
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

	m_position = glm::vec3(rotationMatrix * glm::vec4(m_position, 1.0f));
}
