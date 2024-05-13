#ifndef PENGOBREAKINGBLOCKSTATE
#define PENGOBREAKINGBLOCKSTATE

#include "PlayerState.h"
#include "DebugOutput.h"

#include "EventManager.h"
#include "SwitchAnimationEvent.h"
#include "Entity.h"

#include "SceneManager.h"

#include "PlayerSystem.h"

#include <string>

namespace Pengo
{
	using namespace Pengin;

	class PengoBreakingBlockState final : public PlayerState
	{
	public:
		PengoBreakingBlockState(const UserIndex& userIdx) :
			PlayerState{ userIdx }
		{ }

		void OnEnter()
		{
			auto& sceneData = SceneManager::GetInstance().GetActiveScene()->GetSceneData();

			auto it = sceneData.user_UUIDVecIdxMap.find(PlayerState::GetUserIndex());

			if (it != sceneData.user_UUIDVecIdxMap.end())
			{
				//TODO depending on curr animation Idx, we select the correct push animation

				auto entity = SceneManager::GetInstance().GetActiveScene()->GetEntity(sceneData.playerUUIDs[it->second]);
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(entity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushLeft)));
			}

			DEBUG_OUT("Enter Breaking Block");
		}

		std::unique_ptr<PlayerState> Update(const UserIndex& userIndex);

		std::unique_ptr<PlayerState> HandleInput(const UserIndex&, const std::string&)
		{
			return nullptr;
		}

		void OnExit()
		{
			DEBUG_OUT("Exit Breaking Block");
		}

	private:

	};
}

#endif