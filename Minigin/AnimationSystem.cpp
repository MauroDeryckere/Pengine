#include "AnimationSystem.h"

#include "EventManager.h"
#include "SwitchAnimationEvent.h"

#include "ECS.h"
#include "AnimationComponent.h"
#include "SceneManager.h"
#include "SpriteComponent.h"

#include "Entity.h"
#include "GameTime.h"

#include <algorithm>
#include <execution>

namespace Pengin
{
	AnimationSystem::AnimationSystem(ECS& ecs):
		BaseSystem{ },
		m_ECS{ ecs },
		m_pObserer{ EventManager::GetInstance().CreateObserver() }
	{
		m_pObserer->RegisterForEvent(m_pObserer, SwitchAnimationEvent::SWITCH_ANIMATION_NAME, [this](const BaseEvent& event) { OnSwitchAnimationEvent(event); });
	}

	void AnimationSystem::Update()
	{
		const float elapsedSec = GameTime::GetInstance().ElapsedSec();
		auto animationComps{ m_ECS.GetComponents<AnimationComponent>() };

		std::for_each(std::execution::par_unseq, animationComps.begin(), animationComps.end(),
			[this, &elapsedSec, &animationComps](AnimationComponent& entity)
			{
				if (entity.animations[entity.currAnimationIdx].frameDuration == 0.f)
				{
					return;
				}

				entity.frameTimer += elapsedSec;

				if (entity.frameTimer >= entity.animations[entity.currAnimationIdx].frameDuration)
				{
					++entity.currFrame %= entity.animations[entity.currAnimationIdx].frameCt;

					auto it = animationComps.begin() + std::distance(&*animationComps.begin(), &entity);

					const auto id = animationComps.GetIdFromIterator(it);
					auto& spriteComp = m_ECS.GetComponent<SpriteComponent>(id);

					auto newSrcRect = entity.animations[entity.currAnimationIdx].frame0sourceRect;
					newSrcRect.x += entity.currFrame * newSrcRect.width;

					spriteComp.sourceRect = newSrcRect;

					entity.frameTimer -= entity.animations[entity.currAnimationIdx].frameDuration;
				}
			});
	}

	void AnimationSystem::OnSwitchAnimationEvent(const BaseEvent& event)
	{
		const auto& aniEv = static_cast<const SwitchAnimationEvent&>(event);
		auto entity = Entity{ aniEv.GetEntityId(), SceneManager::GetInstance().GetActiveScene() };

		assert(entity.HasComponent<AnimationComponent>());
		if (entity.HasComponent<AnimationComponent>())
		{
			auto& aniComp = entity.GetComponent<AnimationComponent>();

			if (aniComp.currAnimationIdx != aniEv.NewAniIdx())
			{
				aniComp.ChangeAnimation(aniEv.NewAniIdx(), aniEv.KeepPrevTime(), aniEv.IsPlaying(), aniEv.GetNewFrame());

				auto newSrcRect = aniComp.animations[aniComp.currAnimationIdx].frame0sourceRect;
				newSrcRect.x += aniComp.currFrame * newSrcRect.width;

				auto& spriteComp = entity.GetComponent<SpriteComponent>();
				spriteComp.sourceRect = newSrcRect;
			}
		}
	}
}
