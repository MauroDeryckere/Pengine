#ifndef PENGO_UIGAMEMODE
#define PENGO_UIGAMEMODE

#include <string>

namespace Pengo
{
	struct UIGameModeComponent final
	{
		UIGameModeComponent() = default;
		UIGameModeComponent(const std::string& gm) :
			gameMode{ gm }
		{}

		~UIGameModeComponent() = default;

		std::string gameMode{ "NO STRING" };
	};
}

#endif