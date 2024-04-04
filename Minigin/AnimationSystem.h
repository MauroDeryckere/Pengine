#ifndef ANIMATIONSYSTEM
#define ANIMATIONSYSTEM

namespace Pengin
{
	class ECS;

	class AnimationSystem final
	{
	public:
		AnimationSystem(ECS& ecs) :
			m_ECS{ ecs }
		{}

		~AnimationSystem() = default;

		void Update();

		AnimationSystem(const AnimationSystem&) = delete;
		AnimationSystem(AnimationSystem&&) = delete;
		AnimationSystem& operator=(const AnimationSystem&) = delete;
		AnimationSystem& operator=(const AnimationSystem&&) = delete;

	private:
		ECS& m_ECS;
	};
}

#endif
