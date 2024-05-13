#include "PengoIdleState.h"

#include "PengoWalkState.h"
#include "PengoBreakingBlockState.h"

namespace Pengo
{
	std::unique_ptr<Pengin::PlayerState> Pengo::PengoIdleState::HandleInput(const Pengin::UserIndex& userIndex, const std::string& actionName)
	{
		if (actionName == "PengoMovement")
		{
			return std::move(std::make_unique<PengoWalkState>(userIndex));
		}
		else if (actionName == "PengoBreakBlock")
		{
			return std::move(std::make_unique<PengoBreakingBlockState>(userIndex));
		}

		return nullptr;
	}
}

