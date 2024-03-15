#ifndef INPUTCOMMANDS
#define INPUTCOMMANDS

#include "InputCommand.h"
#include "CharacterMovementComponent.h"

#include "glm/vec3.hpp"

#include <iostream>

namespace Pengin
{
	class PrintTest final : public InputCommand
	{
	public:
		PrintTest() : InputCommand{} {}

		virtual void Execute() override { std::cout << "executed \n"; };
		virtual ~PrintTest() override = default;

		PrintTest(const PrintTest&) = delete;
		PrintTest& operator=(const PrintTest&) = delete;
		PrintTest(PrintTest&&) noexcept = delete;
		PrintTest& operator=(PrintTest&&) noexcept = delete;
	};

	class CharacterMovement final : public InputCommand
	{
	public:
		CharacterMovement(const glm::vec3& direction, float movementSpeed, dae::CharacterMovementComponent* characterMovement) : 
			InputCommand{},
			m_Direction{direction},
			m_MovementSpeed{movementSpeed},
			m_CharacterMovement{characterMovement}
		{
		}

		virtual void Execute() override 
		{ 
			m_CharacterMovement->TriggerMovement(m_Direction, m_MovementSpeed);
		}
		virtual ~CharacterMovement() override = default;

		CharacterMovement(const CharacterMovement&) = delete;
		CharacterMovement& operator=(const CharacterMovement&) = delete;
		CharacterMovement(CharacterMovement&&) noexcept = delete;
		CharacterMovement& operator=(CharacterMovement&&) noexcept = delete;


	private:
		float m_MovementSpeed;
		glm::vec3 m_Direction;

		dae::CharacterMovementComponent* m_CharacterMovement;
	};
}

#endif