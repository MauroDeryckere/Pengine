#ifndef PENGO_UISELECTOR
#define PENGO_UISELECTOR

#include "EntityId.h"

namespace Pengo
{
	struct UISelectorComponent final
	{
		UISelectorComponent() = default;
		~UISelectorComponent() = default;

		void ScrollIdx(bool right) noexcept
		{
			if (right)
			{
				++currHorIdx;
				if (currHorIdx > 2)
				{
					currHorIdx = 0;
				}
			}
			else
			{
				--currHorIdx;
				if (currHorIdx < 0)
				{
					currHorIdx = 2;
				}
			}
		}

		int currHorIdx = 0;
		std::vector<Pengin::EntityId> letters;

		unsigned score;
	};
}

#endif