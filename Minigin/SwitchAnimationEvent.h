#ifndef PENGIN_SWITCHANIMATIONEVENT
#define PENGIN_SWITCHANIMATIONEVENT

#include "Event.h"
#include "EntityId.h"

#include <cassert>
#include <variant>

namespace Pengin
{
	class SwitchAnimationEvent final : public BaseEvent
	{
	public:

		enum class SwitchAnimationEventFrame : uint8_t
		{
			First = 0,
			KeepCurrent = 1, //will pick last if the current is over the amount of frames for the given animation
			Last = 2
		};
		
		struct NewFrame final
		{
			std::variant<SwitchAnimationEventFrame, uint8_t> frame;

			[[nodiscard]] bool IsCustomFrame() const noexcept
			{
				return std::holds_alternative<uint8_t>(frame);
			}

			NewFrame() :
				frame{ SwitchAnimationEventFrame::First } {}

			NewFrame(SwitchAnimationEventFrame mode) : 
				frame{mode} {}

			NewFrame(uint8_t idx) : 
				frame{idx} {}

			~NewFrame() = default;
		};

		static constexpr const char* SWITCH_ANIMATION_NAME{ "OnAniSwitch" };

		SwitchAnimationEvent(const EntityId entityId, const uint8_t newAnimationIdx, bool keepPrevTime = false, bool isPlaying = true, NewFrame newFrame = {}) :
			BaseEvent{ SWITCH_ANIMATION_NAME },

			m_EntityId{ entityId },
			m_NewAnimationIdx{ newAnimationIdx },

			m_KeepPrevTime{ keepPrevTime },

			m_IsPlaying{ isPlaying },
			m_NewFrame { newFrame }
		{
			assert(m_EntityId != NULL_ENTITY_ID);
		}

		~SwitchAnimationEvent() = default;

		[[nodiscard]] const EntityId GetEntityId() const noexcept
		{
			return m_EntityId;
		}

		[[nodiscard]] const uint8_t NewAniIdx() const noexcept
		{
			return m_NewAnimationIdx;
		}

		[[nodiscard]] const bool KeepPrevTime() const noexcept
		{
			return m_KeepPrevTime;
		}

		[[nodiscard]] const bool IsPlaying() const noexcept
		{
			return m_IsPlaying;
		}

		[[nodiscard]] const NewFrame GetNewFrame() const noexcept
		{
			return m_NewFrame;
		}

		SwitchAnimationEvent(const SwitchAnimationEvent&) = delete;
		SwitchAnimationEvent(SwitchAnimationEvent&&) = delete;
		SwitchAnimationEvent& operator=(const SwitchAnimationEvent&) = delete;
		SwitchAnimationEvent& operator=(const SwitchAnimationEvent&&) = delete;

	private:
		const EntityId m_EntityId{ NULL_ENTITY_ID };
		const uint8_t m_NewAnimationIdx{ 0 };

		const bool m_KeepPrevTime{ false };
		const bool m_IsPlaying{ true };

		const NewFrame m_NewFrame{ SwitchAnimationEventFrame::First };
	};
}

#endif