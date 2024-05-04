#ifndef ANIMATIONCOMPONENT
#define ANIMATIONCOMPONENT

#include "UtilStructs.h"
#include <vector>
#include <cassert>
#include <algorithm>

#include "SerializationRegistry.h"

namespace Pengin
{ 
	struct AnimationData final
	{
		UtilStructs::Rectu16 frame0sourceRect;

		float frameDuration;

		uint8_t frameCt;

		static void Serialize(const FieldSerializer& fieldSer, const AnimationData& serStruct, std::vector<uint8_t>& fieldVector)
		{
			const std::vector<uint16_t> serSrcRect{ serStruct.frame0sourceRect.x, serStruct.frame0sourceRect.y, serStruct.frame0sourceRect.width, serStruct.frame0sourceRect.height };

			fieldSer.SerializeField("Frame0SrcRect", serSrcRect, fieldVector);
			fieldSer.SerializeField("FrameDuration", serStruct.frameDuration, fieldVector);
			fieldSer.SerializeField("FrameCount", serStruct.frameCt, fieldVector);
		}
	};

	struct AnimationComponent final
	{
		AnimationComponent(const std::vector<AnimationData>& animations = {}, uint8_t initAnimationIdx = 0, bool playOnInit = true) :
			animations{ animations },
			isPlaying{ playOnInit },
			currAnimationIdx{ initAnimationIdx }
		{

			assert(std::all_of(animations.begin(), animations.end(), [](const AnimationData& animation) { return animation.frameCt > 0; }) && "Can not have an animation with 0 frames");
			assert(std::all_of(animations.begin(), animations.end(), [](const AnimationData& animation) { return animation.frameDuration > 0.f; }) && "Can not have an animation with negative or 0 frame duration");
			assert(std::all_of(animations.begin(), animations.end(), [](const AnimationData& animation) { return animation.frame0sourceRect; }) && "Must provide valid frame sourceRect");
		}
		
		~AnimationComponent() = default;

		std::vector<AnimationData> animations { };
		
		float m_FrameTimer { 0.f };

		uint8_t currAnimationIdx { 0 };
		uint8_t currFrame { 0 };

		bool isPlaying { true };

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<AnimationComponent>(id);

			fieldSer.SerializeField("AnimationData", comp.animations, fieldVector);

			fieldSer.SerializeField("FrameTimer", comp.m_FrameTimer, fieldVector);
			fieldSer.SerializeField("CurrAnimationIdx", comp.currAnimationIdx, fieldVector);
			fieldSer.SerializeField("CurrFrame", comp.currFrame, fieldVector);
			fieldSer.SerializeField("IsPlaying", comp.isPlaying, fieldVector);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(AnimationComponent, AnimationComponent::Serialize);
}


#endif