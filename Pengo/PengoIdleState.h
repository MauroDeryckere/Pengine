#ifndef PENGOIDLESTATE
#define PENGOIDLESTATE

#include "PlayerState.h"

#include "EventManager.h"
#include "SwitchAnimationEvent.h"

#include "DebugOutput.h"
#include "EntityId.h"
#include "SceneManager.h"
#include "PlayerSystem.h"
#include "Entity.h"

#include <string>
#include <memory>

namespace Pengo
{
	class PengoIdleState final : public Pengin::PlayerState
	{
	public:
		PengoIdleState(const Pengin::UserIndex& userIdx) :
			Pengin::PlayerState{ userIdx }
		{}

		void OnEnter()
		{
			auto playerEntity{ Pengin::SceneManager::GetInstance().GetActiveScene()->GetPlayer(Pengin::PlayerState::GetUserIndex()) };

			if (playerEntity)
			{
				Pengin::EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<Pengin::SwitchAnimationEvent>(playerEntity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::Idle)));
			}

			DEBUG_OUT("Enter Idle");
		}

		std::unique_ptr<Pengin::PlayerState> HandleInput(const Pengin::UserIndex& userIndex);

		void OnExit()
		{
			DEBUG_OUT("Exit Idle");
		}

	};
}

#endif