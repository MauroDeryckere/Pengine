#include "PlayerSystem.h"

#include "SceneManager.h"
#include "Entity.h"

#include "SwitchAnimationEvent.h"
#include "EventManager.h"
#include "PengoRespawnEvent.h"

#include "PlayerComponent.h"
#include "PengoComponent.h"
#include "BodyComponent.h"

#include "PengoIdleState.h"
#include "PengoDyingState.h"

#include "HealthChangeEvent.h"

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
				const auto& playerComp{ playerEntity.GetComponent<PlayerComponent>() };

				pengoComp.HandleInput(playerComp.userIdx);
				pengoComp.Update(playerComp.userIdx);
			}
		}
	}

	void PlayerSystem::OnDeathEvent(const Pengin:: BaseEvent& event)
	{
		using namespace Pengin;

		const auto& deahtEv{ static_cast<const HealthChangeEvent&>(event) };

		assert(m_ECS.HasComponent<PlayerComponent>(deahtEv.GetEntityId()));
		assert(m_ECS.HasComponent<PengoComponent>(deahtEv.GetEntityId()));

		m_ECS.GetComponent<PengoComponent>(deahtEv.GetEntityId()).SetPlayerState(std::make_unique<PengoDyingState>(m_ECS.GetComponent<PlayerComponent>(deahtEv.GetEntityId()).userIdx));
	}

	void PlayerSystem::OnRespawnEvent(const Pengin::BaseEvent& event)
	{
		using namespace Pengin;

		const auto& respawnEv{ static_cast<const PengoRespawnEvent&>(event) };

		const auto& userIdx = respawnEv.GetUserInex();

		auto player = SceneManager::GetInstance().GetActiveScene()->GetPlayer(userIdx);

		if (player)
		{
			assert(player.HasComponent<PengoComponent>());
			player.GetComponent<PengoComponent>().SetPlayerState(std::make_unique<PengoIdleState>(userIdx, glm::vec2{0, 1}));
		}
	}

	void PlayerSystem::OnBlockBreakEvent(const Pengin::BaseEvent& event)
	{
		using namespace Pengin;

		const auto& breakEv{ static_cast<const PengoBlockBreakEvent&>(event) };
		//const auto& userIdx = breakEv.GetUserIndex();

		m_ECS.DestroyEntity(breakEv.GetBlockId());
	}
}

