#ifndef ANIMATIONSYSTEM
#define ANIMATIONSYSTEM

#include "BaseSystem.h"
#include "Observer.h"

namespace Pengin
{
	class ECS;
	class BaseEvent;

	class AnimationSystem final : public BaseSystem
	{
	public:
		AnimationSystem(ECS& ecs);
		virtual ~AnimationSystem() override = default;

		virtual void Update() override;

		AnimationSystem(const AnimationSystem&) = delete;
		AnimationSystem(AnimationSystem&&) = delete;
		AnimationSystem& operator=(const AnimationSystem&) = delete;
		AnimationSystem& operator=(const AnimationSystem&&) = delete;

	private:
		ECS& m_ECS;

		std::shared_ptr<Observer> m_pObserer;

		void OnSwitchAnimationEvent(const BaseEvent& event) noexcept;
	};
}

#endif
