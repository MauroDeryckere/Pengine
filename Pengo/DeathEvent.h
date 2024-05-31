#ifndef PENGO_DEATHEVENT
#define PENGO_DEATHEVENT

#include "Event.h"
#include "EntityId.h"

namespace Pengo
{
	class DeathEvent final : public Pengin::BaseEvent
	{
	public:
		DeathEvent(const std::string& eventName, const Pengin::EntityId entityId):
			BaseEvent{ eventName },
			m_EntityID{ entityId }
		{ 
			assert(entityId != Pengin::NULL_ENTITY_ID);
		}

		virtual ~DeathEvent() override = default;

		[[nodiscard]] const Pengin::EntityId GetEntityId() const noexcept { return m_EntityID; }
		
		DeathEvent(const DeathEvent&) = delete;
		DeathEvent(DeathEvent&&) = delete;
		DeathEvent& operator=(const DeathEvent&) = delete;
		DeathEvent& operator=(const DeathEvent&&) = delete;

	private:
		const Pengin::EntityId m_EntityID{ Pengin::NULL_ENTITY_ID };
	};
}

#endif