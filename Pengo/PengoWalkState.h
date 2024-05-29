#ifndef PENGOWALKSTATE
#define PENGOWALKSTATE

#include "PlayerState.h"
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

			m_GoalPos{ }
		{ }

		void OnEnter();

		std::unique_ptr<Pengin::PlayerState> HandleInput(const Pengin::UserIndex& userIndex);
		std::unique_ptr<Pengin::PlayerState> Update(const Pengin::UserIndex& userIndex);

	private:
		const glm::vec3 m_Direction{ };
		glm::vec3 m_GoalPos{ };

		bool m_CheckedColl{ false };

		bool CheckCollision();
	};
}

#endif