#ifndef PENGIN_PLAYERSTATE
#define PENGIN_PLAYERSTATE

#include <memory>
#include <iostream>
#include <string>

#include "GameUUID.h"

namespace Pengin
{
	using UserIndex = GameUUID;

	class PlayerState
	{
	public:
		PlayerState(const UserIndex& userIdx) :
			m_UserIdx{ userIdx }
		{
			assert(userIdx);
		}

		virtual ~PlayerState() = default;

		virtual void OnEnter() {}
		
		virtual std::unique_ptr<PlayerState> HandleInput(const UserIndex&) { return nullptr; }
		virtual std::unique_ptr<PlayerState> Update(const UserIndex&) { return nullptr; }

		virtual void OnExit() {}

	protected:
		[[nodiscard]] const UserIndex& GetUserIndex() const noexcept
		{
			return m_UserIdx;
		}

	private:
		const UserIndex m_UserIdx{};
	};
}

#endif
