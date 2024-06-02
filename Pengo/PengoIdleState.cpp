#include "PengoIdleState.h"

#include "PengoWalkState.h"
#include "PengoPushingBlockState.h"
#include "PengoBreakingBlockState.h"
#include "PengoInputCommands.h"
#include "SwitchAnimationEvent.h"
#include "EventManager.h"
#include "SnobeeComponent.h"
#include "DeathEvent.h"

#include "BodyComponent.h"
#include "PengoComponent.h"
#include "BlockComponent.h"
#include "BlockSystem.h"

#include "InputManager.h"
#include "GridSystem.h"
#include "PengoGrid.h"
#include "OnGridTag.h"

namespace Pengo
{
	PengoIdleState::PengoIdleState(const Pengin::UserIndex& userIdx, const glm::vec2& dir = {}) :
		Pengin::PlayerState{ userIdx },
		m_Direction{ dir },
		m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }
	{
		m_pObserver->RegisterForEvent(m_pObserver, Pengin::CollisionEvent::COLLISION_EVENT_NAME, [this](const Pengin::BaseEvent& event) {OnCollision(event); });
	}

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
		if (m_Cooldown)
		{
			m_Cooldown = false;
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
			return ValidateBlockBreak();
		}

		return nullptr;
	}

	void PengoIdleState::OnCollision(const Pengin::BaseEvent& event)
	{
		if (m_Cooldown)
		{
			return;
		}

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

	std::unique_ptr<Pengin::PlayerState> PengoIdleState::ValidateBlockBreak()
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
			return nullptr;
		}

		const auto pos = pGridSys->GetCellPos(gridTag.gridId, static_cast<uint16_t>(row), static_cast<uint16_t>(col));

		auto& cellData = pGridSys->GetCellData(gridTag.gridId, static_cast<uint16_t>(row), static_cast<uint16_t>(col));
		PengoCellType type = static_cast<PengoCellType>(cellData.type);
		
		if (type == Pengo::PengoCellType::Block)
		{
			Entity block{ cellData.entity, pActiveScene.get() };

			auto& blockComp = block.GetComponent<BlockComponent>();

			if (blockComp.blockState == BlockComponent::BlockState::Still)
			{
				//check if we should push or break
				bool push = true;

				const int nextRow{ static_cast<int>(cellCoords.first) + static_cast<int>(m_Direction.y * 2) };
				const int nextCol{ static_cast<int>(cellCoords.second) + static_cast<int>(m_Direction.x * 2) };

				if (!pGridSys->IsWithinBounds(gridTag.gridId, static_cast<uint16_t>(nextRow), static_cast<uint16_t>(nextCol)))
				{
					push = false;
				}
				else
				{
					const auto& nextCellData = pGridSys->GetCellData(gridTag.gridId, static_cast<uint16_t>(nextRow), static_cast<uint16_t>(nextCol));
					PengoCellType nextType = static_cast<PengoCellType>(nextCellData.type);

					if (nextType != Pengo::PengoCellType::Walkable)
					{
						push = false;
					}
				}

				if (push)
				{
					blockComp.blockState = BlockComponent::BlockState::Moving;
					blockComp.dir = m_Direction;

					blockComp.pusherId = SceneManager::GetInstance().GetActiveScene()->GetPlayer(GetUserIndex()).GetEntityId();

					cellData.entity = NULL_ENTITY_ID;
					cellData.type = static_cast<uint8_t>(Pengo::PengoCellType::Walkable);

					block.GetComponent<BodyComponent>().collType = CollType::Dynamic;
					block.AddComponent<OnGridTag>(gridTag.gridId);
					
					return std::make_unique<PengoPushingBlockState>(GetUserIndex(), m_Direction);
				}
				else
				{
					EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
						cellData.entity,
						static_cast<uint8_t>(BlockSystem::BlockAnimations::Breaking)));

					blockComp.blockState = BlockComponent::BlockState::Breaking;

					return std::make_unique<PengoBreakingBlockState>(GetUserIndex(), m_Direction);
				}
			}
		}

		return nullptr;
	}
}

