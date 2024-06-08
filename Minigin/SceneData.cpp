#include "SceneData.h"

#include "Entity.h"
#include "Scene.h"
#include "PlayerComponent.h"

void Pengin::SceneData::SetPlayerUUID(const UserIndex& userIdx, GameUUID playerUUID) noexcept
{
	auto it = userUUID_VecIdxMap.find(userIdx);

	if (it == end(userUUID_VecIdxMap))
	{
		playerUUIDs.emplace_back(playerUUID);
		userUUID_VecIdxMap[userIdx] = playerUUIDs.size() - 1;
		vecIdx_userUUIDMap[playerUUIDs.size() - 1] = userIdx;
	}
	else
	{
		DEBUG_OUT("Adding a player UUID for a userIdx that was already added, replacing it with given UUID");
		playerUUIDs[(*it).second] = playerUUID;
		vecIdx_userUUIDMap[it->second] = userIdx;
		userUUID_VecIdxMap[userIdx] = it->second;
	}
}

bool Pengin::SceneData::RemovePlayer(const UserIndex& userIdx, Scene* pScene) noexcept
{
	auto it = userUUID_VecIdxMap.find(userIdx);
	if (it == end(userUUID_VecIdxMap))
	{
		return false;
	}

	const auto vecIdx = it->second;
	const auto& backUUID = playerUUIDs.back();

	auto backEnt = pScene->GetEntity(backUUID);
	const auto& backUIdx = backEnt.GetComponent<PlayerComponent>().userIdx;

	if (vecIdx != playerUUIDs.size() - 1)
	{
		std::swap(playerUUIDs[vecIdx], playerUUIDs.back());

		userUUID_VecIdxMap[backUIdx] = vecIdx;
		vecIdx_userUUIDMap[vecIdx] = backUIdx;
	}

	userUUID_VecIdxMap.erase(it);
	vecIdx_userUUIDMap.erase(playerUUIDs.size() - 1);

	playerUUIDs.pop_back();

	return true;
}
