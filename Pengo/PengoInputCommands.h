#ifndef PENGO_INPUTCOMMANDS
#define PENGO_INPUTCOMMANDS

#include "EventManager.h"
#include "InputCommand.h"

#include "Entity.h"
#include "SceneManager.h"

#include "BodyComponent.h"
#include "PlayerComponent.h"

#include "PengoComponent.h"

#include "PengoBlockBreakEvent.h"

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

		virtual void Execute() override;

		const glm::vec3& GetDirection() const noexcept { return m_Direction; }

		virtual ~Movement() override = default;

		Movement(const Movement&) = delete;
		Movement& operator=(const Movement&) = delete;
		Movement(Movement&&) noexcept = delete;
		Movement& operator=(Movement&&) noexcept = delete;

	private:
		const glm::vec3 m_Direction;
	};

	class AttackPlayer final : public Pengin::InputCommand //bound to input for now
	{
	public:
		AttackPlayer(const Pengin::UserIndex& user) :
			InputCommand{ user, "Attack" }
		{ }

		virtual void Execute() override;
		virtual ~AttackPlayer() override = default;

		AttackPlayer(const AttackPlayer&) = delete;
		AttackPlayer& operator=(const AttackPlayer&) = delete;
		AttackPlayer(AttackPlayer&&) noexcept = delete;
		AttackPlayer& operator=(AttackPlayer&&) noexcept = delete;

	private:
	};

	class CollectScore final : public Pengin::InputCommand //bound to input for now
	{
	public:
		CollectScore(const Pengin::UserIndex& user, unsigned score = 10) :
			InputCommand{ user, "ScoreCollect" },
			m_ScoreVal{ score }
		{ }

		virtual void Execute() override;
		virtual ~CollectScore() override = default;

		CollectScore(const CollectScore&) = delete;
		CollectScore& operator=(const CollectScore&) = delete;
		CollectScore(CollectScore&&) noexcept = delete;
		CollectScore& operator=(CollectScore&&) noexcept = delete;

	private:
		const unsigned m_ScoreVal;
	};

	class BreakBlock final : public Pengin::InputCommand
	{
	public:
		BreakBlock(const Pengin::UserIndex& user) :
			Pengin::InputCommand{ user, "PengoBreakBlock" }
		{ }

		virtual void Execute() override;

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
			Pengin::InputCommand{ user, "PlayGame" }
		{ }

		virtual void Execute() override;

		virtual ~PengoPlayGame() override = default;

		PengoPlayGame(const PengoPlayGame&) = delete;
		PengoPlayGame& operator=(const PengoPlayGame&) = delete;
		PengoPlayGame(PengoPlayGame&&) noexcept = delete;
		PengoPlayGame& operator=(PengoPlayGame&&) noexcept = delete;
	};
}

#endif