#pragma once

#include <typeindex>
#include "Entity.h"

namespace Pengin
{
	class EcsEventInter final
	{
	public:
		void SetObserverDirty(EntityId entityId, std::type_index typeIdx) const noexcept;
	};
}


