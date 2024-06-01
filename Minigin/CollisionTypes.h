#ifndef PENGIN_COLLISIONTYPES
#define PENGIN_COLLISIONTYPES

#include <stdint.h>

namespace Pengin
{
	enum class CollType : uint8_t
	{
		Dynamic = 0,
		Static = 1,
		Trigger = 2
	};
}

#endif