#ifndef SCENEDATA
#define SCENEDATA

#include "CoreIncludes.h"
#include "SceneFileData.h"

namespace Pengin
{
	using UserIndex = UUID;

	struct SceneData final
	{
		std::string name{ "No Scene Name" };
		
		std::vector<UUID> playerUUIDs{};
		std::unordered_map<UserIndex, size_t> user_UUIDVecIdxMap{};

		SceneFileData sceneFileData{};

		bool isUUIDInit{ false };

		//Functions
		void SetPlayer(const UserIndex& userIdx, UUID playerUUID) noexcept
		{
			isUUIDInit = true;

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