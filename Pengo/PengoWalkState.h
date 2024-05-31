#ifndef PENGOWALKSTATE
#define PENGOWALKSTATE

#include "PlayerState.h"
#include "CollisionEvent.h"
#include "DebugOutput.h"
#include <glm/vec3.hpp>
#include <string>

namespace Pengin
{
	class GridSystem;
}

namespace Pengo
{
	class PengoWalkState final : public Pengin::PlayerState
	{
	public:
		PengoWalkState(const Pengin::UserIndex& userIdx, const glm::vec3& direction) :
			Pengin::PlayerState{ userIdx },
			m_Direction{ direction },

			m_CheckedColl{ false },

			m_GoalPos{ },
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }
		{
			m_pObserver->RegisterForEvent(m_pObserver, Pengin::CollisionEvent::COLLISION_EVENT_NAME, [this](const Pengin::BaseEvent& event) {OnCollision(event); });
		}

		void OnEnter();

		std::unique_ptr<Pengin::PlayerState> Update(const Pengin::UserIndex& userIndex);

	private:
		const glm::vec3 m_Direction{ };
		glm::vec3 m_GoalPos{ };

		bool m_CheckedColl{ false };

		std::shared_ptr<Pengin::Observer> m_pObserver;

		void OnCollision(const Pengin::BaseEvent& event);
		bool CheckCollision();
	};
}

#endif