#ifndef ANIMATIONSYSTEM
#define ANIMATIONSYSTEM

#include "BaseSystem.h"

namespace Pengin
{
	class ECS;

	class AnimationSystem final : public BaseSystem
	{
	public:
		AnimationSystem(ECS& ecs) :
			BaseSystem{ },
			m_ECS{ ecs }
		{}

		virtual ~AnimationSystem() override = default;

		virtual void Update() override;

		AnimationSystem(const AnimationSystem&) = delete;
		AnimationSystem(AnimationSystem&&) = delete;
		AnimationSystem& operator=(const AnimationSystem&) = delete;
		AnimationSystem& operator=(const AnimationSystem&&) = delete;

	private:
		ECS& m_ECS;
	};
}

#endif
