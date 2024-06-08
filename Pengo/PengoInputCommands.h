#ifndef PENGO_INPUTCOMMANDS
#define PENGO_INPUTCOMMANDS

#include "InputCommand.h"
#include <memory>

namespace Pengo
{
	//Pengo
	class Movement final : public Pengin::InputCommand
	{
	public:
		Movement(const Pengin::UserIndex& user, const glm::vec3& direction) :
			Pengin::InputCommand{ user, "PengoMovement" },
			m_Direction{ direction }
		{ }

		void Execute() {};

		const glm::vec3& GetDirection() const noexcept { return m_Direction; }

		~Movement() = default;

	private:
		const glm::vec3 m_Direction;
	};

	class BreakBlock final : public Pengin::InputCommand
	{
	public:
		BreakBlock(const Pengin::UserIndex& user) :
			Pengin::InputCommand{ user, "PengoBreakBlock" }
		{ }

		void Execute() {}

		~BreakBlock() = default;
	};

	//Game
	class PengoPlayGame final : public Pengin::InputCommand
	{
	public:
		PengoPlayGame(const Pengin::UserIndex& user) :
			Pengin::InputCommand{ user, "PengoPlayGame" }
		{ }

		void Execute();

		~PengoPlayGame() = default;
	};

	class PengoSkipLevel final : public Pengin::InputCommand
	{
	public:
		PengoSkipLevel(const Pengin::UserIndex& user) :
			Pengin::InputCommand{ user, "PengoSkipLevel" }
		{ }

		void Execute();

		~PengoSkipLevel() = default;
	};

	class MuteSounds final : public Pengin::InputCommand
	{
	public:
		MuteSounds(const Pengin::UserIndex& user) :
			Pengin::InputCommand{ user, "MuteSound" }
		{ }

		void Execute();

		~MuteSounds() = default;
	};
	//-------
}

#endif