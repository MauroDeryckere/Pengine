#ifndef MOVEMENTSYSTEM
#define MOVEMENTSYSTEM

#include "BaseSystem.h"

namespace Pengin
{
	class ECS;

	class MovementSystem final
	{
	public:
		MovementSystem(ECS& ecs) :
			m_ECS{ ecs }
		{}

		~MovementSystem() = default;

		void FirstStep(const float stepTime);

		void Step(const float stepTime);

		MovementSystem(const MovementSystem&) = delete;
		MovementSystem(MovementSystem&&) = delete;
		MovementSystem& operator=(const MovementSystem&) = delete;
		MovementSystem& operator=(const MovementSystem&&) = delete;

	private:
		ECS& m_ECS;
	};
}

#endif
