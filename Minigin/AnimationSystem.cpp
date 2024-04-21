#include "AnimationSystem.h"

#include "ECS.h"
#include "AnimationComponent.h"
#include "SpriteComponent.h"

#include "Time.h"

namespace Pengin
{ 
	void AnimationSystem::Update()
	{
		const float elapsedSec = Time::GetInstance().GetElapsedSec();
		auto animationComps{ m_ECS.GetComponents<AnimationComponent>() };
		
		for (auto it{ animationComps.begin() }; auto& entity : animationComps)
		{
			entity.m_FrameTimer += elapsedSec;

			if (entity.m_FrameTimer >= entity.animations[entity.currAnimationIdx].frameDuration)
			{
				++entity.currFrame %= entity.animations[entity.currAnimationIdx].frameCt;

				const auto id = animationComps.GetIdFromIterator(it);
				auto& spriteComp = m_ECS.GetComponent<SpriteComponent>(id);

				auto newSrcRect = entity.animations[entity.currAnimationIdx].frame0sourceRect;
				newSrcRect.x += entity.currFrame * newSrcRect.width;
				newSrcRect.y += entity.currFrame * newSrcRect.height;

				spriteComp.sourceRect = newSrcRect;

				entity.m_FrameTimer -= entity.animations[entity.currAnimationIdx].frameDuration;
			}

			++it;
		}
	}
}
