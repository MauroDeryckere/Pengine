#ifndef INPUTCOMMANDS
#define INPUTCOMMANDS

#include "InputCommand.h"
#include "EventManager.h"

#include "ECS.h"

#include "Components.h"
#include "HealthComponent.h"

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

	class AttackPlayer final : public InputCommand //bound to input for now
	{
	public:
		AttackPlayer(EntityId id, const std::string& event, unsigned attackDamage = 1) :
			m_Id{ id },
			m_AttackDamage{ attackDamage },
			m_Event{event}
		{
			assert(ECS::GetInstance().HasComponent<HealthComponent>(m_Id));
		}

		virtual void Execute() override
		{
			auto& playerHealth{ ECS::GetInstance().GetComponent<HealthComponent>(m_Id) };
			playerHealth.TakeDamage(m_AttackDamage, m_Event);
		}

		virtual ~AttackPlayer() override = default;

		AttackPlayer(const AttackPlayer&) = delete;
		AttackPlayer& operator=(const AttackPlayer&) = delete;
		AttackPlayer(AttackPlayer&&) noexcept = delete;
		AttackPlayer& operator=(AttackPlayer&&) noexcept = delete;

	private:
		const EntityId m_Id;
		const unsigned m_AttackDamage;
		const std::string m_Event;
	};

	class CollectScore final : public InputCommand //bound to input for now
	{
	public:
		CollectScore(EntityId id, unsigned score = 10) :
			m_Id{ id },
			m_Score{ score }
		{ }

		virtual void Execute() override
		{
			auto scoreEvent{"OnScoreCollect"};
			EventManager::GetInstance().BroadcoastEvent(scoreEvent);
		}

		virtual ~CollectScore() override = default;

		CollectScore(const CollectScore&) = delete;
		CollectScore& operator=(const CollectScore&) = delete;
		CollectScore(CollectScore&&) noexcept = delete;
		CollectScore& operator=(CollectScore&&) noexcept = delete;

	private:
		const EntityId m_Id;
		const unsigned m_Score;
	};

}

#endif