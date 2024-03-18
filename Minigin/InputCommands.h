#ifndef INPUTCOMMANDS
#define INPUTCOMMANDS

#include "InputCommand.h"
#include "CharacterMovementComponent.h"

#include "ECS.h"
#include "Components.h"

#include "glm/vec3.hpp"

#include <iostream>
#include <string>

namespace Pengin
{
	class ConsolePrint final : public InputCommand
	{
	public:
		ConsolePrint(const std::string& message) : InputCommand{}, m_Message{message} {}

		virtual void Execute() override { std::cout << m_Message << "\n"; }
		virtual ~ConsolePrint() override = default;

		ConsolePrint(const ConsolePrint&) = delete;
		ConsolePrint& operator=(const ConsolePrint&) = delete;
		ConsolePrint(ConsolePrint&&) noexcept = delete;
		ConsolePrint& operator=(ConsolePrint&&) noexcept = delete;

	private:
		const std::string m_Message;
	};

	class CharacterMovement final : public InputCommand
	{
	public:
		CharacterMovement(const glm::vec3& direction, EntityId id) :
			InputCommand{},
			m_Direction{direction},
			m_Id{ id }
		{
		}

		virtual void Execute() override 
		{ 
			auto& characterMovement{ ECS::GetInstance().GetComponent<CharacterMovementComponent>(m_Id) };
			characterMovement.Move(m_Direction);
		}

		virtual ~CharacterMovement() override = default;

		CharacterMovement(const CharacterMovement&) = delete;
		CharacterMovement& operator=(const CharacterMovement&) = delete;
		CharacterMovement(CharacterMovement&&) noexcept = delete;
		CharacterMovement& operator=(CharacterMovement&&) noexcept = delete;

	private:
		const glm::vec3 m_Direction;
		const EntityId m_Id;
	};
}

#endif