#ifndef PENGO_GRID
#define PENGO_GRID

#include <stdint.h>

namespace Pengo
{
	enum class PengoCellType : uint8_t
	{
		Walkable = 0,
		Block = 1
	};
}

#endif