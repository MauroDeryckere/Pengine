#ifndef ANIMATIONSYSTEM
#define ANIMATIONSYSTEM

#include "BaseSystem.h"
#include "EventManager.h"

namespace Pengin
{
	class ECS;

	class AnimationSystem final : public BaseSystem
	{
	public:
		AnimationSystem(ECS& ecs);
		~AnimationSystem() = default;

		void Update();

		AnimationSystem(const AnimationSystem&) = delete;
		AnimationSystem(AnimationSystem&&) = delete;
		AnimationSystem& operator=(const AnimationSystem&) = delete;
		AnimationSystem& operator=(const AnimationSystem&&) = delete;

	private:
		ECS& m_ECS;

		std::shared_ptr<Observer> m_pObserer;

		void OnSwitchAnimationEvent(const BaseEvent& event);
	};
}

#endif
