#include "PlayerSystem.h"

#include "PengoWalkState.h"

#include "PengoIdleState.h"
#include "PengoBreakingBlockState.h"

#include "EventManager.h"
#include "SwitchAnimationEvent.h"

#include "SceneManager.h"
#include "BodyComponent.h"
#include "Entity.h"

#include <cassert>

namespace Pengo
{
	std::unique_ptr<PlayerState> PengoWalkState::Update(const UserIndex& userIndex)
	{
		auto& sceneData = SceneManager::GetInstance().GetActiveScene()->GetSceneData();

		auto it = sceneData.user_UUIDVecIdxMap.find(userIndex);
	
		if (it != sceneData.user_UUIDVecIdxMap.end()) 
		{
			auto entity = SceneManager::GetInstance().GetActiveScene()->GetEntity(sceneData.playerUUIDs[it->second]);

			const auto& body = entity.GetComponent<BodyComponent>();

			if (body.velocity.x == 0.f && body.velocity.y == 0.f)
			{
				return std::move(std::make_unique<PengoIdleState>(userIndex));
			}
			else
			{
				if (body.velocity.x > 0.f)
				{
					EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(entity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::MoveRight)));
				}
				else if (body.velocity.x < 0.f)
				{
					EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(entity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::MoveLeft)));
				}
				else if (body.velocity.y > 0.f)
				{
					EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(entity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::MoveDown)));
				}
				else if (body.velocity.y < 0.f)
				{
					EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(entity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::MoveUp)));
				}
			}
		}
		
		return nullptr;
	}

	std::unique_ptr<PlayerState> PengoWalkState::HandleInput(const UserIndex& userIndex, const std::string& actionName)
	{
		if (actionName == "PengoBreakBlock")
		{
			return std::move(std::make_unique<PengoBreakingBlockState>(userIndex));
		}

		return nullptr;
	}
}

