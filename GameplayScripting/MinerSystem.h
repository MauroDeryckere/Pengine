#pragma once

#include "BaseSystem.h"

#include "EventManager.h"
#include "ECS.h"

namespace GS
{
	class MinerSystem final : public Pengin::BaseSystem
	{
	public:
		MinerSystem(Pengin::ECS& ecs) :
			m_ECS{ ecs },
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }
		{
			m_pObserver->RegisterForEvent(m_pObserver, "PlayerDeath", [this](const Pengin::BaseEvent&) { OnPlayerDeath(); });
		}

		~MinerSystem() = default;

		void Update();

	private:
		Pengin::ECS& m_ECS;

		std::shared_ptr<Pengin::Observer> m_pObserver;

		void OnPlayerDeath();
	};
}