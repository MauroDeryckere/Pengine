#include "CharacterMovementComponent.h"
#include "GameObject.h"
#include "Time.h"

dae::CharacterMovementComponent::CharacterMovementComponent(GameObject* pGameObj, float movementSpeed):
	FunctionalComponent{ pGameObj },
	m_MovementSpeed{movementSpeed}
{
}

void dae::CharacterMovementComponent::Move(const glm::vec3& dir)
{
	auto gameObj = GetGameObj();
	auto& transform = gameObj->GetTransform();
	auto& currPos = transform.GetPosition();

	auto movement = dir * m_MovementSpeed * Pengin::Time::GetInstance().GetElapsedSec() + currPos;

	transform.SetPosition(movement.x, movement.y, movement.z);
}
