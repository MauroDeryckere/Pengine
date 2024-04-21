#ifndef HEALTHCOMPONENT
#define HEALTHCOMPONENT

#include "CoreIncludes.h"
#include <vector>

namespace Pengin
{
	struct HealthComponent final
	{
	public:
		HealthComponent(unsigned health = 0, const std::vector<EntityId>& healthbarIds = { }) :
			health{ health },
			healthDisplayIds{ healthbarIds }
		{
			healthDisplayIds.shrink_to_fit();
		}

		~HealthComponent() = default; 

		unsigned health;
		std::vector<EntityId> healthDisplayIds;
	};
}

#endif
