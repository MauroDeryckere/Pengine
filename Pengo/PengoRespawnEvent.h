#ifndef PENGO_RESPAWNEVENT
#define PENGO_RESPAWNEVENT

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
	class PengoRespawnEvent final : public Pengin::BaseEvent
	{
	public:
		constexpr static const char* PENGO_BLOCKBR_EVENT_NAME{ "OnPengoRespawn" };

		PengoRespawnEvent(const Pengin::UserIndex& userIndex) :
			Pengin::BaseEvent{ PENGO_BLOCKBR_EVENT_NAME },

			m_UserIdx{ userIndex }
		{
			assert(userIndex);
		}

		~PengoRespawnEvent() = default;

		[[nodiscard]] const Pengin::UserIndex& GetUserIndex() const noexcept
		{
			return m_UserIdx;
		}

		PengoRespawnEvent(const PengoRespawnEvent&) = delete;
		PengoRespawnEvent(PengoRespawnEvent&&) = delete;
		PengoRespawnEvent& operator=(const PengoRespawnEvent&) = delete;
		PengoRespawnEvent& operator=(const PengoRespawnEvent&&) = delete;

	private:
		const Pengin::UserIndex m_UserIdx{ true };
	};
}

#endif