#include "PengoBreakingBlockState.h"
#include "GameTime.h"

#include "EventManager.h"
#include "PengoComponent.h"
#include "PengoBlockBreakEvent.h"
#include "PengoIdleState.h"
#include "GridSystem.h"
#include "OnGridTag.h"
#include "PengoGrid.h"
#include "BlockSystem.h"
#include "BlockComponent.h"
#include "SnobeeComponent.h"
#include "DeathEvent.h"

namespace Pengo
{
	void PengoBreakingBlockState::OnEnter()
	{		
		using namespace Pengin;

		ServiceLocator::GetSoundSystem().PlaySound({ "../Data/Audio/Ice Block Destroyed.mp3" });

		auto playerEntity{ Pengin::SceneManager::GetInstance().GetActiveScene()->GetPlayer(Pengin::PlayerState::GetUserIndex())};

		if (playerEntity)
		{
			if (m_Direction.x > 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushRight)));
			}
			else if (m_Direction.x < 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushLeft)));
			}
			else if (m_Direction.y > 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushDown)));
			}
			else if (m_Direction.y < 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushUp)));
			}
		}
	}

	std::unique_ptr<Pengin::PlayerState> PengoBreakingBlockState::Update(const Pengin::UserIndex&)
	{
        using namespace Pengin;

        m_Timer += GameTime::GetInstance().ElapsedSec();

        if (m_Timer >= 1.0f)
        {            
			auto activeScene = SceneManager::GetInstance().GetActiveScene();
			auto player = activeScene->GetPlayer(GetUserIndex());

			auto pGridSys = activeScene->GetSystem<GridSystem>();
			const auto& coords = pGridSys->GetCellCoords(player.GetEntityId());

			OnGridTag onGridTag{ player.GetComponent<OnGridTag>() };
			auto& cellData = pGridSys->GetCellData(onGridTag.gridId, static_cast<uint16_t>(coords.first + m_Direction.y), 
																	 static_cast<uint16_t>(coords.second + m_Direction.x));

			cellData.entity = NULL_ENTITY_ID;
			
			if (cellData.type == static_cast<uint8_t>(PengoCellType::Block))
			{
				cellData.type = static_cast<uint8_t>(PengoCellType::Walkable);
			}

			return std::make_unique<PengoIdleState>(GetUserIndex(), m_Direction);
		}
		
		return nullptr;
	}

	void PengoBreakingBlockState::OnCollision(const Pengin::BaseEvent& event)
	{
		using namespace Pengin;
		const auto& collEvent{ static_cast<const CollisionEvent&>(event) };

		auto activeScene = SceneManager::GetInstance().GetActiveScene();
		auto player = activeScene->GetPlayer(GetUserIndex());

		const auto entityA{ Pengin::Entity{ collEvent.GetEntityA(), activeScene} };
		const auto entityB{ Pengin::Entity{ collEvent.GetEntityB(), activeScene} };
		
		if (player.GetEntityId() == entityA.GetEntityId() 
			&& entityA.HasComponent<PengoComponent>() && entityB.HasComponent<SnobeeComponent>())
		{
			EventManager::GetInstance().BroadcoastEvent(std::make_unique<DeathEvent>("PengoDeath", collEvent.GetEntityA()));
			ResetBlock();
		}
		else if (player.GetEntityId() == entityB.GetEntityId() 
			&& entityB.HasComponent<PengoComponent>() && entityA.HasComponent<SnobeeComponent>())
		{
			EventManager::GetInstance().BroadcoastEvent(std::make_unique<DeathEvent>("PengoDeath", collEvent.GetEntityB()));
			ResetBlock();
		}
	}

	void PengoBreakingBlockState::ResetBlock()
	{
		using namespace Pengin;
		auto activeScene = SceneManager::GetInstance().GetActiveScene();
		auto player = activeScene->GetPlayer(GetUserIndex());

		auto pGridSys = activeScene->GetSystem<GridSystem>();
		const auto& coords = pGridSys->GetCellCoords(player.GetEntityId());

		OnGridTag onGridTag{ player.GetComponent<OnGridTag>() };
		auto& cellData = pGridSys->GetCellData(onGridTag.gridId, static_cast<uint16_t>(coords.first + m_Direction.y),
			static_cast<uint16_t>(coords.second + m_Direction.x));

		if (cellData.type == static_cast<uint8_t>(PengoCellType::Block))
		{
			Entity block{ cellData.entity, activeScene };
			auto& blockComp = block.GetComponent<BlockComponent>();

			blockComp.blockState = BlockComponent::BlockState::Still;

			EventManager::GetInstance().BroadcoastEvent(std::make_unique<SwitchAnimationEvent>(cellData.entity, static_cast<uint8_t>(BlockSystem::BlockAnimations::Still)));
		}
	}
}
