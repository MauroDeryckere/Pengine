#pragma once

#include "EntityId.h"

#include <vector>

namespace GS
{
	struct MinerComponent final
	{
		std::vector<unsigned> ores{ };

		int baseWeight{ -10 };
		int totOreWeight{ }; //total weight the miner has (this simple effects how much miner y velocity is effected), 0 == float

		Pengin::EntityId oreDisplayId{ Pengin::NULL_ENTITY_ID };

		void DropOre()
		{
			if (!ores.empty())
			{
				totOreWeight -= ores.back();
				ores.pop_back();
			}
		}
	};
}