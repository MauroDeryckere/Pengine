#ifndef PLAYERCOMPONENT
#define PLAYERCOMPONENT

#include "CoreIncludes.h"

namespace Pengin
{
	using UserIndex = UUID;
	struct PlayerComponent final
	{
		PlayerComponent(const UserIndex& userIdx, float movementSpeed = 0.f) :
			userIdx{ userIdx },
			movementSpeed{movementSpeed}
		{}

		PlayerComponent() = default;
		~PlayerComponent() = default;

		UserIndex userIdx{ true };
		float movementSpeed{ 0.f }; //Should probably be moved in future
	};
}

#endif