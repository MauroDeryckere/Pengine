#ifndef WORLDPOSSYSTEM
#define WORLDPOSSYSTEM

#include "BaseSystem.h"

namespace Pengin
{
	class ECS;
	struct TransformComponent;

	class WorldPositionSystem final : public BaseSystem
	{
	public:
		WorldPositionSystem(ECS& ecs) :
			BaseSystem{ }, 
			m_ECS{ ecs }
		{}

		virtual ~WorldPositionSystem() override = default;

		virtual void FixedUpdate() override;

		WorldPositionSystem(const WorldPositionSystem&) = delete;
		WorldPositionSystem(WorldPositionSystem&&) = delete;
		WorldPositionSystem& operator=(const WorldPositionSystem&) = delete;
		WorldPositionSystem& operator=(const WorldPositionSystem&&) = delete;

	private:
		ECS& m_ECS;

		void UpdateWorldPosition(TransformComponent& transform);
	};
}

#endif
