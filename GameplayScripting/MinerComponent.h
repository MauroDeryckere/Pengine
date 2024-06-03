#pragma once

#include <vector>

namespace GS
{
	struct MinerComponent final
	{
		std::vector<unsigned> ores{ };

		int totalWeight{ }; //total weight the miner has (this simple effects how much miner y velocity is effected), 0 == float

		void DropOre()
		{
			if (!ores.empty())
			{
				totalWeight -= ores.back();
				ores.pop_back();
			}
		}
	};
}