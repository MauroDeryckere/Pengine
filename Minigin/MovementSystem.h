#ifndef MOVEMENTSYSTEM
#define MOVEMENTSYSTEM

#include "BaseSystem.h"

namespace Pengin
{
	class ECS;
	struct TransformComponent;

	class MovementSystem final : public BaseSystem
	{
	public:
		MovementSystem(ECS& ecs) :
			BaseSystem{ },
			m_ECS{ ecs }
		{}

		virtual ~MovementSystem() override = default;

		virtual void Update() override;

		MovementSystem(const MovementSystem&) = delete;
		MovementSystem(MovementSystem&&) = delete;
		MovementSystem& operator=(const MovementSystem&) = delete;
		MovementSystem& operator=(const MovementSystem&&) = delete;

	private:
		ECS& m_ECS;

		void SetPosDirty(TransformComponent& transform);
	};
}

#endif
