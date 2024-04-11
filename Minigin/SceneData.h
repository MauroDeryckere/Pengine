#ifndef SCENEDATA
#define SCENEDATA

#include "CoreIncludes.h"

namespace Pengin
{
	struct SceneData final
	{
		std::string name{ "No Scene Name" };
		
		std::vector<UUID> playerUUIDs;
		std::vector<size_t> userIdx;

		bool isUUIDInit{ false };
	};
}

#endif