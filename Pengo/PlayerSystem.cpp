#include "PlayerSystem.h"

#include "SceneManager.h"
#include "Entity.h"

#include "SwitchAnimationEvent.h"
#include "EventManager.h"
#include "PengoRespawnEvent.h"

#include "PlayerComponent.h"
#include "PengoComponent.h"
#include "BodyComponent.h"
#include "ScoreComponent.h"

#include "PengoIdleState.h"
#include "Gridsystem.h"
#include "OnGridTag.h"
#include "HealthComponent.h"
#include "PengoDyingState.h"
#include "ScoreCollectEvent.h"
#include "SnobeeDeathEvent.h"

#include "DeathEvent.h"

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

		const auto& deahtEv{ static_cast<const DeathEvent&>(event) };

		assert(m_ECS.HasComponent<PlayerComponent>(deahtEv.GetEntityId()));
		assert(m_ECS.HasComponent<PengoComponent>(deahtEv.GetEntityId()));
		assert(m_ECS.HasComponent<HealthComponent>(deahtEv.GetEntityId()));

		m_ECS.GetComponent<HealthComponent>(deahtEv.GetEntityId()).health--;
		m_ECS.GetComponent<PengoComponent>(deahtEv.GetEntityId()).SetPlayerState(std::make_unique<PengoDyingState>(m_ECS.GetComponent<PlayerComponent>(deahtEv.GetEntityId()).userIdx));
	}

	void PlayerSystem::OnRespawnEvent(const Pengin::BaseEvent& event)
	{
		using namespace Pengin;

		const auto& respawnEv{ static_cast<const PengoRespawnEvent&>(event) };

		const auto& userIdx = respawnEv.GetUserIndex();

		auto player = SceneManager::GetInstance().GetActiveScene()->GetPlayer(userIdx);

		if (player)
		{
			assert(player.HasComponent<PengoComponent>());
			
			GridSystem* pGridSys = SceneManager::GetInstance().GetActiveScene()->GetSystem<GridSystem>();
			assert(pGridSys);

			assert(player.HasComponent<OnGridTag>());
			const auto& gridTag{ player.GetComponent<OnGridTag>() };

			const auto pos = pGridSys->GetCellPos(gridTag.gridId, 0, 0);

			player.SetLocalPosition({ pos.x, pos.y, 0.f });
			player.SetWorldPosition({ pos.x, pos.y, 0.f });

			player.GetComponent<PengoComponent>().SetPlayerState(std::make_unique<PengoIdleState>(userIdx, glm::vec2{0, 1}));
		}
	}

	void PlayerSystem::OnSnobeeDeathEvent(const Pengin::BaseEvent& event)
	{
		using namespace Pengin;
		EventManager::GetInstance().BroadcoastEvent(std::make_unique<ScoreCollectEvent>(400, static_cast<const SnobeeDeathEvent&>(event).GetKillerId()));
	}
}

