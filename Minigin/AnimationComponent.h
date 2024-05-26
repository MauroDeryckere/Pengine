#ifndef ANIMATIONCOMPONENT
#define ANIMATIONCOMPONENT

#include "UtilStructs.h"
#include <vector>
#include <cassert>
#include <algorithm>

#include "SerializationRegistry.h"

#include "SwitchAnimationEvent.h"

namespace Pengin
{ 
	struct AnimationData final
	{
		UtilStructs::Rectu16 frame0sourceRect{};

		float frameDuration{}; //0.f frame duration means we never switch a frame, some animations could have a single frame animation like an idle frame,...

		uint8_t frameCt{};

		static void Serialize(const FieldSerializer& fieldSer, const AnimationData& serStruct, std::vector<uint8_t>& fieldVector, const ECS& ecs)
		{
			const std::vector<uint16_t> serSrcRect{ serStruct.frame0sourceRect.x, serStruct.frame0sourceRect.y, serStruct.frame0sourceRect.width, serStruct.frame0sourceRect.height };

			fieldSer.SerializeField("Frame0SrcRect", serSrcRect, ecs, fieldVector);
			fieldSer.SerializeField("FrameDuration", serStruct.frameDuration, ecs, fieldVector);
			fieldSer.SerializeField("FrameCount", serStruct.frameCt, ecs, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, AnimationData& deserStruct, const std::unordered_map<std::string, std::vector<uint8_t>>& fields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			std::vector<uint16_t> deserSrcRect{};

			fieldSer.DeserializeField("Frame0SrcRect", deserSrcRect, fields, entityMap);

			deserStruct.frame0sourceRect = UtilStructs::Rectu16{ deserSrcRect[0], deserSrcRect[1], deserSrcRect[2], deserSrcRect[3] };

			fieldSer.DeserializeField("FrameDuration", deserStruct.frameDuration, fields, entityMap);
			fieldSer.DeserializeField("FrameCount", deserStruct.frameCt, fields, entityMap);
		}
	};

	struct AnimationComponent final
	{
		std::vector<AnimationData> animations { };
		
		float frameTimer { 0.f };

		uint8_t currAnimationIdx { 0 };
		uint8_t currFrame { 0 };

		bool isPlaying { true };

		AnimationComponent(const std::vector<AnimationData>& animations = {}, uint8_t initAnimationIdx = 0, bool playOnInit = true) :
			animations{ animations },
			isPlaying{ playOnInit },
			currAnimationIdx{ initAnimationIdx }
		{

			assert(std::all_of(animations.begin(), animations.end(), [](const AnimationData& animation) { return animation.frameCt > 0; }) && "Can not have an animation with 0 frames");
			assert(std::all_of(animations.begin(), animations.end(), [](const AnimationData& animation) { return animation.frameDuration >= 0.f; }) && "Can not have an animation with negative");
			assert(std::all_of(animations.begin(), animations.end(), [](const AnimationData& animation) { return animation.frame0sourceRect; }) && "Must provide valid frame sourceRect");
		}

		~AnimationComponent() = default;

		void ChangeAnimation(const uint8_t newAnimatonIndex, bool keepPrevTime, bool startPlaying, SwitchAnimationEvent::NewFrame newFrame) noexcept
		{
			assert(newAnimatonIndex < animations.size());
			currAnimationIdx = newAnimatonIndex;

			frameTimer = keepPrevTime ?  std::min<float>(frameTimer, animations[newAnimatonIndex].frameDuration)
									  :  0.f;

			isPlaying = startPlaying;

			if (newFrame.IsCustomFrame())
			{
				assert(std::get<uint8_t>(newFrame.frame) < animations[newAnimatonIndex].frameCt);
				currFrame = std::get<uint8_t>(newFrame.frame);
			}
			else
			{
				switch (std::get<SwitchAnimationEvent::SwitchAnimationEventFrame>(newFrame.frame))
				{
				case SwitchAnimationEvent::SwitchAnimationEventFrame::First:
					currFrame = 0;
					break;

				case SwitchAnimationEvent::SwitchAnimationEventFrame::KeepCurrent:
					currFrame = std::clamp<uint8_t>(currFrame, 0, animations[newAnimatonIndex].frameCt - 1);
					break;

				case SwitchAnimationEvent::SwitchAnimationEventFrame::Last:
					currFrame = animations[newAnimatonIndex].frameCt - 1;
					break;
				default:
					break;
				}
			}
		}


		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<AnimationComponent>(id);

			fieldSer.SerializeField("AnimationData", comp.animations, ecs, fieldVector);

			fieldSer.SerializeField("FrameTimer", comp.frameTimer, ecs, fieldVector);
			fieldSer.SerializeField("CurrAnimationIdx", comp.currAnimationIdx, ecs, fieldVector);
			fieldSer.SerializeField("CurrFrame", comp.currFrame, ecs, fieldVector);
			fieldSer.SerializeField("IsPlaying", comp.isPlaying, ecs, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<AnimationComponent>(id);

			fieldSer.DeserializeField("AnimationData", comp.animations, serializedFields, entityMap);

			fieldSer.DeserializeField("FrameTimer", comp.frameTimer, serializedFields, entityMap);

			fieldSer.DeserializeField("CurrAnimationIdx", comp.currAnimationIdx, serializedFields, entityMap);
			fieldSer.DeserializeField("CurrFrame", comp.currFrame, serializedFields, entityMap);
			fieldSer.DeserializeField("IsPlaying", comp.isPlaying, serializedFields, entityMap);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(AnimationComponent, AnimationComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(AnimationComponent, AnimationComponent::Deserialize);
}


#endif