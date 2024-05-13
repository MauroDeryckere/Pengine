#ifndef INPUTCOMMAND
#define INPUTCOMMAND

#include "CoreIncludes.h"

#include "glm/vec3.hpp"

#include <unordered_map>
#include <string>
#include <any>
#include <memory>

namespace Pengin
{
	using UserIndex = GameUUID;
	class InputCommand abstract
	{
	public:
		virtual void Execute() = 0;
		virtual ~InputCommand() = default;

		InputCommand(const InputCommand&) = delete;
		InputCommand& operator=(const InputCommand&) = delete;
		InputCommand(InputCommand&&) noexcept = delete;
		InputCommand& operator=(InputCommand&&) noexcept = delete;

	protected:
		InputCommand(const UserIndex& user, const std::string& actionName) :
			m_UserIdx{ user },
			m_ActionName{ actionName }
		{ 
			assert(!actionName.empty());
		}

		const UserIndex& GetUserIdx() const noexcept { return m_UserIdx; }
		const std::string& GetActionName() const noexcept { return m_ActionName; }

	private:
		const UserIndex m_UserIdx;
		const std::string m_ActionName;
	};
}

#endif