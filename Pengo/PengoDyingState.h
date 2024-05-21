#ifndef PENGODYINGSTATE
#define PENGODYINGSTATE

#include "PlayerState.h"

#include "DebugOutput.h"

#include "PlayerSystem.h"
#include "EventManager.h"
#include "SwitchAnimationEvent.h"
#include "SceneManager.h"
#include "Entity.h"

#include <string>

namespace Pengo
{
	class PengoDyingState final : public Pengin::PlayerState
	{
	public:
		PengoDyingState(const Pengin::UserIndex& userIdx) :
			Pengin::PlayerState{ userIdx }
		{}

		void OnEnter();

		std::unique_ptr<Pengin::PlayerState> Update(const Pengin::UserIndex& userIndex);

		std::unique_ptr<Pengin::PlayerState> HandleInput(const Pengin::UserIndex&)
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