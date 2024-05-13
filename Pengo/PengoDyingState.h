#ifndef PENGODYINGSTATE
#define PENGODYINGSTATE

#include "PlayerState.h"
#include "DebugOutput.h"

#include <string>

namespace Pengo
{
	using namespace Pengin;

	class PengoDyingState final : public PlayerState
	{
	public:
		PengoDyingState(const UserIndex& userIdx) :
			PlayerState{ userIdx }
		{}

		void OnEnter()
		{
			//TOOD switch animation

			DEBUG_OUT("Enter Dying");
		}

		std::unique_ptr<PlayerState> HandleInput(const UserIndex& userIndex, const std::string&)
		{
			return nullptr;
		}

		void OnExit()
		{
			DEBUG_OUT("Exit Dying");
		}

	};
}

#endif