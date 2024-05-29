#include "PengoIdleState.h"

#include "PengoWalkState.h"
#include "PengoBreakingBlockState.h"
#include "PengoInputCommands.h"
#include "SwitchAnimationEvent.h"
#include "EventManager.h"

#include "InputManager.h"
#include "GridSystem.h"
#include "PengoGrid.h"
#include "OnGridTag.h"

namespace Pengo
{
	PengoIdleState::PengoIdleState(const Pengin::UserIndex& userIdx, const glm::vec2& dir = {}) :
		Pengin::PlayerState{ userIdx },
		m_Direction{ dir }
	{ }

	void PengoIdleState::OnEnter()
	{
		using namespace Pengin;

		auto playerEntity{ Pengin::SceneManager::GetInstance().GetActiveScene()->GetPlayer(Pengin::PlayerState::GetUserIndex()) };

		if (m_Direction.x > 0.f)
		{
			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
				playerEntity.GetEntityId(),
				static_cast<uint8_t>(PlayerSystem::PengoAnimations::IdleRight)));
		}
		else if (m_Direction.x < 0.f)
		{
			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
				playerEntity.GetEntityId(),
				static_cast<uint8_t>(PlayerSystem::PengoAnimations::IdleLeft)));
		}
		else if (m_Direction.y > 0.f)
		{
			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
				playerEntity.GetEntityId(),
				static_cast<uint8_t>(PlayerSystem::PengoAnimations::IdleDown)));
		}
		else if (m_Direction.y < 0.f)
		{
			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
				playerEntity.GetEntityId(),
				static_cast<uint8_t>(PlayerSystem::PengoAnimations::IdleUp)));
			}
	}

	std::unique_ptr<Pengin::PlayerState> Pengo::PengoIdleState::HandleInput(const Pengin::UserIndex& userIndex)
	{
		if (coold)
		{
			coold = false;
			return nullptr;
		}

		std::shared_ptr<Pengin::InputCommand> pCommand;
		if (Pengin::InputManager::GetInstance().IsActionExecuted(userIndex, "PengoMovement", &pCommand))
		{
			assert(pCommand);
			Pengo::Movement* pMovement{ static_cast<Pengo::Movement*>(pCommand.get()) };

			return std::move(std::make_unique<PengoWalkState>(userIndex, pMovement->GetDirection()));
		}
		if (Pengin::InputManager::GetInstance().IsActionExecuted(userIndex, "PengoBreakBlock"))
		{
			if (ValidateBlockBreak())
			{
				return std::move(std::make_unique<PengoBreakingBlockState>(userIndex, m_Direction));
			}
		}

		return nullptr;
	}

	bool PengoIdleState::ValidateBlockBreak()
	{
		using namespace Pengin;

		auto pActiveScene{ SceneManager::GetInstance().GetActiveScene() };
		auto playerEntity{ pActiveScene->GetPlayer(PlayerState::GetUserIndex()) };

		GridSystem* pGridSys = pActiveScene->GetSystem<GridSystem>();
		assert(pGridSys);

		assert(playerEntity.HasComponent<OnGridTag>());
		const auto& gridTag{ playerEntity.GetComponent<OnGridTag>() };

		const auto& cellCoords{ pGridSys->GetCellCoords(playerEntity.GetEntityId()) };

		const int row{ static_cast<int>(cellCoords.first) + static_cast<int>(m_Direction.y) };
		const int col{ static_cast<int>(cellCoords.second) + static_cast<int>(m_Direction.x) };

		if (!pGridSys->IsWithinBounds(gridTag.gridId, static_cast<uint16_t>(row), static_cast<uint16_t>(col)) || row < 0 || col < 0)
		{
			//Play sound TODO
			return false;
		}

		const auto pos = pGridSys->GetCellPos(gridTag.gridId, static_cast<uint16_t>(row), static_cast<uint16_t>(col));

		const auto& cellData = pGridSys->GetCellData(gridTag.gridId, static_cast<uint16_t>(row), static_cast<uint16_t>(col));
		PengoCellType type = static_cast<PengoCellType>(cellData.type);

		return (type == Pengo::PengoCellType::Wall);
	}


}

