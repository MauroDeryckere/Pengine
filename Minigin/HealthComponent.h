#ifndef HEALTHCOMPONENT
#define HEALTHCOMPONENT

#include "CoreIncludes.h"
#include <vector>

namespace Pengin
{
	struct HealthComponent final
	{
	public:
		HealthComponent(unsigned health = 0, const std::vector<UUID>& healthbarIds = { }) :
			m_Health{ health },
			m_HealthDisplayIds{ healthbarIds }
		{
			m_HealthDisplayIds.shrink_to_fit();
		}

		~HealthComponent() = default; 

		unsigned m_Health;
		std::vector<UUID> m_HealthDisplayIds;
	};
}

#endif
