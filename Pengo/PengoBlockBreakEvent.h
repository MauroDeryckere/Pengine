#ifndef PENGO_BLOCKBREAKEVENT
#define PENGO_BLOCKBREAKEVENT

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
	class PengoBlockBreakEvent final : public Pengin::BaseEvent
	{
	public:
		constexpr static const char* PENGO_BLOCKBR_EVENT_NAME{ "OnPengoBlockBreak" };

		PengoBlockBreakEvent(const Pengin::UserIndex& userIndex, Pengin::EntityId blockId) :
			Pengin::BaseEvent{ PENGO_BLOCKBR_EVENT_NAME },

			m_UserIdx{ userIndex },
			m_BlockId{ blockId }
		{
			assert(userIndex);
			assert(blockId != Pengin::NULL_ENTITY_ID);
		}

		~PengoBlockBreakEvent() = default;

		[[nodiscard]] const Pengin::UserIndex& GetUserIndex() const noexcept
		{
			return m_UserIdx;
		}

		[[nodiscard]] Pengin::EntityId GetBlockId() const noexcept
		{
			return m_BlockId;
		}


		PengoBlockBreakEvent(const PengoBlockBreakEvent&) = delete;
		PengoBlockBreakEvent(PengoBlockBreakEvent&&) = delete;
		PengoBlockBreakEvent& operator=(const PengoBlockBreakEvent&) = delete;
		PengoBlockBreakEvent& operator=(const PengoBlockBreakEvent&&) = delete;

	private:
		const Pengin::UserIndex m_UserIdx{ true };
		const Pengin::EntityId m_BlockId{ Pengin::NULL_ENTITY_ID };
	};
}

#endif