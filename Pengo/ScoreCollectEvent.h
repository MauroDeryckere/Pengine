#ifndef PENGO_SCORECOLLECTEVENT
#define PENGO_SCORECOLLECTEVENT

#include "Event.h"
#include "EntityId.h"

namespace Pengo
{
	class ScoreCollectEvent final : public Pengin::BaseEvent
	{
	public:
		ScoreCollectEvent(unsigned score, const Pengin::EntityId entityId):
			BaseEvent{ "ScoreCollect" },
			m_Score{ score },
			m_EntityID{ entityId }
		{ 
			assert(entityId != Pengin::NULL_ENTITY_ID);
		}

		virtual ~ScoreCollectEvent() override = default;

		[[nodiscard]] const Pengin::EntityId GetEntityId() const noexcept { return m_EntityID; }
		[[nodiscard]] const unsigned GetScore() const noexcept { return m_Score; }

		ScoreCollectEvent(const ScoreCollectEvent&) = delete;
		ScoreCollectEvent(ScoreCollectEvent&&) = delete;
		ScoreCollectEvent& operator=(const ScoreCollectEvent&) = delete;
		ScoreCollectEvent& operator=(const ScoreCollectEvent&&) = delete;

	private:
		const unsigned m_Score{};
		const Pengin::EntityId m_EntityID{ Pengin::NULL_ENTITY_ID };
	};
}

#endif