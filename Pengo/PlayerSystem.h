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
	using namespace Pengin;
	class PlayerSystem final : public BaseSystem
	{
	public:
		PlayerSystem(ECS& ecs) :
			m_ECS{ ecs },
			m_pObserver{ EventManager::GetInstance().CreateObserver() }

		{
			m_pObserver->RegisterForEvent(m_pObserver, PengoBlockBreakEvent::PENGO_BLOCKBR_EVENT_NAME, [this](const BaseEvent& event) { OnBlockBreakEvent(event); });
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
			PushRight = 8

			//Die
		};

	private:
		ECS& m_ECS;

		void OnBlockBreakEvent(const BaseEvent& event);

		std::shared_ptr<Observer> m_pObserver;
	};
}

#endif
