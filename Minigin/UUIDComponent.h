#ifndef UUIDCOMPONENT
#define UUIDCOMPONENT

#include "CoreIncludes.h"

namespace Pengin
{
	struct UUIDComponent final
	{
		UUIDComponent(const GameUUID& uuid = GameUUID{ }) :
			uuid{ uuid }
		{ }

		UUIDComponent(const UUIDComponent& uuid) = default;

		~UUIDComponent() = default;

		GameUUID uuid;
	};
}

#endif