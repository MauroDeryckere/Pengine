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
		std::unique_ptr<Pengin::State> Update()
		{
			m_CheckedCollision = true;
			///TODO pengo movement pattern
			return nullptr;
		
			//Check all directions && decide what to do
			//store a temp list of all available dirs

			//A: 
			//Out of border, skip this dir

			//B: 
			//Wall
			//Enter blockbreak state

			//C:
			//Walkable
			//Stay in this state until reached, then repeat process

			//Need idle or decisionmake state?
		}

	private:
		bool m_CheckedCollision{ false };

		glm::vec2 m_Direction{ };

		glm::vec2 CalcDirection();
	};
}

#endif