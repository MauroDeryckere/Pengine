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

#include "InputManager.h"
#include "Components.h"

#include "InputCommands.h"
#include "PengoInputCommands.h"

#include "ServiceLocator.h"

#include "DebugDrawSystem.h"
#include "PlayerSystem.h"
#include "EnemySystem.h"
#include "UIDisplaySystem.h"
#include "BlockSystem.h"
#include "GameSystem.h"

#include "TestSerComponent.h"

void LoadGamePlayScripting();

void LoadDemo();
void LoadSceneGraphDemo();

void RegisterControllerInput_DemoScene(const Pengin::InputData& inpData);
void RegisterKeyboardInput_DemoScene(const Pengin::InputData& inpData);

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
	LoadDemo();
	//LoadSceneGraphDemo();
	//LoadGamePlayScripting();
}

void LoadDemo()  
{
	using namespace Pengin;

	//Most of thesceneData is actually read from a file if you choose a loadPath, just typing it here too for readability
	SceneData sceneData{};
	sceneData.name = "Pengo Scene";

	SceneFileData data{};
	data.sceneSavePath = "";
	data.saveSceneOnDestroy = false;
	data.sceneLoadPath = "../Data/PengoSaveBackup.json";

	data.inputFilePath = "../Data/InputTest.json";
	data.f_RegControllerInput = RegisterControllerInput_DemoScene;
	data.f_RegKeyboardInput = RegisterKeyboardInput_DemoScene;
	data.keepPrevInput = false; //boolean to override any funcs

	sceneData.sceneFileData = data;

	ServiceLocator::GetSoundSystem().LoadSoundsFromFolder("../Data/Audio");
	auto pScene = SceneManager::GetInstance().CreateScene(sceneData);

	pScene->RegisterSystems([&](SystemManager& sysManager, ECS& ecs) 
		{ 
			sysManager.RegisterSystem<Pengo::GameSystem>(std::make_shared<Pengo::GameSystem>(ecs) );

			sysManager.RegisterSystem<Pengo::PlayerSystem>(std::make_shared<Pengo::PlayerSystem>(ecs) ); 
			sysManager.RegisterSystem<Pengo::EnemySystem>(std::make_shared<Pengo::EnemySystem>(ecs) );
			sysManager.RegisterSystem<Pengo::BlockSystem>(std::make_shared<Pengo::BlockSystem>(ecs) );

			sysManager.RegisterSystem<Pengo::UIDisplaySystem>(std::make_shared<Pengo::UIDisplaySystem>(ecs, pScene.get()) );
		});
}

void LoadGamePlayScripting()
{
	using namespace Pengin;

	SceneData sceneData{};
	sceneData.name = "Gameplay Scripting";

	auto pScene = SceneManager::GetInstance().CreateScene(sceneData);	

	auto& input = InputManager::GetInstance();
	const auto userIdx = input.RegisterUser(UserType::Keyboard);

	pScene->RegisterSystems([](SystemManager& sysManager, ECS& ecs)
		{
			sysManager.RegisterSystem<DebugDrawSystem>(std::make_shared<DebugDrawSystem>(ecs));
		}
	);

	auto player = pScene->CreateEntity({ 20, 20, 0 }, {}, {1,1,1}, userIdx);
	player.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,255,255,255 }, uint16_t{ 100 }, uint16_t{100}, true);
}

void RegisterControllerInput_DemoScene(const Pengin::InputData& inpData)
{
	using namespace Pengin;
	using namespace Pengo;

	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapControllerAction(userIndex, ControllerButton::DPadLeft, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ -1, 0, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadRight, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 1, 0, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadUp, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, -1, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadDown, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, 1, 0 }));

	input.MapControllerAction(userIndex, ControllerButton::A, InputState::UpThisFrame, std::make_shared<AttackPlayer>(userIndex));
	input.MapControllerAction(userIndex, ControllerButton::B, InputState::Pressed, std::make_shared<CollectScore>(userIndex));
	input.MapControllerAction(userIndex, ControllerButton::X, InputState::DownThisFrame, std::make_shared<MakeSound>(userIndex, SoundData{"../Data/Audio/Act Start.mp3"}));
}

void RegisterKeyboardInput_DemoScene(const Pengin::InputData& inpData)
{
	using namespace Pengin;
	using namespace Pengo;

	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapKeyboardAction(userIndex, KeyBoardKey::Left, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ -1, 0, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::Right, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 1, 0, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::Up, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, -1, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::Down, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, 1, 0 }));

	input.MapKeyboardAction(userIndex, KeyBoardKey::A, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ -1, 0, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::D, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 1, 0, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::W, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, -1, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::S, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, 1, 0 }));

	input.MapKeyboardAction(userIndex, KeyBoardKey::E, InputState::Pressed, std::make_shared<Pengo::BreakBlock>(userIndex));

	input.MapKeyboardAction(userIndex, KeyBoardKey::C, InputState::UpThisFrame, std::make_shared<AttackPlayer>(userIndex));
	input.MapKeyboardAction(userIndex, KeyBoardKey::V, InputState::Pressed, std::make_shared<CollectScore>(userIndex));

	SoundData data{ "../Data/Audio/Act Start.mp3" };

	ServiceLocator::GetSoundSystem().LoadSound(data);

	input.MapKeyboardAction(userIndex, KeyBoardKey::B, InputState::DownThisFrame, std::make_shared<MakeSound>(userIndex, data));

	auto a1 [[maybe_unused]] = input.MapKeyboardAction(userIndex, KeyBoardKey::T, InputState::Pressed, std::make_shared<InpDebugCommand>(userIndex, "T down"));
	auto a2 [[maybe_unused]] = input.MapKeyboardAction(userIndex, KeyBoardKey::Y, InputState::Pressed, std::make_shared<InpDebugCommand>(userIndex, "Y down"));
	input.MapKeyboardAction(userIndex, KeyBoardKey::U, InputState::Pressed, std::make_shared<InpDebugCommand>(userIndex, "U down"));

	//Example of mapping a combo
	InputCombo combo;
	combo.pComboActions.emplace_back(a1);
	combo.pComboActions.emplace_back(a2);

	combo.allowedDelay.emplace_back(1.f);

	auto a3 = std::make_shared<InpDebugCommand>(userIndex, "Resulting action");
	combo.pResultingAction = a3;
	input.MapCombo(userIndex, combo);
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
