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

#include "SceneManager.h"

#include "Entity.h"
#include "Scene.h"

#include "InputCommands.h"
#include "InputManager.h"

#include "ServiceLocator.h"

#include "DebugDrawSystem.h"

void LoadGamePlayScripting();
void Load();

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
	//using namespace Pengin;

	//SceneData sceneData{};
	//sceneData.name = "Gameplay Scripting";

	//auto pScene = SceneManager::GetInstance().CreateScene(sceneData);

	//auto& input = InputManager::GetInstance();
	//const auto userIdx = input.RegisterUser(UserType::Keyboard);

	//pScene->RegisterSystems([](SystemManager& sysManager, ECS& ecs)
	//	{
	//		sysManager.RegisterSystem<DebugDrawSystem>(std::make_shared<DebugDrawSystem>(ecs));
	//	}
	//);

	//auto player = pScene->CreateEntity({ 20, 20, 0 }, {}, { 1,1,1 }, userIdx);
	//player.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,255,255,255 }, uint16_t{ 100 }, uint16_t{ 100 }, true);
}
