#ifndef INPUTCOMMANDS
#define INPUTCOMMANDS

#include "SceneManager.h"

#include "InputCommand.h"
#include "EventManager.h"

#include "SoundData.h"
#include "ServiceLocator.h"

#include "HealthChangeEvent.h"
#include "ScoreChangeEvent.h"

#include "ECS.h"
#include "Entity.h"
#include "Components.h"

#include "glm/vec3.hpp"

#include <memory>
#include <iostream>
#include <string>

namespace Pengin
{
	class InpDebugCommand final : public InputCommand
	{
	public:

		InpDebugCommand(const UserIndex& user, const std::string& msg) :
			InputCommand{ user, "InpDebug"},
			m_Message{ msg }
		{ }

		virtual void Execute() override
		{
			DEBUG_OUT(m_Message);
			DEBUG_OUT("on user: " << GetUserIdx().GetUUID_PrettyStr());
		}

		virtual ~InpDebugCommand() override = default;

		InpDebugCommand(const InpDebugCommand&) = delete;
		InpDebugCommand& operator=(const InpDebugCommand&) = delete;
		InpDebugCommand(InpDebugCommand&&) noexcept = delete;
		InpDebugCommand& operator=(InpDebugCommand&&) noexcept = delete;

	private:
		const std::string m_Message;
	};

	class AttackPlayer final : public InputCommand //bound to input for now
	{
	public:
		AttackPlayer(const UserIndex& user) :
			InputCommand{ user, "Attack"}
		{ }

		virtual void Execute() override
		{
			auto pActiveScene = SceneManager::GetInstance().GetActiveScene();
			Entity playerEntity{ pActiveScene->GetPlayer(InputCommand::GetUserIdx()) };

			if (!playerEntity)
			{
				DEBUG_OUT("attack for invalid player");
				return;
			}

			playerEntity.GetComponent<HealthComponent>().health--;

			EventManager::GetInstance().BroadcoastEvent(std::make_unique<HealthChangeEvent>("OnHealthChangeEvent", playerEntity.GetEntityId()));
		}

		virtual ~AttackPlayer() override = default;

		AttackPlayer(const AttackPlayer&) = delete;
		AttackPlayer& operator=(const AttackPlayer&) = delete;
		AttackPlayer(AttackPlayer&&) noexcept = delete;
		AttackPlayer& operator=(AttackPlayer&&) noexcept = delete;

	private:
	};

	class CollectScore final : public InputCommand //bound to input for now
	{
	public:
		CollectScore(const UserIndex& user, unsigned score = 10) :
			InputCommand{ user, "ScoreCollect"},
			m_ScoreVal{ score }
		{ }

		virtual void Execute() override
		{
			auto pActiveScene = SceneManager::GetInstance().GetActiveScene();

			Entity playerEntity{ pActiveScene->GetPlayer(InputCommand::GetUserIdx()) };

			if (!playerEntity)
			{
				DEBUG_OUT("score for invalid player");
				return;
			}

			playerEntity.GetComponent<ScoreComponent>().score += m_ScoreVal;

			EventManager::GetInstance().BroadcoastEvent(std::make_unique<ScoreChangeEvent>("OnScoreCollectEvent", playerEntity.GetEntityId()));
		}

		virtual ~CollectScore() override = default;

		CollectScore(const CollectScore&) = delete;
		CollectScore& operator=(const CollectScore&) = delete;
		CollectScore(CollectScore&&) noexcept = delete;
		CollectScore& operator=(CollectScore&&) noexcept = delete;

	private:
		const unsigned m_ScoreVal;
	};

	class MakeSound final : public InputCommand //bound to input for now
	{
	public:
		MakeSound(const UserIndex& user, const SoundData& soundData) :
			InputCommand{ user, "MakeSound"},
			m_SoundData{ soundData }
		{ 
			assert(!soundData.soundPath.empty());
		}

		virtual void Execute() override
		{
			ServiceLocator::GetSoundSystem().PlaySound(m_SoundData);
		}

		virtual ~MakeSound() override = default;

		MakeSound(const MakeSound&) = delete;
		MakeSound& operator=(const MakeSound&) = delete;
		MakeSound(MakeSound&&) noexcept = delete;
		MakeSound& operator=(MakeSound&&) noexcept = delete;

	private:
		SoundData m_SoundData;
	};
}

#endif