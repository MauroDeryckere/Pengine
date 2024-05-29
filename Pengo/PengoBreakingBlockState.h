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
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() },
			m_Direction{ dir }
		{ 
			m_pObserver->RegisterForEvent(m_pObserver, PengoBlockBreakEvent::PENGO_BLOCKBR_EVENT_NAME, [this](const Pengin::BaseEvent& ev) {OnBlockBreakEvent(ev); });
		}

		void OnEnter();
		std::unique_ptr<Pengin::PlayerState> Update(const Pengin::UserIndex& userIndex);

	private:
		std::shared_ptr<Pengin::Observer> m_pObserver;
		const glm::vec2 m_Direction{  };

		void OnBlockBreakEvent(const Pengin::BaseEvent& event);
	};
}

#endif