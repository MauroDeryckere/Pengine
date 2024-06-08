#ifndef SCENEDATA
#define SCENEDATA

#include "CoreIncludes.h"
#include "SceneFileData.h"

namespace Pengin
{
	using UserIndex = GameUUID;

	struct SceneData final
	{
		std::string name{ "No Scene Name" };
		
		std::vector<GameUUID> playerUUIDs{};

		std::unordered_map<UserIndex, size_t> userUUID_VecIdxMap{};
		std::unordered_map<UserIndex, size_t> vecIdx_userUUIDMap{};

		SceneFileData sceneFileData{};


		const GameUUID& GetPlayerUUID(const UserIndex& userIdx) const noexcept
		{
			auto it{ userUUID_VecIdxMap.find(userIdx) };
			assert( it != end(userUUID_VecIdxMap) && "userIdx not found");

			if ( it != end(userUUID_VecIdxMap) )
			{
				return playerUUIDs[it->second];
			}

			return GameUUID::INVALID_UUID;
		}

		void SetPlayerUUID(const UserIndex& userIdx, GameUUID playerUUID) noexcept
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

			}
		}

		bool RemovePlayer(const UserIndex& userIdx) noexcept
		{
			auto it = userUUID_VecIdxMap.find(userIdx);
			if (it == end(userUUID_VecIdxMap))
			{
				return false;
			}

			const auto& backUUID = playerUUIDs.back();
			const auto vecIdx = it->second;

			std::swap(playerUUIDs[vecIdx], playerUUIDs.back());
			playerUUIDs.pop_back();

			userUUID_VecIdxMap[backUUID] = vecIdx;
			userUUID_VecIdxMap.erase(it);

			vecIdx_userUUIDMap[vecIdx] = backUUID;
			vecIdx_userUUIDMap.erase(playerUUIDs.size());

			return true;
		}
	};
}

#endif