#ifndef PENGO_WALLSYSTEM
#define PENGO_WALLSYSTEM

#include "BaseSystem.h"

#include "EventManager.h"
#include "CollisionEvent.h"


namespace Pengin
{
	class ECS;
}
namespace Pengo
{
	class WallSystem final : public Pengin::BaseSystem
	{
	public:
		WallSystem(Pengin::ECS& ecs) :
			Pengin::BaseSystem{ },
			m_ECS{ ecs },
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }

		{
			m_pObserver->RegisterForEvent(m_pObserver, Pengin::CollisionEvent::COLLISION_EVENT_NAME, [this](const Pengin::BaseEvent& ev) { OnCollision(ev); });
		}

		~WallSystem() = default;

		void Update();

		WallSystem(const WallSystem&) = delete;
		WallSystem(WallSystem&&) = delete;
		WallSystem& operator=(const WallSystem&) = delete;
		WallSystem& operator=(const WallSystem&&) = delete;

	private:
		Pengin::ECS& m_ECS;

		std::shared_ptr<Pengin::Observer> m_pObserver;

		void OnCollision(const Pengin::BaseEvent& event);
	};
}

#endif
