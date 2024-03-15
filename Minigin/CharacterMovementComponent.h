#ifndef CHARACTERMOVEMENTCOMPONENT
#define CHARACTERMOVEMENTCOMPONENT

#include "FunctionalComponent.h"
#include "glm/vec3.hpp"
#include "glm/common.hpp"

namespace dae
{
	class CharacterMovementComponent final :
		public FunctionalComponent
	{
	public:
		CharacterMovementComponent(GameObject* pGameObj);
		~CharacterMovementComponent() override = default;

		virtual void Update() override { };

		void TriggerMovement(const glm::vec3& dir, float movementSpeed);

		CharacterMovementComponent(const CharacterMovementComponent& other) = delete;
		CharacterMovementComponent(CharacterMovementComponent&& other) = delete;
		CharacterMovementComponent& operator=(const CharacterMovementComponent& other) = delete;
		CharacterMovementComponent& operator=(CharacterMovementComponent&& other) = delete;
	};
}

#endif

