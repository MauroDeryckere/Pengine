#ifndef PENGO_SNOBEEDEATHEVENT
#define PENGO_SNOBEEDEATHEVENT

#include "Event.h"
#include "EntityId.h"
#include "GameUUID.h"

#include <cassert>

namespace Pengin
{
	using UserIndex = GameUUID;
}

namespace Pengo
{
	class SnobeeDeathEvent final : public Pengin::BaseEvent
	{
	public:
		constexpr static const char* PENGO_BLOCKBR_EVENT_NAME{ "SnobeeDeath" };

		SnobeeDeathEvent(const Pengin::EntityId killerId) :
			Pengin::BaseEvent{ PENGO_BLOCKBR_EVENT_NAME },

			m_KillerId{ killerId }
		{
			assert(killerId != Pengin::NULL_ENTITY_ID);
		}

		~SnobeeDeathEvent() = default;

		[[nodiscard]] const Pengin::EntityId GetKillerId() const noexcept
		{
			return m_KillerId;
		}

		SnobeeDeathEvent(const SnobeeDeathEvent&) = delete;
		SnobeeDeathEvent(SnobeeDeathEvent&&) = delete;
		SnobeeDeathEvent& operator=(const SnobeeDeathEvent&) = delete;
		SnobeeDeathEvent& operator=(const SnobeeDeathEvent&&) = delete;

	private:
		const Pengin::EntityId m_KillerId{ Pengin::NULL_ENTITY_ID };
	};
}

#endif