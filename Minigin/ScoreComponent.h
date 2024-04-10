#ifndef SCORECOMPONENT
#define SCORECOMPONENT

#include "CoreIncludes.h"
#include <vector>

namespace Pengin
{
	struct ScoreComponent final
	{
	public:
		ScoreComponent(const std::vector<UUID>& scoreDisplayIds = { }) :
			m_Score{ 0 },
			m_ScoreDisplays{ scoreDisplayIds }
		{
			m_ScoreDisplays.shrink_to_fit();
		}

		~ScoreComponent() = default;

		unsigned m_Score{};
		std::vector<UUID> m_ScoreDisplays{};
	};
}

#endif
