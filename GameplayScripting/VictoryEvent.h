#pragma once

#include "InputManager.h"
#include "Event.h"

namespace GS
{
	class VictoryEvent final : public Pengin::BaseEvent
	{
	public:
		VictoryEvent(const Pengin::UserIndex& userIdx):
			Pengin::BaseEvent{ "Victory" },
			m_UserIndex{ userIdx }
		{
			assert(userIdx);
		}

		~VictoryEvent() = default;

		const Pengin::UserIndex& GetUserIndex() const noexcept
		{
			return m_UserIndex;
		}

	private:
		const Pengin::UserIndex m_UserIndex{};
	};
}