#ifndef INPUTCOMMAND
#define INPUTCOMMAND

#include "CoreIncludes.h"

#include <string>

namespace Pengin
{
	using UserIndex = GameUUID;
	class InputCommand
	{
	public:
		virtual void Execute() = 0;
		virtual ~InputCommand() = default;

		const std::string& GetActionName() const noexcept { return m_ActionName; }

		InputCommand(const InputCommand&) = delete;
		InputCommand& operator=(const InputCommand&) = delete;
		InputCommand(InputCommand&&) noexcept = delete;
		InputCommand& operator=(InputCommand&&) noexcept = delete;

	protected:
		InputCommand(const UserIndex& user, const std::string& actionName) :
			m_UserIdx{ user },
			m_ActionName{ actionName }
		{ 
			assert(user);
			assert(!actionName.empty());
		}

		const UserIndex& GetUserIdx() const noexcept { return m_UserIdx; }

	private:
		const UserIndex m_UserIdx;
		const std::string m_ActionName;
	};
}

#endif