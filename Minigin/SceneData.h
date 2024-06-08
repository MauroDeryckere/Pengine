#ifndef SCENEDATA
#define SCENEDATA

#include "CoreIncludes.h"
#include "SceneFileData.h"
#include "PlayerComponent.h"

namespace Pengin
{
	using UserIndex = GameUUID;

	class Scene;

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

		void SetPlayerUUID(const UserIndex& userIdx, GameUUID playerUUID) noexcept;

		bool RemovePlayer(const UserIndex& userIdx, Scene* pScene) noexcept;
	};
}

#endif