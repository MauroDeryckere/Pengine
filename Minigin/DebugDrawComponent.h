#ifndef DEBUGDRAWCOMPONENT
#define DEBUGDRAWCOMPONENT

#include "UtilStructs.h"
#include "glm/vec4.hpp"

namespace Pengin
{
	struct DebugDrawComponent final
	{
		glm::u8vec4 color;

		uint16_t width;
		uint16_t height;

		bool fill{ false };

		DebugDrawComponent() = default;
		DebugDrawComponent(const glm::u8vec4& color, uint16_t width, uint16_t height, bool fill = false) :
			color{ color },

			width{ width },
			height{ height },

			fill{ fill }
		{}

	};
}

#endif 
