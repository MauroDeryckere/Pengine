#ifndef PENGIN_COLLISIONEVENT
#define PENGIN_COLLISIONEVENT

#include "Event.h"
#include "EntityId.h"

#include <cassert>

namespace Pengin
{
	class CollisionEvent final : public BaseEvent
	{
	public:
		constexpr static const char* COLLISION_EVENT_NAME{ "OnCollisionEvent" };

		CollisionEvent(const EntityId entityA, const EntityId entityB) :
			BaseEvent{ COLLISION_EVENT_NAME },

			m_EntityA{ entityA },
			m_EntityB{ entityB }
		{
			assert(entityA != NULL_ENTITY_ID);
			assert(entityB != NULL_ENTITY_ID);
		}

		~CollisionEvent() = default;

		[[nodiscard]] const EntityId GetEntityA() const noexcept
		{
			return m_EntityA;
		}

		[[nodiscard]] const EntityId GetEntityB() const noexcept
		{
			return m_EntityB;
		}

		CollisionEvent(const CollisionEvent&) = delete;
		CollisionEvent(CollisionEvent&&) = delete;
		CollisionEvent& operator=(const CollisionEvent&) = delete;
		CollisionEvent& operator=(const CollisionEvent&&) = delete;

	private:
		const EntityId m_EntityA{ NULL_ENTITY_ID };
		const EntityId m_EntityB{ NULL_ENTITY_ID };
	};
}

#endif