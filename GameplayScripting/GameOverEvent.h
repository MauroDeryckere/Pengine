#pragma once

#include "InputManager.h"
#include "Event.h"

namespace GS
{
	class GameOverEvent final : public Pengin::BaseEvent
	{
	public:
		GameOverEvent(const Pengin::UserIndex& userIdx):
			Pengin::BaseEvent{"GameOver"},
			m_UserIndex{ userIdx }
		{ 
			assert(userIdx);
		}

		~GameOverEvent() = default;

		const Pengin::UserIndex& GetUserIndex() const noexcept
		{
			return m_UserIndex;
		}

	private:
		const Pengin::UserIndex m_UserIndex{};
	};
}