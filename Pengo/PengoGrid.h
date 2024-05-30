#ifndef PENGO_GRID
#define PENGO_GRID

#include <stdint.h>

namespace Pengo
{
	enum class PengoCellType : uint8_t
	{
		Walkable = 0,
		Wall = 1
	};

	enum class BlockType : uint8_t
	{
		Idle = 0, 
		Breaking = 1
	};
}

#endif