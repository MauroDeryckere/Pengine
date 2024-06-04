#include "Minigin.h"
#include "CoreIncludes.h"

#ifdef USE_VLD
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#ifdef USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable: 4996)
#include "steam_api.h"
#pragma warning (pop)
#endif

#include "LevelManager.h"

void Load();
void LoadGamePlayScripting();

int main(int, char* [])
{
#ifdef USE_STEAMWORKS
	if (!SteamAPI_Init())
	{
		DEBUG_OUT("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).");
		return 1;
	}
	DEBUG_OUT("Successfully initialized steam.");
#endif

	dae::Minigin engine("../Data/");
	engine.Run(Load);

#ifdef USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif

	return 0;
}

void Load()
{
	LoadGamePlayScripting();
}

void LoadGamePlayScripting()
{
	using namespace Pengin;
	using namespace GS;

	//Init
	auto& l [[maybe_unused]] = LevelManager::GetInstance();

	EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("LoadPlayGame"));
}
