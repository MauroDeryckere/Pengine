#ifndef PENGIN_HEALTHCHANGEEVENT
#define PENGIN_HEALTHCHANGEEVENT

#include "Event.h"
#include "EntityId.h"

//TODO move to pengo

namespace Pengin
{
	class HealthChangeEvent final : public BaseEvent
	{
	public:
		HealthChangeEvent(const std::string& eventName, const EntityId entityId):
			BaseEvent{ eventName },
			m_EntityID{ entityId }
		{ 
			assert(entityId != NULL_ENTITY_ID);
		}

		virtual ~HealthChangeEvent() override = default;

		[[nodiscard]] const EntityId GetEntityId() const noexcept { return m_EntityID; }
		
		HealthChangeEvent(const HealthChangeEvent&) = delete;
		HealthChangeEvent(HealthChangeEvent&&) = delete;
		HealthChangeEvent& operator=(const HealthChangeEvent&) = delete;
		HealthChangeEvent& operator=(const HealthChangeEvent&&) = delete;

	private:
		const EntityId m_EntityID{ NULL_ENTITY_ID };
	};
}

#endif