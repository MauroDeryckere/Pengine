#ifndef PENGO_LEVELWONEVENT
#define PENGO_LEVELWONEVENT

#include "GameUUID.h"
#include "Event.h"
#include <cassert>

namespace Pengo
{
	class LevelWonEvent final : public Pengin::BaseEvent
	{
	public:
		LevelWonEvent(const Pengin::GameUUID& userIdx) :
			BaseEvent{ "LevelWon" },
			m_UserIdx{ userIdx }
		{
			assert(userIdx != Pengin::GameUUID::INVALID_UUID);
		}

		~LevelWonEvent() = default;

		[[nodiscard]] const Pengin::GameUUID& GetUserIdx() const noexcept { return m_UserIdx; }

	private:
		const Pengin::GameUUID m_UserIdx;
	};
}

#endif