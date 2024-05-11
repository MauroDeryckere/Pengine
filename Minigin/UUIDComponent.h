#ifndef UUIDCOMPONENT
#define UUIDCOMPONENT

#include "CoreIncludes.h"

namespace Pengin
{
	struct UUIDComponent final
	{
		GameUUID uuid;

		UUIDComponent(const GameUUID& uuid = GameUUID{ }) :
			uuid{ uuid }
		{ }

		UUIDComponent(const UUIDComponent& uuid) = default;

		~UUIDComponent() = default;
	};
}

#endif