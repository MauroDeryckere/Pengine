#ifndef PENGO_RESPAWNEVENT
#define PENGO_RESPAWNEVENT

#include "Event.h"
#include "EntityId.h"
#include "GameUUID.h"

#include <cassert>

namespace Pengo
{
	using namespace Pengin;

	using UserIndex = GameUUID;

	class PengoRespawnEvent final : public BaseEvent
	{
	public:
		constexpr static const char* PENGO_BLOCKBR_EVENT_NAME{ "OnPengoRespawn" };

		PengoRespawnEvent(const UserIndex& userIndex) :
			BaseEvent{ PENGO_BLOCKBR_EVENT_NAME },

			m_UserIdx{ userIndex }
		{
			assert(userIndex);
		}

		~PengoRespawnEvent() = default;

		[[nodiscard]] const UserIndex& GetUserInex() const noexcept
		{
			return m_UserIdx;
		}

		PengoRespawnEvent(const PengoRespawnEvent&) = delete;
		PengoRespawnEvent(PengoRespawnEvent&&) = delete;
		PengoRespawnEvent& operator=(const PengoRespawnEvent&) = delete;
		PengoRespawnEvent& operator=(const PengoRespawnEvent&&) = delete;

	private:
		const UserIndex m_UserIdx{ true };
	};
}

#endif