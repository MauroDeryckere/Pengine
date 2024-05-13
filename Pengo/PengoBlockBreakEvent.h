#ifndef PENGO_BLOCKBREAKEVENT
#define PENGO_BLOCKBREAKEVENT

#include "Event.h"
#include "EntityId.h"
#include "GameUUID.h"

#include <cassert>

namespace Pengo
{
	using namespace Pengin;

	using UserIndex = GameUUID;

	class PengoBlockBreakEvent final : public BaseEvent
	{
	public:
		constexpr static const char* PENGO_BLOCKBR_EVENT_NAME{ "OnPengoBlockBreak" };

		PengoBlockBreakEvent(const UserIndex& userIndex) :
			BaseEvent{ PENGO_BLOCKBR_EVENT_NAME },

			m_UserIdx{ userIndex }
		{
			assert(userIndex);
		}

		~PengoBlockBreakEvent() = default;

		[[nodiscard]] const UserIndex& GetUserInex() const noexcept
		{
			return m_UserIdx;
		}

		PengoBlockBreakEvent(const PengoBlockBreakEvent&) = delete;
		PengoBlockBreakEvent(PengoBlockBreakEvent&&) = delete;
		PengoBlockBreakEvent& operator=(const PengoBlockBreakEvent&) = delete;
		PengoBlockBreakEvent& operator=(const PengoBlockBreakEvent&&) = delete;

	private:
		const UserIndex m_UserIdx{ true };
	};
}

#endif