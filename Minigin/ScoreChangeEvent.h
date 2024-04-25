#ifndef PENGIN_SCORECHANGEEVENT
#define PENGIN_SCORECHANGEEVENT

#include "Event.h"
#include "EntityId.h"

namespace Pengin
{
	class ScoreChangeEvent final : public BaseEvent
	{
	public:
		ScoreChangeEvent(const std::string& eventName, const EntityId entityId):
			BaseEvent{ eventName },
			m_EntityID{ entityId }
		{ 
			assert(entityId != NULL_ENTITY_ID);
		}

		virtual ~ScoreChangeEvent() override = default;

		[[nodiscard]] const EntityId GetEntityId() const noexcept { return m_EntityID; }

		ScoreChangeEvent(const ScoreChangeEvent&) = delete;
		ScoreChangeEvent(ScoreChangeEvent&&) = delete;
		ScoreChangeEvent& operator=(const ScoreChangeEvent&) = delete;
		ScoreChangeEvent& operator=(const ScoreChangeEvent&&) = delete;

	private:
		const EntityId m_EntityID{ NULL_ENTITY_ID };
	};
}

#endif