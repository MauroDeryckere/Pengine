#include "LevelManager.h"

#include "SceneManager.h"

#include "Entity.h"
#include "Scene.h"

#include "InputCommands.h"
#include "InputManager.h"

#include "DebugDrawSystem.h"
#include "MinerSystem.h"
#include "OreSystem.h"
#include "FactorySystem.h"
#include "SpikeSystem.h"

#include "GSInputCommands.h"

#include "MinerComponent.h"
#include "OreComponent.h"
#include "FactoryComponent.h"
#include "SpikeComponent.h"

#include "GameOverEvent.h"
#include "VictoryEvent.h"

void GS::LevelManager::LoadLevel()
{
	using namespace Pengin;

	//Scene Setup
	SceneData sceneData{};
	sceneData.name = "Gameplay Scripting Level";

	auto pScene = SceneManager::GetInstance().CreateScene(sceneData);
	pScene->RegisterSystems([](SystemManager& sysManager, ECS& ecs)
		{
			sysManager.RegisterSystem<DebugDrawSystem>(std::make_shared<DebugDrawSystem>(ecs));
			sysManager.RegisterSystem<FactorySystem>(std::make_shared<FactorySystem>(ecs));
			sysManager.RegisterSystem<MinerSystem>(std::make_shared<MinerSystem>(ecs));
			sysManager.RegisterSystem<OreSytem>(std::make_shared<OreSytem>(ecs));
			sysManager.RegisterSystem<SpikeSystem>(std::make_shared<SpikeSystem>(ecs));
		}
	);
	//-----------

	//INPUT
	auto& input = InputManager::GetInstance();
	input.Clear();

	const auto userIdx = input.RegisterUser(UserType::Keyboard);

	input.MapKeyboardAction(userIdx, KeyBoardKey::Left, InputState::Pressed, std::make_shared<Movement>(userIdx, glm::vec3{ -1.f, 0.f, 0.f }));
	input.MapKeyboardAction(userIdx, KeyBoardKey::Right, InputState::Pressed, std::make_shared<Movement>(userIdx, glm::vec3{ 1.f, 0.f, 0.f }));
	input.MapKeyboardAction(userIdx, KeyBoardKey::Up, InputState::Pressed, std::make_shared<Mine>(userIdx));
	input.MapKeyboardAction(userIdx, KeyBoardKey::Down, InputState::DownThisFrame, std::make_shared<DropOre>(userIdx));
	//----------

	//Player
	auto player = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 100, 100 }, { 100, 250, 0 });
	pScene->SetPlayer(userIdx, player);

	player.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,255,0,255 }, uint16_t{ 100 }, uint16_t{ 100 }, true);

	auto& miner = player.AddComponent<MinerComponent>();
	miner.baseWeight = -50;

	auto scoreDispl = pScene->CreateEntity({20.f, 680.f, 0.f});
	scoreDispl.AddComponent<SpriteComponent>();
	scoreDispl.AddComponent<TextComponent>("Lingua.otf", 32, "50", glm::u8vec4{0, 0, 255, 255});

	miner.oreDisplayId = scoreDispl.GetEntityId();
	miner.ores.emplace_back(50);
	miner.totOreWeight = 50;
	//----------

	//Enviroment

	//rocks
	auto topLevel = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 1280, 75 }, { 0,-50.f,0 });
	topLevel.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,255,255,255 }, uint16_t{ 1280 }, uint16_t{ 75 }, true);
	topLevel.GetComponent<BodyComponent>().collType = CollType::Static;

	auto rock0 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 200, 50}, { 175.f, 400.f,0 });
	rock0.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,255,255,255 }, uint16_t{ 200 }, uint16_t{ 50 }, true);
	rock0.GetComponent<BodyComponent>().collType = CollType::Static;

	auto rock1 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 50, 300 }, { 675.f, 100.f,0 });
	rock1.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,255,255,255 }, uint16_t{ 50 }, uint16_t{ 300 }, true);
	rock1.GetComponent<BodyComponent>().collType = CollType::Static;

	auto rock2 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 500, 50 }, { 475.f, 550.f,0 });
	rock2.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,255,255,255 }, uint16_t{ 500 }, uint16_t{ 50 }, true);
	rock2.GetComponent<BodyComponent>().collType = CollType::Static;

	auto rock3 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 200, 50 }, { 900.f, 100.f,0 });
	rock3.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,255,255,255 }, uint16_t{ 200 }, uint16_t{ 50 }, true);
	rock3.GetComponent<BodyComponent>().collType = CollType::Static;
	auto rock4 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 100, 50 }, { 900.f, 275 ,0 });
	rock4.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,255,255,255 }, uint16_t{ 100 }, uint16_t{ 50 }, true);
	rock4.GetComponent<BodyComponent>().collType = CollType::Static;
	//-----

	//factory
	auto factory = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 100, 75 }, { 1230.f, 550.f, 0.f });
	factory.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,255,0,255 }, uint16_t{ 100 }, uint16_t{ 75 }, true);
	factory.GetComponent<BodyComponent>().collType = CollType::Static;
	factory.AddComponent<FactoryComponent>();
	//-------

	//ores
	auto ore0 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 25, 75 }, { 100.f, 20.f,0.f });
	ore0.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,0,255,255 }, uint16_t{ 25 }, uint16_t{ 75 }, true);
	ore0.GetComponent<BodyComponent>().collType = CollType::Static;
	ore0.AddComponent<OreComponent>().weight = 100;

	auto ore1 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 25, 50 }, { 25.f, 20.f,0.f });
	ore1.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,0,255,255 }, uint16_t{ 25 }, uint16_t{ 50 }, true);
	ore1.GetComponent<BodyComponent>().collType = CollType::Static;
	ore1.AddComponent<OreComponent>().weight = 65;


	auto ore2 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 50, 12 }, { 635.f, 100.f ,0.f });
	ore2.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,0,255,255 }, uint16_t{ 50 }, uint16_t{ 12 }, true);
	ore2.GetComponent<BodyComponent>().collType = CollType::Static;
	ore2.AddComponent<OreComponent>().weight = 65;

	auto ore3 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 50, 10 }, { 635.f, 200.f ,0.f });
	ore3.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,0,255,255 }, uint16_t{ 50 }, uint16_t{ 10 }, true);
	ore3.GetComponent<BodyComponent>().collType = CollType::Static;
	ore3.AddComponent<OreComponent>().weight = 50;

	auto ore4 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 50, 10 }, { 635.f, 240.f ,0.f });
	ore4.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,0,255,255 }, uint16_t{ 50 }, uint16_t{ 10 }, true);
	ore4.GetComponent<BodyComponent>().collType = CollType::Static;
	ore4.AddComponent<OreComponent>().weight = 50;

	auto ore5 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 50, 10 }, { 675.f + 5.F , 250.f ,0.f });
	ore5.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,0,255,255 }, uint16_t{ 50 }, uint16_t{ 10 }, true);
	ore5.GetComponent<BodyComponent>().collType = CollType::Static;
	ore5.AddComponent<OreComponent>().weight = 50;

	auto ore6 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 12, 30 }, { 1050, 150.f ,0.f });
	ore6.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,0,255,255 }, uint16_t{ 12 }, uint16_t{ 30 }, true);
	ore6.GetComponent<BodyComponent>().collType = CollType::Static;
	ore6.AddComponent<OreComponent>().weight = 50;

	auto ore7 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 115, 20 }, { 850 + 100.f + 50.f, 275.f + 50.f - 10.f ,0.f });
	ore7.AddComponent<DebugDrawComponent>(glm::u8vec4{ 0,0,255,255 }, uint16_t{ 115 }, uint16_t{ 20 }, true);
	ore7.GetComponent<BodyComponent>().collType = CollType::Static;
	ore7.AddComponent<OreComponent>().weight = 200;
	//----------

	//Spikes
	auto spike0 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 10, 45 }, { 300, 20.f,0.f });
	spike0.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,0,0,255 }, uint16_t{ 10 }, uint16_t{ 45 }, true);
	spike0.GetComponent<BodyComponent>().collType = CollType::Static;
	spike0.AddComponent<SpikeComponent>();

	auto spike1 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 10, 175 }, { 300, 225.f ,0.f });
	spike1.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,0,0,255 }, uint16_t{ 10 }, uint16_t{ 175 }, true);
	spike1.GetComponent<BodyComponent>().collType = CollType::Static;
	spike1.AddComponent<SpikeComponent>();

	auto spike2 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 50, 10 }, { 375.f, 425.f ,0.f });
	spike2.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,0,0,255 }, uint16_t{ 50 }, uint16_t{ 10 }, true);
	spike2.GetComponent<BodyComponent>().collType = CollType::Static;
	spike2.AddComponent<SpikeComponent>();

	auto spike3 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 100, 10 }, { 375.f + 50.f + 175.f, 400.f ,0.f });
	spike3.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,0,0,255 }, uint16_t{ 100 }, uint16_t{ 10 }, true);
	spike3.GetComponent<BodyComponent>().collType = CollType::Static;
	spike3.AddComponent<SpikeComponent>();

	auto spike4 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 50, 10 }, { 850, 275.f + 50.f - 10.f ,0.f });
	spike4.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,0,0,255 }, uint16_t{ 50 }, uint16_t{ 10 }, true);
	spike4.GetComponent<BodyComponent>().collType = CollType::Static;
	spike4.AddComponent<SpikeComponent>();

	auto spike5 = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 0, 0, 20, 100 }, { 850 + 100.f + 50.f - 40.f, 275.f + 50.f - 10.f ,0.f });
	spike5.AddComponent<DebugDrawComponent>(glm::u8vec4{ 255,0,0,255 }, uint16_t{ 20 }, uint16_t{ 100 }, true);
	spike5.GetComponent<BodyComponent>().collType = CollType::Static;
	spike5.AddComponent<SpikeComponent>();
	//-------
	
	//----------

	m_StartTime = std::chrono::steady_clock::now();
}

