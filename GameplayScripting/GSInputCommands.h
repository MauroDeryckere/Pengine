#pragma once

#include "InputCommand.h"
#include "SceneManager.h"
#include "Entity.h"
#include "BodyComponent.h"

#include "MinerComponent.h"

#include "EventManager.h"
#include "LevelManager.h"
#include "SceneManager.h"

#include <algorithm>

namespace GS
{
	class Movement final : public Pengin::InputCommand
	{
	public:
		Movement(const Pengin::UserIndex& user, const glm::vec3& direction):
			Pengin::InputCommand{ user, "Movement" },
			m_Direction{ direction }
		{ }

		~Movement() = default;

		void Execute() override
		{
			auto pActiveScene = Pengin::SceneManager::GetInstance().GetActiveScene();
			auto player = pActiveScene->GetPlayer(GetUserIdx());

			const auto& minerComp = player.GetComponent<MinerComponent>();

			float movementSpeed = 100.f;

			if (minerComp.totOreWeight > 0)
			{
				movementSpeed *= std::min(1.0f, std::abs(static_cast<float>(minerComp.baseWeight) / static_cast<float>(minerComp.totOreWeight)));
			}

			player.GetComponent<Pengin::BodyComponent>().inputVelocity += (m_Direction * movementSpeed);
		}

	private:
		const glm::vec3 m_Direction{ };
	};

	class Mine final : public Pengin::InputCommand
	{
	public:
		Mine(const Pengin::UserIndex& user) :
			Pengin::InputCommand{ user, "Mine" }
		{ }

		~Mine() = default;

		void Execute() {}
	};

	class DropOre final : public Pengin::InputCommand
	{
	public:
		DropOre(const Pengin::UserIndex& user) :
			Pengin::InputCommand{ user, "DropOre" }
		{ }

		~DropOre() = default;

		void Execute() 
		{			
			auto pActiveScene = Pengin::SceneManager::GetInstance().GetActiveScene();
			auto player = pActiveScene->GetPlayer(GetUserIdx());

			player.GetComponent<MinerComponent>().DropOre();

			Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("MinerOreChange"));
		}
	};

	class Restart final : public Pengin::InputCommand
	{
	public:
		Restart(const Pengin::UserIndex& user):
			Pengin::InputCommand{ user, "Restart" }
		{}
		~Restart() = default;

		void Execute() 
		{
			Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("LoadLevel"));
		}
	};

}