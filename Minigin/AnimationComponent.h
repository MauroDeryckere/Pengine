#ifndef ANIMATIONCOMPONENT
#define ANIMATIONCOMPONENT

#include "Texture2D.h"
#include "ECS.h"

#include <string_view>
#include <vector>
#include <memory>

//namespace Pengin
//{ 
//	class dae::Texture2D;
//	struct SpriteAnimationData final
//	{
//		float frameDuration;
//
//		uint16_t startX;
//		uint16_t startY;
//		uint16_t width;
//		uint16_t height;
//
//		uint8_t frameCt;
//	};
//
//	class AnimationComponent final
//	{
//	public:
//		AnimationComponent(EntityId id, const std::string_view spriteSheet, const std::vector<SpriteAnimationData>& animations, bool playOnInit = true);
//		~AnimationComponent() = default;
//
//		void Render() const;
//
//		void Update();
//
//		void SwitchAnimation(unsigned aniIdx);
//
//	private:
//		std::vector<SpriteAnimationData> m_Animations;
//		std::shared_ptr<dae::Texture2D> m_pSpriteSheet;
//
//		float m_FrameTimer;
//		const EntityId m_Id;
//
//		uint8_t m_CurrAnimationIdx;
//		uint8_t m_CurrFrame;
//
//		bool m_IsPlaying;
//	};
//}


#endif