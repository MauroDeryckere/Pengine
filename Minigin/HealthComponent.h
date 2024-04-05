#ifndef HEALTHCOMPONENT
#define HEALTHCOMPONENT

#include "EntityId.h"

#include <string> //TODo remove
#include <vector>

namespace Pengin
{
	struct HealthComponent final
	{
	public:
		HealthComponent(unsigned health, const std::vector<EntityId> healthbarIds = { }) :
			m_Health{ health },
			m_HealthDisplayIds{ healthbarIds }
		{
			m_HealthDisplayIds.shrink_to_fit();
		}

		~HealthComponent() = default; 

		unsigned m_Health;
		std::vector<EntityId> m_HealthDisplayIds;
	};
}

#endif
