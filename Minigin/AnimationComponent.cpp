#include "AnimationComponent.h"

#include "Time.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UtilStructs.h"
#include "TransformComponent.h"

Pengin::AnimationComponent::AnimationComponent(EntityId id, const std::string_view spriteSheet, const std::vector<SpriteAnimationData>& animations, bool playOnInit):
	m_Animations{ animations },
	m_pSpriteSheet{ dae::ResourceManager::GetInstance().LoadTexture(spriteSheet) },
	m_FrameTimer{ 0.f },
	m_Id{ id },
	m_CurrAnimationIdx{ 0 },
	m_CurrFrame{ 0 },
	m_IsPlaying{ playOnInit }

{
	assert(std::all_of(animations.begin(), animations.end(), [](const SpriteAnimationData& anim)
		{
			return anim.frameDuration > 0.f;
		})
		&& "All animation frame durations must be positive");

	assert(std::all_of(animations.begin(), animations.end(), [](const SpriteAnimationData& anim)
		{
			return anim.startX >= 0 && anim.startY >= 0;
		})
		&& "All animation start positions must be >= 0");

	assert(std::all_of(animations.begin(), animations.end(), [](const SpriteAnimationData& anim)
		{
			return anim.width > 0 && anim.height > 0;
		})
		&& "All animation width / heights must be valid (> 0)");

	assert(std::all_of(animations.begin(), animations.end(), [](const SpriteAnimationData& anim)
		{
			return anim.frameCt > 0;
		})
		&& "All animations need at least one frame");
}

void Pengin::AnimationComponent::Render() const
{
	auto& ecs = ECS::GetInstance();
	auto& transform = ecs.GetComponent<TransformComponent>(m_Id);

	SpriteAnimationData currAnimation{ m_Animations[m_CurrAnimationIdx] };

	const UtilStructs::Recti dstRect{ static_cast<int>(transform.m_Position.x + currAnimation.width),
									  static_cast<int>(transform.m_Position.y + currAnimation.height),
									  static_cast<unsigned>(transform.m_Scale.x * currAnimation.width),
									  static_cast<unsigned>(transform.m_Scale.y * currAnimation.height) };

	const UtilStructs::Recti srcRect{ currAnimation.startX + currAnimation.width * m_CurrFrame,
									  currAnimation.startY,
									  currAnimation.width,
									  currAnimation.height };

	dae::Renderer::GetInstance().RenderTexture(*m_pSpriteSheet, dstRect, srcRect);
}

void Pengin::AnimationComponent::Update()
{
	if (!m_IsPlaying)
	{
		return;
	}

	m_FrameTimer += Time::GetInstance().GetElapsedSec();

	if (m_FrameTimer >= m_Animations[m_CurrAnimationIdx].frameDuration)
	{
		m_FrameTimer -= m_Animations[m_CurrAnimationIdx].frameDuration;
		++m_CurrFrame;

		if (m_CurrFrame > m_Animations[m_CurrAnimationIdx].frameCt - 1)
		{
			m_CurrFrame = 0;
		}
	}
}

void Pengin::AnimationComponent::SwitchAnimation(unsigned aniIdx)
{
	assert(aniIdx < m_Animations.size());

	m_CurrAnimationIdx = static_cast<uint8_t>(aniIdx);
}
