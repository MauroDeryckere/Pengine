#ifndef SCENEDATA
#define SCENEDATA

#include "CoreIncludes.h"

namespace Pengin
{
	struct SceneData final
	{
		std::string name{ "No Scene Name" };
		
		std::vector<UUID> playerUUIDs;
		std::unordered_map<size_t, size_t> user_UUIDVecIdxMap;

		bool isUUIDInit{ false };
	};
}

#endif