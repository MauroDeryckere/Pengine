#ifndef PENGOIDLESTATE
#define PENGOIDLESTATE

#include "PlayerState.h"

#include "EventManager.h"
#include "SwitchAnimationEvent.h"

#include "DebugOutput.h"
#include "EntityId.h"
#include "SceneManager.h"
#include "PlayerSystem.h"
#include "Entity.h"

#include <string>
#include <glm/vec2.hpp>
#include <memory>

namespace Pengo
{
	class PengoIdleState final : public Pengin::PlayerState
	{
	public:
		PengoIdleState(const Pengin::UserIndex& userIdx, const glm::vec2& dir);

		void OnEnter();

		std::unique_ptr<Pengin::PlayerState> HandleInput(const Pengin::UserIndex& userIndex);

	private:
		bool coold{ true };
		const glm::vec2 m_Direction{};

		std::shared_ptr<Pengin::Observer> m_pObserver;

		void OnCollision(const Pengin::BaseEvent& event);
		bool ValidateBlockBreak();

	};
}

#endif