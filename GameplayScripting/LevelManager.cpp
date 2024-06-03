#include "LevelManager.h"

#include "SceneManager.h"

#include "Entity.h"
#include "Scene.h"

#include "InputCommands.h"
#include "InputManager.h"

#include "DebugDrawSystem.h"
#include "MinerSystem.h"
#include "OreSystem.h"

#include "GSInputCommands.h"

#include "MinerComponent.h"
#include "OreComponent.h"
#include "FactoryComponent.h"

void GS::LevelManager::LoadLevel()
{
	using namespace Pengin;

	//Scene Setup
	SceneData sceneData{};
	sceneData.name = "Gameplay Scripting";

	auto pScene = SceneManager::GetInstance().CreateScene(sceneData);
	pScene->RegisterSystems([](SystemManager& sysManager, ECS& ecs)
		{
			sysManager.RegisterSystem<DebugDrawSystem>(std::make_shared<DebugDrawSystem>(ecs));
			sysManager.RegisterSystem<MinerSystem>(std::make_shared<MinerSystem>(ecs));
			sysManager.RegisterSystem<OreSytem>(std::make_shared<OreSytem>(ecs));
		}
	);
	//-----------

	//INPUT
	auto& input = InputManager::GetInstance();
	const auto userIdx = input.RegisterUser(UserType::Keyboard);

	input.MapKeyboardAction(userIdx, KeyBoardKey::Left, InputState::Pressed, std::make_shared<Movement>(userIdx, glm::vec3{ -1.f, 0.f, 0.f }));
	input.MapKeyboardAction(userIdx, KeyBoardKey::Right, InputState::Pressed, std::make_shared<Movement>(userIdx, glm::vec3{ 1.f, 0.f, 0.f }));
	input.MapKeyboardAction(userIdx, KeyBoardKey::Up, InputState::Pressed, std::make_shared<Mine>(userIdx));
	input.MapKeyboardAction(userIdx, KeyBoardKey::Down, InputState::Pressed, std::make_shared<DropOre>(userIdx));
	//----------

	//Player
	auto player = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 100, 100 }, { 100, 100, 0 });
	pScene->SetPlayer(userIdx, player);

	player.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,255,0,255 }, uint16_t{ 100 }, uint16_t{ 100 }, true);

	auto& miner = player.AddComponent<MinerComponent>();
	miner.totalWeight = -10;

	auto scoreDispl = pScene->CreateEntity();
	scoreDispl.AddComponent<SpriteComponent>();
	scoreDispl.AddComponent<TextComponent>("Lingua.otf", 32, "-10", glm::uvec4{0, 0, 255, 255});

	miner.oreDisplayId = scoreDispl.GetEntityId();
	//----------

	//Enviroment
	auto topLevel = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 1280, 75 }, { 0,-50.f,0 });
	topLevel.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,255,255,255 }, uint16_t{ 1280 }, uint16_t{ 75 }, true);
	topLevel.GetComponent<BodyComponent>().collType = CollType::Static;
	
	auto factory = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 100, 75 }, { 1230.f, 100.f, 0.f });
	factory.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,255,0,255 }, uint16_t{ 100 }, uint16_t{ 75 }, true);
	factory.GetComponent<BodyComponent>().collType = CollType::Static;
	factory.AddComponent<FactoryComponent>();

	//ores
	auto ore0 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 25, 75 }, { 100.f, 20.f,0.f });
	ore0.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,0,255,255 }, uint16_t{ 25 }, uint16_t{ 75 }, true);
	ore0.GetComponent<BodyComponent>().collType = CollType::Static;
	ore0.AddComponent<OreComponent>().weight = 100;
	//----------
	
	//----------
}

void GS::LevelManager::LoadRestart() 
{
	using namespace Pengin;

	SceneData data{ };
	data.name = "Game Over Screen";

	auto pScene = SceneManager::GetInstance().CreateScene(data);

	auto textEnt2 = pScene->CreateEntity({ 100.f, 200.f, 0.f });
	textEnt2.AddComponent<SpriteComponent>();
	textEnt2.AddComponent<TextComponent>("Lingua.otf", 56, "Press Space to play");

	auto textEnt3 = pScene->CreateEntity({ 100.f, 400.f, 0.f });
	textEnt3.AddComponent<SpriteComponent>();
	textEnt3.AddComponent<TextComponent>("Lingua.otf", 28, "Keybinds: ");

	auto textEnt4 = pScene->CreateEntity({ 100.f, 430.f, 0.f });
	textEnt4.AddComponent<SpriteComponent>();
	textEnt4.AddComponent<TextComponent>("Lingua.otf", 26, "Left/Right arrow: Move");

	auto textEnt5 = pScene->CreateEntity({ 100.f, 460.f, 0.f });
	textEnt5.AddComponent<SpriteComponent>();
	textEnt5.AddComponent<TextComponent>("Lingua.otf", 26, "Up arrow: Mine");

	auto textEnt6 = pScene->CreateEntity({ 100.f, 490.f, 0.f });
	textEnt6.AddComponent<SpriteComponent>();
	textEnt6.AddComponent<TextComponent>("Lingua.otf", 26, "Down arrow: Drop");


	auto textEnt7 = pScene->CreateEntity({ 420.f, 400.f, 0.f });
	textEnt7.AddComponent<SpriteComponent>();
	textEnt7.AddComponent<TextComponent>("Lingua.otf", 28, "Game Info: ");

	auto textEnt8 = pScene->CreateEntity({ 420.f, 430.f, 0.f });
	textEnt8.AddComponent<SpriteComponent>();
	textEnt8.AddComponent<TextComponent>("Lingua.otf", 26, "The goal rect is to bring as much ore to the factory as possible (green area)");

	auto textEnt9 = pScene->CreateEntity({ 420.f, 460.f, 0.f });
	textEnt9.AddComponent<SpriteComponent>();
	textEnt9.AddComponent<TextComponent>("Lingua.otf", 26, "The blue rects are ores you can mine too increase the miner weight");

	auto textEnt10 = pScene->CreateEntity({ 420.f, 490.f, 0.f });
	textEnt10.AddComponent<SpriteComponent>();
	textEnt10.AddComponent<TextComponent>("Lingua.otf", 26, "You can drop the collected ores in rev order to decrease your weight again");


	auto& input = InputManager::GetInstance();
	input.Clear();

	auto user = input.RegisterUser(UserType::Keyboard);
	input.MapKeyboardAction(user, KeyBoardKey::SpaceBar, InputState::DownThisFrame, std::make_shared<Restart>(user));
}
