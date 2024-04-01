#ifndef ECSEVENTINTER
#define ECSEVENTINTER

#include <typeindex>
#include "EntityId.h"

namespace Pengin
{
	class EcsEventInter final
	{
	public:
		void SetObserverDirty(const EntityId entityId, std::type_index typeIdx) const noexcept;
	};
}

#endif