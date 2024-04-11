#ifndef PLAYERCOMPONENT
#define PLAYERCOMPONENT

namespace Pengin
{
	struct PlayerComponent final
	{
		PlayerComponent(size_t userIdx) :
			userIdx{ userIdx }
		{}

		PlayerComponent() = default;

		size_t userIdx{ SIZE_MAX };
	};
}

#endif