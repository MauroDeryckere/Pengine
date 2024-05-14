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
		std::unordered_map<UserIndex, size_t> user_UUIDVecIdxMap{};

		SceneFileData sceneFileData{};


		const GameUUID& GetPlayerUUID(const UserIndex& userIdx) const noexcept
		{
			auto it{ user_UUIDVecIdxMap.find(userIdx) };
			assert( it != end(user_UUIDVecIdxMap) && "userIdx not found");

			if ( it != end(user_UUIDVecIdxMap) )
			{
				return playerUUIDs[it->second];
			}

			return GameUUID::INVALID_UUID;
		}

		void SetPlayerUUID(const UserIndex& userIdx, GameUUID playerUUID) noexcept
		{
			auto it = user_UUIDVecIdxMap.find(userIdx);

			if (it == end(user_UUIDVecIdxMap))
			{
				playerUUIDs.emplace_back(playerUUID);
				user_UUIDVecIdxMap[userIdx] = playerUUIDs.size() - 1;
			}
			else
			{
				DEBUG_OUT("Adding a player UUID for a userIdx that was already added, replacing it with given UUID");
				playerUUIDs[(*it).second] = playerUUID;
			}
		}
	};
}

#endif