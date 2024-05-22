#ifndef PENGIN_SCORECHANGEEVENT
#define PENGIN_SCORECHANGEEVENT

#include "Event.h"
#include "EntityId.h"

namespace Pengo
{
	class ScoreChangeEvent final : public Pengin::BaseEvent
	{
	public:
		ScoreChangeEvent(const std::string& eventName, const Pengin::EntityId entityId):
			BaseEvent{ eventName },
			m_EntityID{ entityId }
		{ 
			assert(entityId != Pengin::NULL_ENTITY_ID);
		}

		virtual ~ScoreChangeEvent() override = default;

		[[nodiscard]] const Pengin::EntityId GetEntityId() const noexcept { return m_EntityID; }

		ScoreChangeEvent(const ScoreChangeEvent&) = delete;
		ScoreChangeEvent(ScoreChangeEvent&&) = delete;
		ScoreChangeEvent& operator=(const ScoreChangeEvent&) = delete;
		ScoreChangeEvent& operator=(const ScoreChangeEvent&&) = delete;

	private:
		const Pengin::EntityId m_EntityID{ Pengin::NULL_ENTITY_ID };
	};
}

#endif