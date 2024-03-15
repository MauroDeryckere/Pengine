#include "CharacterMovementComponent.h"
#include "GameObject.h"

dae::CharacterMovementComponent::CharacterMovementComponent(GameObject* pGameObj):
	FunctionalComponent{ pGameObj }
{
}

void dae::CharacterMovementComponent::TriggerMovement(const glm::vec3& dir, float movementSpeed)
{
	auto gameObj = GetGameObj();
	auto& transform = gameObj->GetTransform();
	auto& currPos = transform.GetPosition();

	auto movement = dir * movementSpeed + currPos;

	transform.SetPosition(movement.x, movement.y, movement.z);
}
