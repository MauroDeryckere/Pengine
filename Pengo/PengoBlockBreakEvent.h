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

		PengoBlockBreakEvent(const Pengin::UserIndex& userIndex) :
			Pengin::BaseEvent{ PENGO_BLOCKBR_EVENT_NAME },

			m_UserIdx{ userIndex }
		{
			assert(userIndex);
		}

		~PengoBlockBreakEvent() = default;

		[[nodiscard]] const Pengin::UserIndex& GetUserInex() const noexcept
		{
			return m_UserIdx;
		}

		PengoBlockBreakEvent(const PengoBlockBreakEvent&) = delete;
		PengoBlockBreakEvent(PengoBlockBreakEvent&&) = delete;
		PengoBlockBreakEvent& operator=(const PengoBlockBreakEvent&) = delete;
		PengoBlockBreakEvent& operator=(const PengoBlockBreakEvent&&) = delete;

	private:
		const Pengin::UserIndex m_UserIdx{ true };
	};
}

#endif