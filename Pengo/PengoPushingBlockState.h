#ifndef PENGOPUSHINGBLOCKSTATE
#define PENGOPUSHINGBLOCKSTATE

#include "PlayerState.h"
#include "DebugOutput.h"

#include "EventManager.h"
#include "SwitchAnimationEvent.h"
#include "PengoBlockBreakEvent.h"
#include "Entity.h"

#include "SceneManager.h"
#include "EventManager.h"

#include "PlayerSystem.h"
#include "CollisionEvent.h"

#include <string>
#include <glm/vec2.hpp>

namespace Pengo
{
	class PengoPushingBlockState final : public Pengin::PlayerState
	{
	public:
		PengoPushingBlockState(const Pengin::UserIndex& userIdx, const glm::vec2& dir) :
			Pengin::PlayerState{ userIdx },
			m_Direction{ dir }		
		{ }

		void OnEnter();
		std::unique_ptr<Pengin::PlayerState> Update(const Pengin::UserIndex& userIndex);

	private:
		const glm::vec2 m_Direction{ };

		float m_Timer{ 0.f };
	};
}

#endif