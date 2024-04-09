#ifndef UUIDCOMPONENT
#define UUIDCOMPONENT

#include "CoreIncludes.h"

namespace Pengin
{
	struct UUIDComponent final
	{
		UUIDComponent(const UUID& uuid = UUID{ }) :
			uuid{ uuid }
		{ }

		UUIDComponent(const UUIDComponent& uuid) = default;

		~UUIDComponent() = default;

		UUID uuid;
	};
}

#endif