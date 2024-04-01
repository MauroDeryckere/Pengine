#ifndef WORLDPOSSYSTEM
#define WORLDPOSSYSTEM

namespace Pengin
{
	class ECS;
	struct TransformComponent;

	class WorldPositionSystem final
	{
	public:
		WorldPositionSystem(ECS& ecs) :
			m_ECS{ ecs }
		{}

		~WorldPositionSystem() = default;

		void Update();

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
