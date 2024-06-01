#ifndef PLAYERCOMPONENT
#define PLAYERCOMPONENT

#include "GameUUID.h"

namespace Pengin
{
	using UserIndex = GameUUID;

	struct PlayerComponent final
	{
		UserIndex userIdx{ true };

		PlayerComponent(const UserIndex& userIdx) :
			userIdx{ userIdx }		
		{}

		PlayerComponent() = default;
		~PlayerComponent() = default;
	};
}

#endif