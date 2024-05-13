#ifndef ANIMATIONSYSTEM
#define ANIMATIONSYSTEM

#include "BaseSystem.h"
#include "EventManager.h"

#include "SwitchAnimationEvent.h"

namespace Pengin
{
	class ECS;

	class AnimationSystem final : public BaseSystem
	{
	public:
		AnimationSystem(ECS& ecs) :
			BaseSystem{ },
			m_ECS{ ecs },
			m_pObserer{ EventManager::GetInstance().CreateObserver() }
		{
			m_pObserer->RegisterForEvent(m_pObserer, SwitchAnimationEvent::SWITCH_ANIMATION_NAME, [this](const BaseEvent& event) { OnSwitchAnimationEvent(event); });
		}

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
