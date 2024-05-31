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
			m_pObserver->RegisterForEvent(m_pObserver, PengoBlockBreakEvent::PENGO_BLOCKBR_EVENT_NAME, [this](const Pengin::BaseEvent& ev) {OnBlockBreakEvent(ev); });
			m_pObserver->RegisterForEvent(m_pObserver, "PengoDeath", [this](const Pengin::BaseEvent& event) { OnDeathEvent(event); });
			m_pObserver->RegisterForEvent(m_pObserver, "OnPengoRespawn", [this](const Pengin::BaseEvent& event) { OnRespawnEvent(event); });
		}

		~PlayerSystem() = default;

		void Update();

		PlayerSystem(const PlayerSystem&) = delete;
		PlayerSystem(PlayerSystem&&) = delete;
		PlayerSystem& operator=(const PlayerSystem&) = delete;
		PlayerSystem& operator=(const PlayerSystem&&) = delete;

		enum class PengoAnimations : uint8_t
		{
			IdleDown = 0,
			IdleLeft = 1,
			IdleUp = 2,
			IdleRight = 3,

			MoveDown = 4,
			MoveLeft = 5,
			MoveUp = 6,
			MoveRight = 7,

			PushDown = 8,
			PushLeft = 9,
			PushUp = 10,
			PushRight = 11,

			Dying = 12
		};

	private:
		Pengin::ECS& m_ECS;

		void OnDeathEvent(const Pengin::BaseEvent& event);
		void OnRespawnEvent(const Pengin::BaseEvent& event);
		void OnBlockBreakEvent(const Pengin::BaseEvent& event);

		std::shared_ptr<Pengin::Observer> m_pObserver;
	};
}

#endif
