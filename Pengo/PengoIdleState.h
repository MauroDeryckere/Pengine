#ifndef PENGOIDLESTATE
#define PENGOIDLESTATE

#include "PlayerState.h"

#include "EventManager.h"
#include "SwitchAnimationEvent.h"

#include "DebugOutput.h"
#include "EntityId.h"
#include "SceneManager.h"
#include "PlayerSystem.h"
#include "Entity.h"

#include <string>
#include <memory>

namespace Pengo
{
	using namespace Pengin;

	class PengoIdleState final : public PlayerState
	{
	public:
		PengoIdleState(const UserIndex& userIdx) :
			PlayerState{ userIdx }
		{}

		void OnEnter()
		{
			auto& sceneData = SceneManager::GetInstance().GetActiveScene()->GetSceneData();

			auto it = sceneData.user_UUIDVecIdxMap.find(PlayerState::GetUserIndex());

			if (it != sceneData.user_UUIDVecIdxMap.end())
			{
				auto entity = SceneManager::GetInstance().GetActiveScene()->GetEntity(sceneData.playerUUIDs[it->second]);
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(entity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::Idle)));
			}

			DEBUG_OUT("Enter Idle");
		}

		std::unique_ptr<PlayerState> HandleInput(const UserIndex& userIndex, const std::string& actionName);

		void OnExit()
		{
			DEBUG_OUT("Exit Idle");
		}

	};
}

#endif