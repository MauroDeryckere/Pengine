#ifndef MOVEMENTSYSTEM
#define MOVEMENTSYSTEM

namespace Pengin
{
	class ECS;
	struct TransformComponent;

	class MovementSystem final
	{
	public:
		MovementSystem(ECS& ecs) :
			m_ECS{ ecs }
		{}

		~MovementSystem() = default;

		void Update();

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
