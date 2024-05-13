#ifndef PENGOWALKSTATE
#define PENGOWALKSTATE

#include "PlayerState.h"
#include "DebugOutput.h"

#include <string>

namespace Pengo
{
	using namespace Pengin;

	class PengoWalkState final : public PlayerState
	{
	public:
		PengoWalkState(const UserIndex& userIdx) :
			PlayerState{ userIdx }
		{}

		void OnEnter()
		{
			DEBUG_OUT("Enter walk");
		}

		std::unique_ptr<PlayerState> Update(const UserIndex& userIndex);

		std::unique_ptr<PlayerState> HandleInput(const UserIndex& userIndex, const std::string& actionName);

		void OnExit()
		{
			DEBUG_OUT("Exit walk");
		}

	};
}

#endif