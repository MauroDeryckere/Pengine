#ifndef PENGIN_COLLISIONEVENT
#define PENGIN_COLLISIONEVENT

#include "Event.h"
#include "EntityId.h"

#include "CollisionTypes.h"

#include <cassert>

namespace Pengin
{
	class CollisionEvent final : public BaseEvent
	{
	public:
		constexpr static const char* COLLISION_EVENT_NAME{ "OnCollisionEvent" };

		CollisionEvent(const EntityId entityA, const EntityId entityB, CollType collTypeA, CollType collTypeB) :
			BaseEvent{ COLLISION_EVENT_NAME },

			m_EntityA{ entityA },
			m_EntityB{ entityB },

			m_CollTypeA{ collTypeA },
			m_CollTypeB{ collTypeB }

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

		[[nodiscard]] const CollType GetCollTypeA() const noexcept
		{
			return m_CollTypeA;
		}
		[[nodiscard]] const CollType GetCollTypeB() const noexcept
		{
			return m_CollTypeB;
		}

		CollisionEvent(const CollisionEvent&) = delete;
		CollisionEvent(CollisionEvent&&) = delete;
		CollisionEvent& operator=(const CollisionEvent&) = delete;
		CollisionEvent& operator=(const CollisionEvent&&) = delete;

	private:
		const EntityId m_EntityA{ NULL_ENTITY_ID };
		const EntityId m_EntityB{ NULL_ENTITY_ID };

		const CollType m_CollTypeA{};
		const CollType m_CollTypeB{};
	};
}

#endif