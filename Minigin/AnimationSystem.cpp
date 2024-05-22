#include "AnimationSystem.h"

#include "ECS.h"
#include "AnimationComponent.h"
#include "SpriteComponent.h"

#include "GameTime.h"

namespace Pengin
{ 
	void AnimationSystem::Update()
	{
		const float elapsedSec = GameTime::GetInstance().GetElapsedSec();
		auto animationComps{ m_ECS.GetComponents<AnimationComponent>() };
		
		for (auto it{ animationComps.begin() }; auto& entity : animationComps)
		{
			entity.frameTimer += elapsedSec;

			if (entity.frameTimer >= entity.animations[entity.currAnimationIdx].frameDuration)
			{
				++entity.currFrame %= entity.animations[entity.currAnimationIdx].frameCt;

				const auto id = animationComps.GetIdFromIterator(it);
				auto& spriteComp = m_ECS.GetComponent<SpriteComponent>(id);

				auto newSrcRect = entity.animations[entity.currAnimationIdx].frame0sourceRect;
				newSrcRect.x += entity.currFrame * newSrcRect.width;

				spriteComp.sourceRect = newSrcRect;

				entity.frameTimer -= entity.animations[entity.currAnimationIdx].frameDuration;
			}

			++it;
		}
	}

	void AnimationSystem::OnSwitchAnimationEvent(const BaseEvent& event) noexcept
	{
		const auto& aniEv = static_cast<const SwitchAnimationEvent&>(event);

		assert(m_ECS.HasComponent<AnimationComponent>(aniEv.GetEntityId()));

		auto& aniComp = m_ECS.GetComponent<AnimationComponent>(aniEv.GetEntityId());

		if (aniComp.currAnimationIdx != aniEv.NewAniIdx())
		{
			aniComp.ChangeAnimation(aniEv.NewAniIdx(), aniEv.KeepPrevTime(), aniEv.IsPlaying(), aniEv.GetNewFrame());

			auto newSrcRect = aniComp.animations[aniComp.currAnimationIdx].frame0sourceRect;
			newSrcRect.x += aniComp.currFrame * newSrcRect.width;

			auto& spriteComp = m_ECS.GetComponent<SpriteComponent>(aniEv.GetEntityId());
			spriteComp.sourceRect = newSrcRect;
		}
	}
}
