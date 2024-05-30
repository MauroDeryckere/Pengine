#include "PengoBreakingBlockState.h"
#include "GameTime.h"

#include "EventManager.h"
#include "PengoComponent.h"
#include "PengoBlockBreakEvent.h"
#include "PengoIdleState.h"
#include "GridSystem.h"
#include "OnGridTag.h"
#include "PengoGrid.h"

namespace Pengo
{
	void PengoBreakingBlockState::OnEnter()
	{		
		using namespace Pengin;

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

		static float timer { 0.0f };
        timer += GameTime::GetInstance().ElapsedSec();

        if (timer >= 1.0f)
        {
            timer = 0.0f;
            
			auto activeScene = SceneManager::GetInstance().GetActiveScene();
			auto player = activeScene->GetPlayer(GetUserIndex());

			auto pGridSys = activeScene->GetSystem<GridSystem>();
			const auto& coords = pGridSys->GetCellCoords(player.GetEntityId());

			OnGridTag onGridTag{ player.GetComponent<OnGridTag>() };
			auto& cellData = pGridSys->GetCellData(onGridTag.gridId, static_cast<uint16_t>(coords.first + m_Direction.y), 
																	 static_cast<uint16_t>(coords.second + m_Direction.x));

			cellData.type = static_cast<uint8_t>(PengoCellType::Walkable);

			EventManager::GetInstance().BroadcoastEvent(std::make_unique<PengoBlockBreakEvent>(PlayerState::GetUserIndex(), cellData.entity));
			return std::make_unique<PengoIdleState>(GetUserIndex(), m_Direction);
		}
		
		return nullptr;
	}
}