void GS::LevelManager::LoadPlayGame() 
{
	using namespace Pengin;

	SceneData data{ };
	data.name = "Play Game Screen";

	auto pScene = SceneManager::GetInstance().CreateScene(data);

	auto textEnt2 = pScene->CreateEntity({ 75.f, 200.f, 0.f });
	textEnt2.AddComponent<SpriteComponent>();
	textEnt2.AddComponent<TextComponent>("Lingua.otf", 56, "Press Space to play");

	auto textEnt3 = pScene->CreateEntity({ 75.f, 400.f, 0.f });
	textEnt3.AddComponent<SpriteComponent>();
	textEnt3.AddComponent<TextComponent>("Lingua.otf", 28, "Keybinds: ");

	auto textEnt4 = pScene->CreateEntity({ 75.f, 430.f, 0.f });
	textEnt4.AddComponent<SpriteComponent>();
	textEnt4.AddComponent<TextComponent>("Lingua.otf", 26, "Left/Right arrow: Move");

	auto textEnt5 = pScene->CreateEntity({ 75.f, 460.f, 0.f });
	textEnt5.AddComponent<SpriteComponent>();
	textEnt5.AddComponent<TextComponent>("Lingua.otf", 26, "Up arrow: Mine");

	auto textEnt6 = pScene->CreateEntity({ 75.f, 490.f, 0.f });
	textEnt6.AddComponent<SpriteComponent>();
	textEnt6.AddComponent<TextComponent>("Lingua.otf", 26, "Down arrow: Drop");


	auto textEnt7 = pScene->CreateEntity({ 375.f, 400.f, 0.f });
	textEnt7.AddComponent<SpriteComponent>();
	textEnt7.AddComponent<TextComponent>("Lingua.otf", 28, "Game Info: ");

	auto textEnt8 = pScene->CreateEntity({ 375.f, 430.f, 0.f });
	textEnt8.AddComponent<SpriteComponent>();
	textEnt8.AddComponent<TextComponent>("Lingua.otf", 26, "The goal is to bring as much ore as possible to the factory (green area)");

	auto textEnt9 = pScene->CreateEntity({ 375.f, 460.f, 0.f });
	textEnt9.AddComponent<SpriteComponent>();
	textEnt9.AddComponent<TextComponent>("Lingua.otf", 26, "The blue rects are ores you can mine to increase the miner weight");

	auto textEnt10 = pScene->CreateEntity({ 375.f, 490.f, 0.f });
	textEnt10.AddComponent<SpriteComponent>();
	textEnt10.AddComponent<TextComponent>("Lingua.otf", 26, "You can drop the collected ores in rev order to decrease your weight again");

	auto textEnt11 = pScene->CreateEntity({ 375.f, 520.f, 0.f });
	textEnt11.AddComponent<SpriteComponent>();
	textEnt11.AddComponent<TextComponent>("Lingua.otf", 26, "Red rects are spikes and will kill you");


	auto& input = InputManager::GetInstance();
	input.Clear();

	auto user = input.RegisterUser(UserType::Keyboard);
	input.MapKeyboardAction(user, KeyBoardKey::SpaceBar, InputState::DownThisFrame, std::make_shared<Restart>(user));
}

