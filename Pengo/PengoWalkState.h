#ifndef PENGOWALKSTATE
#define PENGOWALKSTATE

#include "PlayerState.h"
#include "DebugOutput.h"

#include <string>

namespace Pengo
{
	class PengoWalkState final : public Pengin::PlayerState
	{
	public:
		PengoWalkState(const Pengin::UserIndex& userIdx) :
			Pengin::PlayerState{ userIdx }
		{}

		void OnEnter()
		{
			DEBUG_OUT("Enter walk");
		}

		std::unique_ptr<Pengin::PlayerState> Update(const Pengin::UserIndex& userIndex);

		std::unique_ptr<Pengin::PlayerState> HandleInput(const Pengin::UserIndex& userIndex, const std::string& actionName);

		void OnExit()
		{
			DEBUG_OUT("Exit walk");
		}

	};
}

#endif