#ifndef PENGO_SNOOBEEWALKSTATE
#define PENGO_SNOOBEEWALKSTATE

#include "State.h"

#include <glm/vec2.hpp>

namespace Pengo
{
	class SnobeeWalkState final : public Pengin::State
	{
	public:
		SnobeeWalkState(Pengin::EntityId id) :
			State{ id },
			m_Direction{ CalcDirection() }
		{ }

		~SnobeeWalkState() = default;

		void OnEnter();
		std::unique_ptr<Pengin::State> Update();

	private:
		bool m_CheckedCollision{ false };

		glm::vec2 m_Direction{ };

		glm::vec2 CalcDirection();
	};
}

#endif