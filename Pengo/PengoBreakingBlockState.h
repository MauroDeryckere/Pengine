#ifndef PENGOBREAKINGBLOCKSTATE
#define PENGOBREAKINGBLOCKSTATE

#include "PlayerState.h"
#include "DebugOutput.h"

#include "EventManager.h"
#include "SwitchAnimationEvent.h"
#include "PengoBlockBreakEvent.h"
#include "Entity.h"

#include "SceneManager.h"
#include "EventManager.h"

#include "PlayerSystem.h"

#include <string>
#include <glm/vec2.hpp>

namespace Pengo
{
	class PengoBreakingBlockState final : public Pengin::PlayerState
	{
	public:
		PengoBreakingBlockState(const Pengin::UserIndex& userIdx, const glm::vec2& dir) :
			Pengin::PlayerState{ userIdx },
			m_Direction{ dir }
		{ }

		void OnEnter();
		std::unique_ptr<Pengin::PlayerState> Update(const Pengin::UserIndex& userIndex);

	private:
		const glm::vec2 m_Direction{  };
	};
}

#endif