void GS::LevelManager::LoadGameOver(const Pengin::BaseEvent& event)
{
	using namespace Pengin;
	event;
	//const auto& gameOverEvent{ static_cast<const GameOverEvent&>(event) };

	SceneData data{ };
	data.name = "Game over";

	auto pScene = SceneManager::GetInstance().CreateScene(data);

	auto title = pScene->CreateEntity({ 75.f, 50, 0.f });
	title.AddComponent<SpriteComponent>();
	title.AddComponent<TextComponent>("Lingua.otf", 56, "Game Over");

	auto textEnt = pScene->CreateEntity({ 75.f, 200.f, 0.f });
	textEnt.AddComponent<SpriteComponent>();
	textEnt.AddComponent<TextComponent>("Lingua.otf", 48, "Press Space to play again");

	auto& input = InputManager::GetInstance();
	input.Clear();

	auto user = input.RegisterUser(UserType::Keyboard);
	input.MapKeyboardAction(user, KeyBoardKey::SpaceBar, InputState::DownThisFrame, std::make_shared<Restart>(user));
}

void GS::LevelManager::LoadVictory(const Pengin::BaseEvent& event)
{
	const auto winTime{ std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_StartTime).count() };

	using namespace Pengin;
	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();

	const auto& victoryEvent{ static_cast<const VictoryEvent&>(event) };
	auto player = pActiveScene->GetPlayer(victoryEvent.GetUserIndex());

	const auto oreWeight{ player.GetComponent<MinerComponent>().totOreWeight };

	SceneData data{ };
	data.name = "Victory Screen";

	auto pScene = SceneManager::GetInstance().CreateScene(data);

	auto title = pScene->CreateEntity({ 75.f, 50, 0.f });
	title.AddComponent<SpriteComponent>();
	title.AddComponent<TextComponent>("Lingua.otf", 56, "Victory");

	auto textEnt = pScene->CreateEntity({ 75.f, 200.f, 0.f });
	textEnt.AddComponent<SpriteComponent>();
	textEnt.AddComponent<TextComponent>("Lingua.otf", 48, "Press Space to play again");

	auto textEnt2 = pScene->CreateEntity({ 75.f, 300.f, 0.f });
	textEnt2.AddComponent<SpriteComponent>();
	std::string oreStr{ "Total mined ore: " + std::to_string(oreWeight) };
	textEnt2.AddComponent<TextComponent>("Lingua.otf", 48, oreStr);

	auto textEnt3 = pScene->CreateEntity({ 75.f, 400.f, 0.f });
	textEnt3.AddComponent<SpriteComponent>();

	const auto minutes{ winTime / 60 };
	const auto seconds{ winTime - minutes * 60 };

	std::string timeStr{ "Time spent: " + (minutes != 0 ? std::to_string(minutes) + " min " : "" ) +  std::to_string(seconds) + " seconds"};
	textEnt3.AddComponent<TextComponent>("Lingua.otf", 48, timeStr);

	auto& input = InputManager::GetInstance();
	input.Clear();

	auto user = input.RegisterUser(UserType::Keyboard);
	input.MapKeyboardAction(user, KeyBoardKey::SpaceBar, InputState::DownThisFrame, std::make_shared<Restart>(user));
}
