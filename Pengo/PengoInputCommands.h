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

		virtual void Execute() override;

		virtual ~BreakBlock() override = default;

		BreakBlock(const BreakBlock&) = delete;
		BreakBlock& operator=(const BreakBlock&) = delete;
		BreakBlock(BreakBlock&&) noexcept = delete;
		BreakBlock& operator=(BreakBlock&&) noexcept = delete;
	};
}

#endif