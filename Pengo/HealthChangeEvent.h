#ifndef PENGIN_HEALTHCHANGEEVENT
#define PENGIN_HEALTHCHANGEEVENT

#include "Event.h"
#include "EntityId.h"

namespace Pengo
{
	class HealthChangeEvent final : public Pengin::BaseEvent
	{
	public:
		HealthChangeEvent(const std::string& eventName, const Pengin::EntityId entityId):
			BaseEvent{ eventName },
			m_EntityID{ entityId }
		{ 
			assert(entityId != Pengin::NULL_ENTITY_ID);
		}

		virtual ~HealthChangeEvent() override = default;

		[[nodiscard]] const Pengin::EntityId GetEntityId() const noexcept { return m_EntityID; }
		
		HealthChangeEvent(const HealthChangeEvent&) = delete;
		HealthChangeEvent(HealthChangeEvent&&) = delete;
		HealthChangeEvent& operator=(const HealthChangeEvent&) = delete;
		HealthChangeEvent& operator=(const HealthChangeEvent&&) = delete;

	private:
		const Pengin::EntityId m_EntityID{ Pengin::NULL_ENTITY_ID };
	};
}

#endif