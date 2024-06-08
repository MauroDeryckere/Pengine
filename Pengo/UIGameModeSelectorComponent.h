#ifndef PENGO_UILGMSELECTOR
#define PENGO_UILGMSELECTOR

#include <stdint.h>
#include <cassert>
#include "EntityId.h"

namespace Pengo
{
	struct UIGameModeSelectorComponent final
	{
		UIGameModeSelectorComponent() = default;
		~UIGameModeSelectorComponent() = default;

		void ChangeGameMode(bool up)
		{
			assert(gamemodeIds.size() > 1);
			assert(gamemodeIds.size() <= UINT8_MAX);

			if (up)
			{
				if (currGameMode == 0)
				{
					currGameMode = static_cast<uint8_t>(gamemodeIds.size() - 1);
					return;
				}

				--currGameMode;
			}
			else
			{
				if (currGameMode == static_cast<uint8_t>(gamemodeIds.size() - 1))
				{
					currGameMode = 0;
					return;
				}

				++currGameMode;
			}
		}

		std::uint8_t currGameMode{0};
		std::vector<Pengin::EntityId> gamemodeIds{};
	};
}

#endif