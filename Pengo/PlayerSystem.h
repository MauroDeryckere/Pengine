#ifndef PENGO_PLAYERSYSTEM
#define PENGO_PLAYERSYSTEM

#include "BaseSystem.h"
#include "EventManager.h"
#include "PengoBlockBreakEvent.h"

namespace Pengin
{
	class ECS;
}
namespace Pengo
{
	class PlayerSystem final : public Pengin::BaseSystem
	{
	public:
		PlayerSystem(Pengin::ECS& ecs) :
			Pengin::BaseSystem{ },
			m_ECS{ ecs },
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }

		{
			m_pObserver->RegisterForEvent(m_pObserver, PengoBlockBreakEvent::PENGO_BLOCKBR_EVENT_NAME, [this](const Pengin::BaseEvent& event) { OnBlockBreakEvent(event); });
			m_pObserver->RegisterForEvent(m_pObserver, "OnHealthChangeEvent", [this](const Pengin::BaseEvent& event) { OnDeathEvent(event); });
			m_pObserver->RegisterForEvent(m_pObserver, "OnPengoRespawn", [this](const Pengin::BaseEvent& event) { OnRespawnEvent(event); });
		}

		~PlayerSystem() = default;

		void Update();

		PlayerSystem(const PlayerSystem&) = delete;
		PlayerSystem(PlayerSystem&&) = delete;
		PlayerSystem& operator=(const PlayerSystem&) = delete;
		PlayerSystem& operator=(const PlayerSystem&&) = delete;

		enum class PengoAnimations
		{
			Idle = 0,

			MoveDown = 1,
			MoveLeft = 2,
			MoveUp = 3,
			MoveRight = 4,


			PushDown = 5,
			PushLeft = 6,
			PushUp = 7,
			PushRight = 8,

			Dying = 9
		};

	private:
		Pengin::ECS& m_ECS;

		void OnBlockBreakEvent(const Pengin::BaseEvent& event);
		void OnDeathEvent(const Pengin::BaseEvent& event);
		void OnRespawnEvent(const Pengin::BaseEvent& event);

		std::shared_ptr<Pengin::Observer> m_pObserver;
	};
}

#endif
