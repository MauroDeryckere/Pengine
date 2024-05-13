#include "PlayerSystem.h"

#include "SceneManager.h"
#include "Entity.h"

#include "SwitchAnimationEvent.h"
#include "EventManager.h"

#include "PlayerComponent.h"
#include "PengoComponent.h"
#include "BodyComponent.h"

namespace Pengo
{
	void PlayerSystem::Update()
	{
		using namespace Pengin;

		auto& pSceneManager = SceneManager::GetInstance();

		const auto& playerUUIDs = pSceneManager.GetActiveScene()->GetSceneData().playerUUIDs;

		for (const auto& player : playerUUIDs)
		{
			Entity playerEntity = pSceneManager.GetActiveScene()->GetEntity(player);
			
			if (playerEntity.HasComponent<PengoComponent>())
			{
				auto& pengoComp = playerEntity.GetComponent<PengoComponent>();
				assert(playerEntity.HasComponent<PlayerComponent>());

				pengoComp.Update(playerEntity.GetComponent<PlayerComponent>().userIdx);
			}
		}
	}

	void PlayerSystem::OnBlockBreakEvent(const BaseEvent& event)
	{
		const auto& breakEv{ static_cast<const PengoBlockBreakEvent&>(event) };

		const auto& userIdx = breakEv.GetUserInex();

		auto& sceneData = SceneManager::GetInstance().GetActiveScene()->GetSceneData();

		auto it = sceneData.user_UUIDVecIdxMap.find(userIdx);

		if (it != sceneData.user_UUIDVecIdxMap.end())
		{
			auto entity = SceneManager::GetInstance().GetActiveScene()->GetEntity(sceneData.playerUUIDs[it->second]);

			assert(entity.HasComponent<PengoComponent>());
			entity.GetComponent<PengoComponent>().SetPlayerState(std::make_unique<PengoIdleState>(userIdx));
		}
	}
}

