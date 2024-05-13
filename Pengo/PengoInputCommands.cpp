#include "PengoInputCommands.h"

void Pengo::Movement::Execute()
{
	using namespace Pengin;

	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();

	auto it = pActiveScene->GetSceneData().user_UUIDVecIdxMap.find(GetUserIdx());

	if (it == end(pActiveScene->GetSceneData().user_UUIDVecIdxMap))
	{
		DEBUG_OUT("movement for a deleted playerIdx");
		return;
	}
	const auto& playerUUID = pActiveScene->GetSceneData().playerUUIDs[it->second];

	const EntityId entityId = pActiveScene->GetEntityId(playerUUID);
	Entity playerEntity{ entityId, pActiveScene.get() };

	if (playerEntity.HasComponent<PengoComponent>())
	{
		playerEntity.GetComponent<PengoComponent>().HandleInput(GetUserIdx(), GetActionName());

		const auto movementSpeed = playerEntity.GetComponent<PlayerComponent>().movementSpeed;
		playerEntity.GetComponent<BodyComponent>().inputVelocity += (m_Direction * movementSpeed);
	}
}

void Pengo::BreakBlock::Execute()
{
	using namespace Pengin;

	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();

	auto it = pActiveScene->GetSceneData().user_UUIDVecIdxMap.find(GetUserIdx());

	if (it == end(pActiveScene->GetSceneData().user_UUIDVecIdxMap))
	{
		DEBUG_OUT("movement for a deleted playerIdx");
		return;
	}
	const auto& playerUUID = pActiveScene->GetSceneData().playerUUIDs[it->second];

	const EntityId entityId = pActiveScene->GetEntityId(playerUUID);
	Entity playerEntity{ entityId, pActiveScene.get() };

	if (playerEntity.HasComponent<PengoComponent>())
	{
		playerEntity.GetComponent<PengoComponent>().HandleInput(GetUserIdx(), GetActionName());
	}
}
