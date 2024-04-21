#ifndef SCORECOMPONENT
#define SCORECOMPONENT

#include "CoreIncludes.h"
#include <vector>

namespace Pengin
{
	struct ScoreComponent final
	{
	public:
		ScoreComponent(const std::vector<EntityId>& scoreDisplayIds = { }) :
			score{ 0 },
			scoreDisplays{ scoreDisplayIds }
		{
			scoreDisplays.shrink_to_fit();
		}

		~ScoreComponent() = default;

		unsigned score{};
		std::vector<EntityId> scoreDisplays{};
	};
}

#endif
