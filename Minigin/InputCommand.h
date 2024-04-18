#ifndef INPUTCOMMAND
#define INPUTCOMMAND

#include "CoreIncludes.h"

#include "UUID.h"
#include "glm/vec3.hpp"

#include <unordered_map>
#include <string>
#include <any>
#include <memory>

namespace Pengin
{
	using UserIndex = UUID;
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
		InputCommand(const UserIndex& user) :
			m_UserIdx{ user }
		{ }

		const UserIndex& GetUserIdx() const noexcept { return m_UserIdx; }

	private:
		UserIndex m_UserIdx;
	};
}

#endif