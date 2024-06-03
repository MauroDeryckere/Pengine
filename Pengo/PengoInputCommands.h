#ifndef PENGO_INPUTCOMMANDS
#define PENGO_INPUTCOMMANDS

#include "InputCommand.h"
#include <memory>

namespace Pengo
{
	class Movement final : public Pengin::InputCommand
	{
	public:
		Movement(const Pengin::UserIndex& user, const glm::vec3& direction) :
			Pengin::InputCommand{ user, "PengoMovement" },
			m_Direction{ direction }
		{ }

		void Execute() {};

		const glm::vec3& GetDirection() const noexcept { return m_Direction; }

		virtual ~Movement() override = default;

		Movement(const Movement&) = delete;
		Movement& operator=(const Movement&) = delete;
		Movement(Movement&&) noexcept = delete;
		Movement& operator=(Movement&&) noexcept = delete;

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

		virtual ~BreakBlock() override = default;

		BreakBlock(const BreakBlock&) = delete;
		BreakBlock& operator=(const BreakBlock&) = delete;
		BreakBlock(BreakBlock&&) noexcept = delete;
		BreakBlock& operator=(BreakBlock&&) noexcept = delete;
	};

	class PengoPlayGame final : public Pengin::InputCommand
	{
	public:
		PengoPlayGame(const Pengin::UserIndex& user) :
			Pengin::InputCommand{ user, "PengoPlayGame" }
		{ }

		void Execute();

		virtual ~PengoPlayGame() override = default;

		PengoPlayGame(const PengoPlayGame&) = delete;
		PengoPlayGame& operator=(const PengoPlayGame&) = delete;
		PengoPlayGame(PengoPlayGame&&) noexcept = delete;
		PengoPlayGame& operator=(PengoPlayGame&&) noexcept = delete;
	};

	class PengoSkipLevel final : public Pengin::InputCommand
	{
	public:
		PengoSkipLevel(const Pengin::UserIndex& user) :
			Pengin::InputCommand{ user, "PengoSkipLevel" }
		{ }

		void Execute();

		virtual ~PengoSkipLevel() override = default;

		PengoSkipLevel(const PengoSkipLevel&) = delete;
		PengoSkipLevel& operator=(const PengoSkipLevel&) = delete;
		PengoSkipLevel(PengoSkipLevel&&) noexcept = delete;
		PengoSkipLevel& operator=(PengoSkipLevel&&) noexcept = delete;
	};
}

#endif