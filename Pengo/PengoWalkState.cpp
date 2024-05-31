#include "PlayerSystem.h"
#include "PengoGrid.h"

#include "PengoWalkState.h"

#include "PengoIdleState.h"
#include "PengoBreakingBlockState.h"

#include "EventManager.h"
#include "SwitchAnimationEvent.h"

#include "InputManager.h"
#include "EventManager.h"

#include "GameTime.h"
#include "SceneManager.h"
#include "BodyComponent.h"
#include "PengoComponent.h"
#include "OnGridTag.h"
#include "SnobeeComponent.h"
#include "DeathEvent.h"
#include "Entity.h"

#include "GridSystem.h"
#include <cassert>

namespace Pengo
{
	void PengoWalkState::OnEnter()
	{
		using namespace Pengin;

		auto pActiveScene{ SceneManager::GetInstance().GetActiveScene() };
		auto playerEntity{ pActiveScene->GetPlayer(PlayerState::GetUserIndex()) };
		
		if (m_Direction.x > 0.f)
		{
			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
																playerEntity.GetEntityId(), 
																static_cast<uint8_t>(PlayerSystem::PengoAnimations::MoveRight)));
		}
		else if (m_Direction.x < 0.f)
		{
			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
																playerEntity.GetEntityId(), 
																static_cast<uint8_t>(PlayerSystem::PengoAnimations::MoveLeft)));
		}
		else if (m_Direction.y > 0.f)
		{
			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
																playerEntity.GetEntityId(), 
																static_cast<uint8_t>(PlayerSystem::PengoAnimations::MoveDown)));
		}
		else if (m_Direction.y < 0.f)
		{
			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
																playerEntity.GetEntityId(), 
																static_cast<uint8_t>(PlayerSystem::PengoAnimations::MoveUp)));
		}
	}

	std::unique_ptr<Pengin::PlayerState> PengoWalkState::Update(const Pengin::UserIndex& userIndex)
	{
		using namespace Pengin;

		auto pActiveScene{ SceneManager::GetInstance().GetActiveScene() };
		auto playerEntity{ pActiveScene->GetPlayer(PlayerState::GetUserIndex()) };

		if (playerEntity)
		{
			if (!m_CheckedColl)
			{
				if (CheckCollision())
				{
					return std::make_unique<PengoIdleState>(userIndex, m_Direction);
				}
			}
			
			auto& pengTransform = playerEntity.GetComponent<TransformComponent>();
			// Check if the player has reached the goal in any direction
			if ((m_Direction.x > 0 && pengTransform.worldPos.x >= m_GoalPos.x) ||
				(m_Direction.x < 0 && pengTransform.worldPos.x <= m_GoalPos.x) ||
				(m_Direction.y > 0 && pengTransform.worldPos.y >= m_GoalPos.y) ||
				(m_Direction.y < 0 && pengTransform.worldPos.y <= m_GoalPos.y)) 
			{
				// Clip to cell border
				playerEntity.SetLocalPosition(m_GoalPos);
				pengTransform.worldPos = m_GoalPos;

				return std::make_unique<PengoIdleState>(userIndex, m_Direction);
			}

			playerEntity.GetComponent<BodyComponent>().inputVelocity += m_Direction * playerEntity.GetComponent<PengoComponent>().movementSpeed;
			
			return nullptr;
		}

		return nullptr;
	}

	void PengoWalkState::OnCollision(const Pengin::BaseEvent& event)
	{
		using namespace Pengin;
		const auto& collEvent{ static_cast<const CollisionEvent&>(event) };

		auto activeScene = SceneManager::GetInstance().GetActiveScene();
		auto player = activeScene->GetPlayer(GetUserIndex());

		const auto entityA{ Pengin::Entity{ collEvent.GetEntityA(), activeScene.get()} };
		const auto entityB{ Pengin::Entity{ collEvent.GetEntityB(), activeScene.get()} };

		if (player.GetEntityId() == entityA.GetEntityId()
			&& entityA.HasComponent<PengoComponent>() && entityB.HasComponent<SnobeeComponent>())
		{
			EventManager::GetInstance().BroadcoastEvent(std::make_unique<DeathEvent>("PengoDeath", collEvent.GetEntityA()));
		}
		else if (player.GetEntityId() == entityB.GetEntityId()
			&& entityB.HasComponent<PengoComponent>() && entityA.HasComponent<SnobeeComponent>())
		{
			EventManager::GetInstance().BroadcoastEvent(std::make_unique<DeathEvent>("PengoDeath", collEvent.GetEntityB()));
		}
	}

	bool PengoWalkState::CheckCollision()
	{
		using namespace Pengin;

		m_CheckedColl = true;

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
			return true;
		}

		const auto pos = pGridSys->GetCellPos(gridTag.gridId, static_cast<uint16_t>(row), static_cast<uint16_t>(col));
		m_GoalPos = { pos.x, pos.y , 0.f };

		const auto& cellData = pGridSys->GetCellData(gridTag.gridId, static_cast<uint16_t>(row), static_cast<uint16_t>(col));
		PengoCellType type = static_cast<PengoCellType>(cellData.type);

		return (type == Pengo::PengoCellType::Wall);
	}
}