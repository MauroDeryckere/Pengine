#include "PengoIdleState.h"

#include "PengoWalkState.h"
#include "PengoBreakingBlockState.h"
#include "PengoInputCommands.h"
#include "SwitchAnimationEvent.h"
#include "EventManager.h"

#include "InputManager.h"

namespace Pengo
{
	PengoIdleState::PengoIdleState(const Pengin::UserIndex& userIdx, const glm::vec2& dir = {}) :
		Pengin::PlayerState{ userIdx }
	{
		using namespace Pengin;
		if (dir != glm::vec2{})
		{
			auto playerEntity{ Pengin::SceneManager::GetInstance().GetActiveScene()->GetPlayer(Pengin::PlayerState::GetUserIndex()) };

			if (dir.x > 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::IdleRight)));
			}
			else if (dir.x < 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::IdleLeft)));
			}
			else if (dir.y > 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::IdleDown)));
			}
			else if (dir.y < 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::IdleUp)));
			}
		}
	}

	std::unique_ptr<Pengin::PlayerState> Pengo::PengoIdleState::HandleInput(const Pengin::UserIndex& userIndex)
	{
		if (coold)
		{
			coold = false;
			return nullptr;
		}

		std::shared_ptr<Pengin::InputCommand> pCommand;
		if (Pengin::InputManager::GetInstance().IsActionExecuted(userIndex, "PengoMovement", &pCommand))
		{
			assert(pCommand);
			Pengo::Movement* pMovement{ static_cast<Pengo::Movement*>(pCommand.get()) };

			return std::move(std::make_unique<PengoWalkState>(userIndex, pMovement->GetDirection()));
		}
		if (Pengin::InputManager::GetInstance().IsActionExecuted(userIndex, "PengoBreakBlock"))
		{
			return std::move(std::make_unique<PengoBreakingBlockState>(userIndex));
		}

		return nullptr;
	}
}

