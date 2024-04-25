#include "Minigin.h"
#include "CoreIncludes.h"

#ifdef DEBUG_MODE
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
#include "Serializer.h"

#include "DebugDrawSystem.h"

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
	//LoadDemo();
	//LoadSceneGraphDemo();

	LoadGamePlayScripting();
}

void LoadDemo()
{
	using namespace Pengin;
	SceneData sceneData{};
	sceneData.name = "Demo Scene";

	SceneFileData data{};
	data.sceneSavePath = "../Data/DemoScene.json";

	data.saveSceneOnDestroy = false; //Easier to test if we dont do a runtime save
	data.sceneLoadPath = "../Data/DemoScene.json";

	data.inputFilePath = "../Data/InputTest.json";
	data.f_RegControllerInput = RegisterControllerInput_DemoScene;
	data.f_RegKeyboardInput = RegisterKeyboardInput_DemoScene;
	data.keepPrevInput = false;

	sceneData.sceneFileData = data;

	auto pScene = SceneManager::GetInstance().CreateScene(sceneData);

	Entity testLoadedEntity = pScene->AddEntityFromFile("../Data/TestEntityFFile.json");
	testLoadedEntity.GetComponent<TextComponent>().SetText("TEXT TEST");

	pScene->SerializeEntity(testLoadedEntity, "../Data/TestEntityToFile.json");

	//Manual scene initialization (no load file)
	//Background && logo
	//auto background = pScene->CreateEntity();
	//background.AddComponent<SpriteComponent>("background.tga");

	////Temp testing
	//background.AddComponent<RectColliderComponent>(UtilStructs::Rectu16{0, 0, 100, 100});

	//auto logo = pScene->CreateEntity( { 216, 180, 0 } );
	//logo.AddComponent<SpriteComponent>("logo.tga");
	////-------------------

	////Text---------------
	//auto title = pScene->CreateEntity({ 80, 20, 0 });
	//title.AddComponent<SpriteComponent>();
	//title.AddComponent<TextComponent>("Lingua.otf", 36, "Programming 4 Assignment");

	//auto descr1 = pScene->CreateEntity({ 100, 75, 0 });
	//descr1.AddComponent<SpriteComponent>();
	//descr1.AddComponent<TextComponent>("Lingua.otf", 20, "WASD to move, C attack, B score");

	//auto descr2 = pScene->CreateEntity({ 100, 100, 0 });
	//descr2.AddComponent<SpriteComponent>();
	//descr2.AddComponent<TextComponent>("Lingua.otf", 20, "IJKL/D-Pad to move, V attack, N score");
	////---------------------

	////FPS----------------
	//auto fps = pScene->CreateEntity();
	//fps.AddComponent<SpriteComponent>();
	//fps.AddComponent<TextComponent>("Lingua.otf", 36);
	//fps.AddComponent<FPSCounterComponent>();
	////-------------------

	////2 Players
	////Player 1-----------
	//auto player1 = pScene->CreateEntity({ 200, 200, 0 }, {}, {2,2,0});
	//player1.AddComponent<PlayerComponent>(user1);
	//player1.AddComponent<SpriteComponent>("pengoLowQualityFortesting.png");
	//player1.AddComponent<VelocityComponent>();
	//auto& player1Health = player1.AddComponent<HealthComponent>(3);
	//auto& player1Score = player1.AddComponent<ScoreComponent>();

	//player1.AddComponent<RectColliderComponent>( UtilStructs::Rectu16{10, 10, 100, 100} );

	////Displays
	//auto healthChar1 = pScene->CreateEntity({ 0, 150, 0 });
	//healthChar1.AddComponent<SpriteComponent>();
	//healthChar1.AddComponent<TextComponent>("Lingua.otf", 18, "Player 1 Health: 3");
	//healthChar1.AddComponent<TxtDisplayComponent>("Player 1 Health: ");

	//player1Health.m_HealthDisplayIds.emplace_back(healthChar1.GetEntityId());

	//auto scoreChar1 = pScene->CreateEntity({ 0, 175, 0 });
	//scoreChar1.AddComponent<SpriteComponent>();
	//scoreChar1.AddComponent<TextComponent>("Lingua.otf", 18, "Player 1 Score: 0"); //Todo no text display init
	//scoreChar1.AddComponent<TxtDisplayComponent>("Player 1 Score: ");

	//player1Score.m_ScoreDisplays.emplace_back(scoreChar1.GetEntityId());

	//pScene->SetPlayer(user1, player1); 
	////-------------------
	//
	////Player 2-----------
	//auto player2 = pScene->CreateEntity({ 100, 200, 0 });
	//player2.AddComponent<PlayerComponent>(user2);
	//player2.AddComponent<SpriteComponent>("pengoLowQualityFortesting.png");
	//player2.AddComponent<VelocityComponent>();
	//auto& player2Health = player2.AddComponent<HealthComponent>(3);
	//auto& player2Score = player2.AddComponent<ScoreComponent>();

	////Displays
	//auto healthChar2 = pScene->CreateEntity({ 0, 200, 0 });
	//healthChar2.AddComponent<SpriteComponent>();
	//healthChar2.AddComponent<TextComponent>("Lingua.otf", 18, "Player 2 Health: 3");
	//healthChar2.AddComponent<TxtDisplayComponent>("Player 2 Health: ");

	//player2Health.m_HealthDisplayIds.emplace_back(healthChar2.GetEntityId());

	//auto scoreChar2 = pScene->CreateEntity({ 0, 225, 0 });
	//scoreChar2.AddComponent<SpriteComponent>();
	//scoreChar2.AddComponent<TextComponent>("Lingua.otf", 18, "Player 2 Score: 0"); //Todo no text display init
	//scoreChar2.AddComponent<TxtDisplayComponent>("Player 2 Score: ");

	//player2Score.m_ScoreDisplays.emplace_back(scoreChar2.GetEntityId());
	//
	//pScene->SetPlayer(user2, player2);
	////----------------------------------------

	//---------------
	////
	////animation test code
	//auto aniPengo = pScene->CreateEntity({ 200, 200, 0 }, { }, { 4,4,0 });
	//aniPengo.AddComponent<SpriteComponent>("Pengo_Snobee_Spritesheet.png", UtilStructs::Rectu16{ 32, 0, 16, 16 });
	//
	//AnimationData ani{};
	//ani.frameCt = 2;
	//ani.frameDuration = 1.f;
	//ani.frame0sourceRect = UtilStructs::Rectu16{ 32, 0, 16, 16 };

	//AnimationData ani1{};
	//ani1.frameCt = 2;
	//ani1.frameDuration = 1.f;
	//ani1.frame0sourceRect = UtilStructs::Rectu16{ 0, 0, 16, 16 };

	//std::vector<AnimationData> anis;
	//anis.emplace_back(ani);
	//anis.emplace_back(ani1);

	//aniPengo.AddComponent<AnimationComponent>(anis);
	////---------------------------
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
	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapControllerAction(userIndex, ControllerButton::DPadLeft, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ -1, 0, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadRight, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 1, 0, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadUp, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, -1, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadDown, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, 1, 0 }));
}

void RegisterKeyboardInput_DemoScene(const Pengin::InputData& inpData)
{
	using namespace Pengin;
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

	input.MapKeyboardAction(userIndex, KeyBoardKey::C, InputState::UpThisFrame, std::make_shared<AttackPlayer>(userIndex));
	input.MapKeyboardAction(userIndex, KeyBoardKey::V, InputState::Pressed, std::make_shared<CollectScore>(userIndex));

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
