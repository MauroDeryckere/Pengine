#ifndef PENGODYINGSTATE
#define PENGODYINGSTATE

#include "PlayerState.h"

#include "DebugOutput.h"

#include "PlayerSystem.h"
#include "EventManager.h"
#include "SwitchAnimationEvent.h"
#include "SceneManager.h"
#include "Entity.h"

#include <string>

namespace Pengo
{
	using namespace Pengin;

	class PengoDyingState final : public PlayerState
	{
	public:
		PengoDyingState(const UserIndex& userIdx) :
			PlayerState{ userIdx }
		{}

		void OnEnter()
		{
			auto& sceneData = SceneManager::GetInstance().GetActiveScene()->GetSceneData();

			auto it = sceneData.user_UUIDVecIdxMap.find(PlayerState::GetUserIndex());

			if (it != sceneData.user_UUIDVecIdxMap.end())
			{
				auto entity = SceneManager::GetInstance().GetActiveScene()->GetEntity(sceneData.playerUUIDs[it->second]);
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(entity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::Dying)));
			}

			DEBUG_OUT("Enter Dying");
		}

		std::unique_ptr<PlayerState> Update(const UserIndex& userIndex);

		std::unique_ptr<PlayerState> HandleInput(const UserIndex&, const std::string&)
		{
			return nullptr;
		}

		void OnExit()
		{
			DEBUG_OUT("Exit Dying");
		}

	};
}

#endif