#ifndef PHYSICSSYSTEM
#define PHYSICSSYSTEM

#include "BaseSystem.h"
#include "MovementSystem.h"
#include "WorldPositionSystem.h"
#include "CollisionSystem.h"

#include <memory>

namespace Pengin
{
	class ECS;

	//class CollisionSystem;
	//class MovementSystem;
	//class WorldPositionSystem;

	struct TransformComponent;

	class PhysicsSystem final : public BaseSystem
	{
	public:
		PhysicsSystem(ECS& ecs, const uint32_t numIterations = 10);

		virtual ~PhysicsSystem() override = default;

		virtual void FixedUpdate() override;

		PhysicsSystem(const PhysicsSystem&) = delete;
		PhysicsSystem(PhysicsSystem&&) = delete;
		PhysicsSystem& operator=(const PhysicsSystem&) = delete;
		PhysicsSystem& operator=(const PhysicsSystem&&) = delete;

	private:
		ECS& m_ECS;

		const uint32_t m_NumIterations{ 1 };

		std::unique_ptr<CollisionSystem> m_pCollisionSystem;
		std::unique_ptr<MovementSystem> m_pMovementSystem;
		std::unique_ptr<WorldPositionSystem> m_pWorldposSystem;
	};
}

#endif
