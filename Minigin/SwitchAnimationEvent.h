#ifndef PENGIN_SWITCHANIMATIONEVENT
#define PENGIN_SWITCHANIMATIONEVENT

#include "Event.h"
#include "EntityId.h"

#include <cassert>

namespace Pengin
{
	class SwitchAnimationEvent final : public BaseEvent
	{
	public:
		constexpr static const char* SWITCH_ANIMATION_NAME{ "OnAniSwitch" };

		SwitchAnimationEvent(const EntityId entityId, const uint8_t newAnimationIdx) :
			BaseEvent{ SWITCH_ANIMATION_NAME },

			m_EntityId{ entityId },
			m_NewAnimationIdx{ newAnimationIdx }
		{
			assert(m_EntityId != NULL_ENTITY_ID);
		}

		~SwitchAnimationEvent() = default;

		[[nodiscard]] const EntityId GetEntityId() const noexcept
		{
			return m_EntityId;
		}

		[[nodiscard]] const uint8_t GetNewAniIdx() const noexcept
		{
			return m_NewAnimationIdx;
		}

		SwitchAnimationEvent(const SwitchAnimationEvent&) = delete;
		SwitchAnimationEvent(SwitchAnimationEvent&&) = delete;
		SwitchAnimationEvent& operator=(const SwitchAnimationEvent&) = delete;
		SwitchAnimationEvent& operator=(const SwitchAnimationEvent&&) = delete;

	private:
		const EntityId m_EntityId{ NULL_ENTITY_ID };
		const uint8_t m_NewAnimationIdx{ 0 };
	};
}

#endif