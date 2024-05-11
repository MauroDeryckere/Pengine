#ifndef COLLISIONSYSTEM
#define COLLISIONSYSTEM

#include "BaseSystem.h"

#include "EntityId.h"
#include "EventManager.h"

#include "UtilStructs.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <unordered_set>

namespace Pengin
{
	class ECS;

	struct TransformComponent;
	struct RectColliderComponent;
	struct BodyComponent;

	class CollisionSystem final
	{
	public:
		CollisionSystem(ECS& ecs, bool CollisionResolution = true);

		~CollisionSystem() = default;

		void Step();

		void BroadCastCollisionsEvents() noexcept;

		CollisionSystem(const CollisionSystem&) = delete;
		CollisionSystem(CollisionSystem&&) = delete;
		CollisionSystem& operator=(const CollisionSystem&) = delete;
		CollisionSystem& operator=(const CollisionSystem&&) = delete;

	private:
		ECS& m_ECS;

		bool m_CollisionResolution{ true };

		struct CollPair final
		{
			const EntityId entityA;
			const EntityId entityB;

			bool operator==(const CollPair& other) const noexcept
			{
				return (entityA == other.entityA && entityB == other.entityB) ||
					(entityA == other.entityB && entityB == other.entityA);
			}
		};

		struct CollisionPairHash final
		{
			std::size_t operator()(const CollPair& pair) const noexcept
			{
				return std::hash<int>()(pair.entityA) ^ std::hash<int>()(pair.entityB);
			}
		};

		std::unordered_set<CollPair, CollisionPairHash> m_FrameCollisions{};

		constexpr UtilStructs::Rectf&& CalcCollRect(const TransformComponent& transform, const RectColliderComponent& rColl) const noexcept;
		constexpr UtilStructs::Rectf&& CalcCollRect(const BodyComponent* body, const TransformComponent& transform, const RectColliderComponent& rColl) const noexcept;

		const glm::vec3 CalcCollNormal(const UtilStructs::Rectf& rectA, const UtilStructs::Rectf& rectB) const noexcept;
		const float CalcPenetrationDeth(const glm::vec3& normal, const UtilStructs::Rectf& rectA, const UtilStructs::Rectf& rectB) const noexcept;

		void SeparateBodies(BodyComponent* bodyA, BodyComponent* bodyB, const glm::vec3& normal, const float penDepth) noexcept;
	
	};
}

#endif
