#include "RotatorComponent.h"
#include <glm/glm.hpp>
#include "GameObject.h"
#include <cassert>

dae::RotatorComponent::RotatorComponent(GameObject* pGameObj, float rotationSpeed, bool rotateAroundParent, const glm::vec3& rotPoint):
	FunctionalComponent{ pGameObj }, m_ObjTransform{ GetGameObj()->GetTransform() }, 
	m_RotSpeed{ rotationSpeed }, m_RotateAroundParent{ rotateAroundParent }, m_RotPoint{rotPoint}
{

}

void dae::RotatorComponent::Update(float deltaT)
{
	assert(GetGameObj());

	m_ObjTransform.Rotate(m_RotSpeed * deltaT, glm::vec3(0.0f, 0.0f, 1.0f), m_RotateAroundParent, m_RotPoint);
}
