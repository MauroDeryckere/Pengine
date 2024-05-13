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
	class PengoBreakingBlockState final : public Pengin::PlayerState
	{
	public:
		PengoBreakingBlockState(const Pengin::UserIndex& userIdx) :
			Pengin::PlayerState{ userIdx }
		{ }

		void OnEnter()
		{
			auto& sceneData = Pengin:: SceneManager::GetInstance().GetActiveScene()->GetSceneData();

			auto it = sceneData.user_UUIDVecIdxMap.find(GetUserIndex());

			if (it != sceneData.user_UUIDVecIdxMap.end())
			{
				//TODO depending on curr animation Idx, we select the correct push animation

				auto entity = Pengin::SceneManager::GetInstance().GetActiveScene()->GetEntity(sceneData.playerUUIDs[it->second]);
				Pengin::EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<Pengin::SwitchAnimationEvent>(entity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushLeft)));
			}

			DEBUG_OUT("Enter Breaking Block");
		}

		std::unique_ptr<Pengin::PlayerState> Update(const Pengin::UserIndex& userIndex);

		std::unique_ptr<Pengin::PlayerState> HandleInput(const Pengin::UserIndex&, const std::string&)
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