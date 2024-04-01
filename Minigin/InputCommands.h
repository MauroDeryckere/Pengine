#ifndef INPUTCOMMANDS
#define INPUTCOMMANDS

#include "SceneManager.h"

#include "InputCommand.h"
#include "EventManager.h"

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
		CharacterMovement(const glm::vec3& direction, EntityId id, float movementSpeed = 100.f) :
			InputCommand{},
			m_Direction{direction},
			m_Id{ id },
			m_MovementSpeed{ movementSpeed }
		{
		}

		virtual void Execute() override 
		{ 
			auto& ecs = SceneManager::GetInstance().GetActiveScene()->GetECS();
			ecs.GetComponent<VelocityComponent>(m_Id).m_Velocity += m_Direction * m_MovementSpeed;
		}

		virtual ~CharacterMovement() override = default;

		CharacterMovement(const CharacterMovement&) = delete;
		CharacterMovement& operator=(const CharacterMovement&) = delete;
		CharacterMovement(CharacterMovement&&) noexcept = delete;
		CharacterMovement& operator=(CharacterMovement&&) noexcept = delete;

	private:
		const glm::vec3 m_Direction;
		const EntityId m_Id;
		const float m_MovementSpeed;
	};

	class AttackPlayer final : public InputCommand //bound to input for now
	{
	public:
		AttackPlayer(const EntityId id) :
			m_Id{ id }
		{
			assert(SceneManager::GetInstance().GetActiveScene()->GetECS().HasComponent<HealthComponent>(m_Id));
		}

		virtual void Execute() override
		{
			auto& ecs = SceneManager::GetInstance().GetActiveScene()->GetECS();
			ecs.GetComponent<HealthComponent>(m_Id).m_Health--;

			Event healthchangeEvent{"OnHealthChangeEvent", &m_Id};
			EventManager::GetInstance().BroadcoastEvent(healthchangeEvent);
		}

		virtual ~AttackPlayer() override = default;

		AttackPlayer(const AttackPlayer&) = delete;
		AttackPlayer& operator=(const AttackPlayer&) = delete;
		AttackPlayer(AttackPlayer&&) noexcept = delete;
		AttackPlayer& operator=(AttackPlayer&&) noexcept = delete;

	private:
		const EntityId m_Id;
	};

	class CollectScore final : public InputCommand //bound to input for now
	{
	public:
		CollectScore(const EntityId id, unsigned score = 10) :
			m_EntityId { id },
			m_ScoreVal{ score }
		{ }

		virtual void Execute() override
		{
			auto& ecs = SceneManager::GetInstance().GetActiveScene()->GetECS();
			ecs.GetComponent<ScoreComponent>(m_EntityId).m_Score += m_ScoreVal;

			Event scoreEvent{ "OnScoreCollectEvent" , &m_EntityId };
			EventManager::GetInstance().BroadcoastEvent(scoreEvent);
		}

		virtual ~CollectScore() override = default;

		CollectScore(const CollectScore&) = delete;
		CollectScore& operator=(const CollectScore&) = delete;
		CollectScore(CollectScore&&) noexcept = delete;
		CollectScore& operator=(CollectScore&&) noexcept = delete;

	private:
		const EntityId m_EntityId;

		const unsigned m_ScoreVal;
	};

}

#endif