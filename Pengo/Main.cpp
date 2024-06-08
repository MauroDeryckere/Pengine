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

#include "GameManager.h"

#include "InputCommands.h"
#include "PengoInputCommands.h"
#include "InputManager.h"

#include "ServiceLocator.h"

#include "DebugDrawSystem.h"

void LoadPengo();
void LoadSceneGraphDemo();

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
	LoadPengo();
	//LoadSceneGraphDemo();
}

void LoadPengo()  
{
	Pengin::ServiceLocator::GetSoundSystem().LoadSoundsFromFolder("../Data/Audio");

	Pengo::GameManager::GetInstance().LoadStartUI();
}

void LoadSceneGraphDemo()
{
	using namespace Pengin;
	SceneData sceneData{};
	sceneData.name = "Demo Scene";
	auto scene = SceneManager::GetInstance().CreateScene(sceneData);

	Entity testPlayer = scene->CreateEntity(glm::vec3{ 100, 100, 0 }, {}, { 2,2,0 });
	testPlayer.AddComponent<SpriteComponent>("pengoLowQualityFortesting.png", UtilStructs::Rectu16{ 0, 0, 50, 50 });

	Entity testPlayer2 = scene->CreateEntity(glm::vec3{ 100, 100, 0 });
	testPlayer2.AddComponent<SpriteComponent>("pengoLowQualityFortesting.png");

	Entity testPlayer3 = scene->CreateEntity(glm::vec3{ 150, 150, 0 });
	testPlayer3.AddComponent<SpriteComponent>("pengoLowQualityFortesting.png");

	testPlayer2.SetParent(testPlayer, true);
	testPlayer2.SetLocalPosition({ 100, 0, 0 });
	testPlayer3.SetParent(testPlayer2);

	Entity testPlayer4 = scene->CreateEntity(glm::vec3{ 50, -150, 0 });
	testPlayer4.AddComponent<SpriteComponent>("pengoLowQualityFortesting.png");

	testPlayer4.SetParent(testPlayer3);

	Entity testPlayer5 = scene->CreateEntity(glm::vec3{ 50, -150, 0 });
	testPlayer5.AddComponent<SpriteComponent>("pengoLowQualityFortesting.png");

	Entity testPlayer6 = scene->CreateEntity(glm::vec3{ -50, -150, 0 });
	testPlayer6.AddComponent<SpriteComponent>("pengoLowQualityFortesting.png");

	testPlayer5.SetParent(testPlayer4);
	testPlayer6.SetParent(testPlayer4);

	/* curr graph layout
	Id 0
		Id 1
			Id 2
				Id 3
					Id 4
					Id 5
	*/

	DEBUG_OUT("orginal scene graph: ");
	DEBUG_OUT("\n Id 0\n\tId 1\n\t\t Id 2\n\t\t\t Id 3\n\t\t\t\t Id 4\n\t\t\t\t Id 5");

	//scene->DestroyEntity(testPlayer4, true); //Destroy any entity
}
