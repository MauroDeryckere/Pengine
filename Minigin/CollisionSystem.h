#ifndef COLLISIONSYSTEM
#define COLLISIONSYSTEM

#include "BaseSystem.h"

#include "UtilStructs.h"

namespace Pengin
{
	class ECS;

	class CollisionSystem final : public BaseSystem
	{
	public:
		CollisionSystem(ECS& ecs) :
			BaseSystem{ },
			m_ECS{ ecs }
		{}

		virtual ~CollisionSystem() override = default;

		virtual void Update() override;

		CollisionSystem(const CollisionSystem&) = delete;
		CollisionSystem(CollisionSystem&&) = delete;
		CollisionSystem& operator=(const CollisionSystem&) = delete;
		CollisionSystem& operator=(const CollisionSystem&&) = delete;

	private:
		ECS& m_ECS;

		[[nodiscard]] inline constexpr bool IsCollidingABBA(UtilStructs::Rect16 rect1, UtilStructs::Rect16 rect2) const noexcept;
	};
}

#endif
