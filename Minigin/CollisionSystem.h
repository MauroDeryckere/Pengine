#ifndef COLLISIONSYSTEM
#define COLLISIONSYSTEM

#include "UtilStructs.h"

namespace Pengin
{
	class ECS;

	class CollisionSystem final
	{
	public:
		CollisionSystem(ECS& ecs) :
			m_ECS{ ecs }
		{}

		~CollisionSystem() = default;

		void Update();

		CollisionSystem(const CollisionSystem&) = delete;
		CollisionSystem(CollisionSystem&&) = delete;
		CollisionSystem& operator=(const CollisionSystem&) = delete;
		CollisionSystem& operator=(const CollisionSystem&&) = delete;

	private:
		ECS& m_ECS;

		[[nodiscard]] inline constexpr bool IsCollidingABBA(UtilStructs::Rectu16 rect1, UtilStructs::Rectu16 rect2) const noexcept;
	};
}

#endif
