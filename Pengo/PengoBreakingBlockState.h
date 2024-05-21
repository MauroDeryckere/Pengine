#ifndef PENGOBREAKINGBLOCKSTATE
#define PENGOBREAKINGBLOCKSTATE

#include "PlayerState.h"
#include "DebugOutput.h"

#include "EventManager.h"
#include "SwitchAnimationEvent.h"
#include "Entity.h"

#include "SceneManager.h"

#include "PlayerSystem.h"

#include <string>

namespace Pengo
{
	class PengoBreakingBlockState final : public Pengin::PlayerState
	{
	public:
		PengoBreakingBlockState(const Pengin::UserIndex& userIdx) :
			Pengin::PlayerState{ userIdx }
		{ }

		void OnEnter()
		{
			auto playerEntity{ Pengin::SceneManager::GetInstance().GetActiveScene()->GetPlayer(Pengin::PlayerState::GetUserIndex())};

			if (playerEntity)
			{
				Pengin::EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<Pengin::SwitchAnimationEvent>(playerEntity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushLeft)));
			}

			DEBUG_OUT("Enter Breaking Block");
		}

		std::unique_ptr<Pengin::PlayerState> Update(const Pengin::UserIndex& userIndex);

		std::unique_ptr<Pengin::PlayerState> HandleInput(const Pengin::UserIndex&)
		{
			return nullptr;
		}

		void OnExit()
		{
			DEBUG_OUT("Exit Breaking Block");
		}

	private:

	};
}

#endif