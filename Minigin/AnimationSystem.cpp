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

			if (entity.m_FrameTimer >= entity.m_Animations[entity.m_CurrAnimationIdx].frameDuration)
			{
				++entity.m_CurrFrame %= entity.m_Animations[entity.m_CurrAnimationIdx].frameCt;

				const auto id = animationComps.GetIdFromIterator(it);
				auto& spriteComp = m_ECS.GetComponent<SpriteComponent>(id);

				auto newSrcRect = entity.m_Animations[entity.m_CurrAnimationIdx].frame0sourceRect;
				newSrcRect.x += entity.m_CurrFrame * newSrcRect.width;
				newSrcRect.y += entity.m_CurrFrame * newSrcRect.height;

				spriteComp.m_SourceRect = newSrcRect;

				entity.m_FrameTimer -= entity.m_Animations[entity.m_CurrAnimationIdx].frameDuration;
			}

			++it;
		}
	}
}
