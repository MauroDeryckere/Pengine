#include "PengoIdleState.h"

#include "PengoWalkState.h"
#include "PengoBreakingBlockState.h"

#include "InputManager.h"

namespace Pengo
{
	std::unique_ptr<Pengin::PlayerState> Pengo::PengoIdleState::HandleInput(const Pengin::UserIndex& userIndex)
	{
		if (Pengin::InputManager::GetInstance().IsActionExecuted(userIndex, "PengoMovement"))
		{
			return std::move(std::make_unique<PengoWalkState>(userIndex));
		}
		if (Pengin::InputManager::GetInstance().IsActionExecuted(userIndex, "PengoBreakBlock"))
		{
			return std::move(std::make_unique<PengoBreakingBlockState>(userIndex));
		}

		return nullptr;
	}
}

