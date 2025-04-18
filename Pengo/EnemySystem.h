#ifndef PENGO_ENEMYSYSTEM
#define PENGO_ENEMYSYSTEM

#include "BaseSystem.h"
#include "EventManager.h"
#include "CollisionEvent.h"

namespace Pengin
{
	class ECS;
}
namespace Pengo
{
	class EnemySystem final : public Pengin::BaseSystem
	{
	public:
		EnemySystem(Pengin::ECS& ecs) :
			Pengin::BaseSystem{ },
			m_ECS{ ecs },
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }

		{ 
			m_pObserver->RegisterForEvent(m_pObserver, Pengin::CollisionEvent::COLLISION_EVENT_NAME, [this](const Pengin::BaseEvent& ev) { OnCollision(ev); });
		}

		~EnemySystem() = default;

		void Update();

		EnemySystem(const EnemySystem&) = delete;
		EnemySystem(EnemySystem&&) = delete;
		EnemySystem& operator=(const EnemySystem&) = delete;
		EnemySystem& operator=(const EnemySystem&&) = delete;

		enum class EnemyAnimations : uint8_t
		{
			Spawning = 0,

			WalkDown = 1, 
			WalkLeft = 2,
			WalkUp = 3,
			WalkRight = 4,
		};

	private:
		Pengin::ECS& m_ECS;

		std::shared_ptr<Pengin::Observer> m_pObserver;

		void OnCollision(const Pengin::BaseEvent& event);
	};
}

#endif
