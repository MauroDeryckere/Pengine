#pragma once

#include "BaseSystem.h"
#include "EventManager.h"
#include "ECS.h"

#include "CollisionEvent.h"

namespace GS
{
	class FactorySystem final : public Pengin::BaseSystem
	{
	public:
		FactorySystem(Pengin::ECS& ecs) :
			m_ECS{ ecs },
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }
		{
			m_pObserver->RegisterForEvent(m_pObserver, Pengin::CollisionEvent::COLLISION_EVENT_NAME, [this](const Pengin::BaseEvent& ev) {OnCollision(ev); });
		}

		~FactorySystem() = default;

	private:
		Pengin::ECS& m_ECS;

		std::shared_ptr<Pengin::Observer> m_pObserver;

		void OnCollision(const Pengin::BaseEvent& event);
	};
}