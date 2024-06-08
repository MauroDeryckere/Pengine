#include "GameManager.h"

#include "SceneManager.h"
#include "InputManager.h"
#include "EventManager.h"

#include "PengoInputCommands.h"
#include "InputCommands.h"

#include "PengoGrid.h"

#include "GridSystem.h"
#include "UIDisplaySystem.h"
#include "WallSystem.h"
#include "BlockSystem.h"
#include "EnemySystem.h"
#include "PlayerSystem.h"

#include "AnimationComponent.h"
#include "TagComponent.h"
#include "OnGridTag.h"
#include "BlockComponent.h"
#include "SpriteComponent.h"
#include "ScoreComponent.h"
#include "PlayerComponent.h"
#include "HealthComponent.h"
#include "TxtDisplayComponent.h"
#include "textComponent.h"

#include "UISelectorComponent.h"
#include "UILetterComponent.h"
#include "UIGameModeSelectorComponent.h"
#include "UIGameModeComponent.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>

void Pengo::GameManager::LoadStartUI()
{
	using namespace Pengin;

	SceneData sceneData{};
	sceneData.name = "Pengo UI";

	SceneFileData data{};
	data.inputFilePath = "../Data/InputData.json";

	data.f_RegKeyboardInput = [this](const Pengin::InputData& data) { RegisterKeyboardInputUI(data); };
	data.f_RegControllerInput = [this](const Pengin::InputData& data) { RegisterControllerInputUI(data); };

	sceneData.sceneFileData = data;

	auto pScene = SceneManager::GetInstance().CreateScene(sceneData);

	auto ent = pScene->CreateEntity({ 50.f, 20.f, 0.f });
	ent.AddComponent<TextComponent>("Lingua.otf", 56, "Press spacebar to play");
	ent.AddComponent<SpriteComponent>();

	auto kbKbinds = pScene->CreateEntity({ 50.f, 100, 0.f });
	kbKbinds.AddComponent<TextComponent>("Lingua.otf", 48, "Keyboard keybinds");
	kbKbinds.AddComponent<SpriteComponent>();

	auto kbBind1 = pScene->CreateEntity({ 50.f, 150.f, 0.f });
	kbBind1.AddComponent<TextComponent>("Lingua.otf", 25, "Arrow keys: directional movement");
	kbBind1.AddComponent<SpriteComponent>();
	auto kbBind2 = pScene->CreateEntity({ 50.f, 185.f, 0.f });
	kbBind2.AddComponent<TextComponent>("Lingua.otf", 25, "E: Push/Break block");
	kbBind2.AddComponent<SpriteComponent>();

	auto kbBind3 = pScene->CreateEntity({ 50.f, 230.f, 0.f });
	kbBind3.AddComponent<TextComponent>("Lingua.otf", 25, "F1: Skip level");
	kbBind3.AddComponent<SpriteComponent>();
	auto kbBind4 = pScene->CreateEntity({ 50.f, 260.f, 0.f });
	kbBind4.AddComponent<TextComponent>("Lingua.otf", 25, "F2: Toggle sound");
	kbBind4.AddComponent<SpriteComponent>();


	auto ctrBinds = pScene->CreateEntity({ 50.f, 300.f, 0.f });
	ctrBinds.AddComponent<TextComponent>("Lingua.otf", 48, "Controller keybinds");
	ctrBinds.AddComponent<SpriteComponent>();

	auto cBind1 = pScene->CreateEntity({ 50.f, 350.f, 0.f });
	cBind1.AddComponent<TextComponent>("Lingua.otf", 25, "Dpad: directional movement");
	cBind1.AddComponent<SpriteComponent>();
	auto cBind2 = pScene->CreateEntity({ 50.f, 385.f, 0.f });
	cBind2.AddComponent<TextComponent>("Lingua.otf", 25, "A: Push/Break block");
	cBind2.AddComponent<SpriteComponent>();

	auto cBind3 = pScene->CreateEntity({ 50.f, 430.f, 0.f });
	cBind3.AddComponent<TextComponent>("Lingua.otf", 25, "Ls: Skip level");
	cBind3.AddComponent<SpriteComponent>();
	auto cBind4 = pScene->CreateEntity({ 50.f, 460.f, 0.f });
	cBind4.AddComponent<TextComponent>("Lingua.otf", 25, "Rs: Toggle sound");
	cBind4.AddComponent<SpriteComponent>();


	auto gmTitle = pScene->CreateEntity({ 50.f, 500.f, 0.f });
	gmTitle.AddComponent<TextComponent>("Lingua.otf", 48, "Gamemode: (use directional movmeent to change)");
	gmTitle.AddComponent<SpriteComponent>();

	auto gmSel = pScene->CreateEntity({50.f, 550.f, 0.f});
	gmSel.AddComponent<TextComponent>("Lingua.otf", 48, "Singleplayer Keyboard");
	gmSel.AddComponent<SpriteComponent>();
	auto& sel = gmSel.AddComponent<UIGameModeSelectorComponent>();

	auto gm0 = pScene->CreateEntity();
	gm0.AddComponent<UIGameModeComponent>("Singleplayer Keyboard");
	
	auto gm1 = pScene->CreateEntity();
	gm1.AddComponent<UIGameModeComponent>("Singleplayer Controller");

	auto gm2 = pScene->CreateEntity();
	gm2.AddComponent<UIGameModeComponent>("Multiplayer Keyboard && Controller");

	sel.gamemodeIds.emplace_back(gm0.GetEntityId());
	sel.gamemodeIds.emplace_back(gm1.GetEntityId());
	sel.gamemodeIds.emplace_back(gm2.GetEntityId());
}

void Pengo::GameManager::LoadNextLevel()
{
	using namespace Pengin;

	++m_CurrLevel;

	if (m_CurrLevel == 1)
	{
		SoundData music{ "../Data/Audio/Main BGM (Popcorn).mp3" };
		music.isLooping = true;
		m_BackGroundMusicId = ServiceLocator::GetSoundSystem().PlaySound(music);

		ECS& ecs = SceneManager::GetInstance().GetActiveScene()->GetECS();
		auto selcs = ecs.GetComponents<UIGameModeSelectorComponent>();
		m_CurrGamemode = static_cast<GameMode>(selcs.begin()->currGameMode);
	}

	if (m_CurrLevel == 4)
	{
		Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("GameWon"));
		return;
	}

	LoadLevel(m_CurrLevel);
}

void Pengo::GameManager::LoadLevel(uint8_t level)
{
	using namespace Pengin;

	struct OldPlayerData final
	{
		const UserIndex uIdx;
		const unsigned score;
		const unsigned health;
	};

	std::vector<OldPlayerData> oldPlayerData;

	if (level != 1)
	{
		auto pActiveScene = SceneManager::GetInstance().GetActiveScene();
		
		for (const auto& player : pActiveScene->GetSceneData().playerUUIDs)
		{
			auto p = pActiveScene->GetEntity(player);
			const auto& userIdx = p.GetComponent<PlayerComponent>().userIdx;
			const auto& scoreComp = p.GetComponent<ScoreComponent>();
			const auto& healthComp = p.GetComponent<HealthComponent>();

			oldPlayerData.emplace_back(userIdx, scoreComp.score, healthComp.health);
		}
	}

	const std::string levelStr{ "PengoLevel" + std::to_string(static_cast<int>(level)) };

	SceneData sceneData{};
	sceneData.name = levelStr;

	SceneFileData data{};
	data.sceneSavePath = "";
	data.saveSceneOnDestroy = false;
	data.sceneLoadPath = "../Data/" + levelStr + ".json";

	data.inputFilePath = "../Data/InputData.json";
	data.f_RegControllerInput = [this](const Pengin::InputData& data) { RegisterControllerInputLevel(data); };
	data.f_RegKeyboardInput = [this](const Pengin::InputData& data) { RegisterKeyboardInputLevel(data); };
	sceneData.sceneFileData = data;

	auto pScene = SceneManager::GetInstance().CreateScene(sceneData);

	switch (m_CurrGamemode)
	{
	case Pengo::GameManager::GameMode::SingleplayerKeyboard:
	{
		const auto p = pScene->GetEntity(pScene->GetSceneData().playerUUIDs[CONTROLLER_IDX]);
		const auto& uIdx = p.GetComponent<PlayerComponent>().userIdx;

		const auto& healthComp = p.GetComponent<HealthComponent>();
		const auto& scoreComp = p.GetComponent<ScoreComponent>();

		for (const auto dis : healthComp.healthDisplayIds)
		{
			pScene->DestroyEntity(dis);
		}
		for (const auto dis : scoreComp.scoreDisplays)
		{
			pScene->DestroyEntity(dis);
		}

		InputManager::GetInstance().UnRegisterUser(uIdx);
		pScene->RemovePlayer(uIdx);

		pScene->DestroyEntity(p.GetEntityId());
		break;
	}
	case Pengo::GameManager::GameMode::SingleplayerController:
	{
		const auto p = pScene->GetEntity(pScene->GetSceneData().playerUUIDs[KEYBOARD_IDX]);
		const auto& uIdx = p.GetComponent<PlayerComponent>().userIdx;

		const auto& healthComp = p.GetComponent<HealthComponent>();
		const auto& scoreComp = p.GetComponent<ScoreComponent>();

		for (const auto dis : healthComp.healthDisplayIds)
		{
			pScene->DestroyEntity(dis);
		}
		for (const auto dis : scoreComp.scoreDisplays)
		{
			pScene->DestroyEntity(dis);
		}

		InputManager::GetInstance().UnRegisterUser(uIdx);
		pScene->RemovePlayer(uIdx);

		pScene->DestroyEntity(p.GetEntityId());
		break;
	}
	case Pengo::GameManager::GameMode::Multiplayer:
		break;
	default:
		break;
	}
	
	pScene->RegisterSystems([&](SystemManager& sysManager, ECS& ecs)
		{
			sysManager.RegisterSystem<Pengo::PlayerSystem>(std::make_shared<Pengo::PlayerSystem>(ecs));
			sysManager.RegisterSystem<Pengo::EnemySystem>(std::make_shared<Pengo::EnemySystem>(ecs));
			sysManager.RegisterSystem<Pengo::BlockSystem>(std::make_shared<Pengo::BlockSystem>(ecs));
			sysManager.RegisterSystem<Pengo::WallSystem>(std::make_shared<Pengo::WallSystem>(ecs));

			sysManager.RegisterSystem<Pengo::UIDisplaySystem>(std::make_shared<Pengo::UIDisplaySystem>(ecs, pScene));

			//Init the grid
			auto gridComps = ecs.GetComponents<GridComponent>();
			for (auto it{ gridComps.begin() }; it != gridComps.end(); ++it)
			{
				const auto scale{ ecs.GetComponent<TransformComponent>(ecs.GetComponents<GridComponent>().GetIdFromIterator(it)).scale };

				for(uint16_t row{0}; row < it->rows; ++row)
				{
					for (uint16_t col{ 0 }; col < it->cols; ++col)
					{
						if (it->cells[row * it->cols + col].entity == NULL_ENTITY_ID && it->cells[row * it->cols + col].type == uint8_t{1})
						{
							static constexpr float BORDERSIZE{ 8.f };
							static constexpr float HUDSIZE{ 72.f };

							auto ent = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 1, 1, static_cast<uint16_t>(it->cellWidth - 2u), static_cast<uint16_t>(it->cellHeight - 2u) },
								{ BORDERSIZE * scale.x + (col * it->cellWidth * scale.x), HUDSIZE + BORDERSIZE * scale.y + (row * it->cellHeight * scale.y), 0.f },
								{ },
								scale);

							it->cells[row * it->cols + col].entity = ent.GetEntityId();

							ent.AddComponent<TagComponent>("Block Entity");
							ent.AddComponent<Pengo::BlockComponent>();
							ent.GetComponent<BodyComponent>().collType = CollType::Static;

							ent.AddComponent<SpriteComponent>("PengoFieldAndBlocks.png", UtilStructs::Rectu16{ 708, 0, it->cellWidth, it->cellHeight });

							std::vector<AnimationData> aniData;

							AnimationData ani1;
							ani1.frame0sourceRect = UtilStructs::Rectu16{ 708, 0, it->cellWidth, it->cellHeight };
							ani1.frameDuration = 0.f;
							ani1.frameCt = 1;

							AnimationData ani2;
							ani2.frame0sourceRect = UtilStructs::Rectu16{ 708, 48, it->cellWidth, it->cellHeight };
							ani2.frameDuration = 0.125f;
							ani2.frameCt = 8;

							aniData.emplace_back(ani1);
							aniData.emplace_back(ani2);

							ent.AddComponent<AnimationComponent>(aniData);
						}
					}
				}
			}
		});


	for (const auto& oldPData : oldPlayerData)
	{
		auto player = pScene->GetPlayer(oldPData.uIdx);
		auto& healthComp = player.GetComponent<HealthComponent>();
		healthComp.health = oldPData.health;

		auto& scoreComp = player.GetComponent<ScoreComponent>();
		scoreComp.score = oldPData.score;

		for (const auto& entity : healthComp.healthDisplayIds)
		{
			assert(entity != NULL_ENTITY_ID);
			Entity ent{ entity, pScene };

			assert(ent.HasComponent<TextComponent>());
			assert(ent.HasComponent<TxtDisplayComponent>());

			auto& textComp = ent.GetComponent<TextComponent>();
			const auto& displayComp = ent.GetComponent<TxtDisplayComponent>();

			textComp.SetText(displayComp.prefix + std::to_string(healthComp.health) + displayComp.postfix);
		}


		for (const auto& entity : scoreComp.scoreDisplays)
		{
			assert(entity != NULL_ENTITY_ID);
			Entity ent{ entity, pScene };

			assert(ent.HasComponent<TextComponent>());
			assert(ent.HasComponent<TxtDisplayComponent>());

			auto& textComp = ent.GetComponent<TextComponent>();
			const auto& displayComp = ent.GetComponent<TxtDisplayComponent>();

			textComp.SetText(displayComp.prefix + std::to_string(scoreComp.score) + displayComp.postfix);
		}
	}

	auto levelText = pScene->CreateEntity({ 500.f, 0.f, 0.f });
	levelText.AddComponent<TextComponent>("Lingua.otf", 26, "L" + std::to_string(static_cast<int>(level)));
	levelText.AddComponent<SpriteComponent>();

	m_LevelStartTime = std::chrono::steady_clock::now();
}

void Pengo::GameManager::LoadGameEndUI(bool wonGame)
{
	using namespace Pengin;
	
	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();
	
	std::vector<std::pair<GameUUID, unsigned>> oldScores;
	for (const auto& player : pActiveScene->GetSceneData().playerUUIDs)
	{
		auto p = pActiveScene->GetEntity(player);
		const auto& userIdx = p.GetComponent<PlayerComponent>().userIdx;
		const auto& scoreComp = p.GetComponent<ScoreComponent>();

		oldScores.emplace_back(userIdx, scoreComp.score);
	}

	SceneData sceneData{};
	sceneData.name = "Pengo EndScreen";

	SceneFileData data{};
	data.inputFilePath = "../Data/InputData.json";

	data.f_RegKeyboardInput = [this](const Pengin::InputData& data) { RegisterKeyboardInputGameEndUI(data); };
	data.f_RegControllerInput = [this](const Pengin::InputData& data) { RegisterControllerInputGameEndUI(data); };

	sceneData.sceneFileData = data;
	auto pScene = SceneManager::GetInstance().CreateScene(sceneData);

	auto ent = pScene->CreateEntity({ 50.f, 20.f, 0.f });
	if (wonGame)
	{
		ent.AddComponent<TextComponent>("Lingua.otf", 56, "Victory");
	}
	else
	{
		ent.AddComponent<TextComponent>("Lingua.otf", 56, "Game Over");
	}
	ent.AddComponent<SpriteComponent>();


	for (auto idx = 0; const auto& score : oldScores)
	{
		auto scoreDispl = pScene->CreateEntity({ 50.f, 100.f + idx * 50.f, 0.f });
		scoreDispl.AddComponent<TextComponent>("Lingua.otf", 36, "P" + std::to_string(idx) + " Score: " + std::to_string(score.second));
		scoreDispl.AddComponent<SpriteComponent>();

		auto player = pScene->CreateEntity();
		pScene->SetPlayer(score.first, player);
		auto& uiSelector = player.AddComponent<UISelectorComponent>();


		auto letter1 = pScene->CreateEntity({ 400.f, 100.f + idx * 50.f , 0.f });
		letter1.AddComponent<TextComponent>("Lingua.otf", 36, "A");
		letter1.AddComponent<SpriteComponent>();
		letter1.AddComponent<UILetterComponent>();
		auto letter2 = pScene->CreateEntity({ 425.f, 100.f + idx * 50.f , 0.f });
		letter2.AddComponent<TextComponent>("Lingua.otf", 36, "A");
		letter2.AddComponent<SpriteComponent>();
		letter2.AddComponent<UILetterComponent>();
		auto letter3 = pScene->CreateEntity({ 450.f, 100.f + idx * 50.f , 0.f });
		letter3.AddComponent<TextComponent>("Lingua.otf", 36, "A");
		letter3.AddComponent<SpriteComponent>();
		letter3.AddComponent<UILetterComponent>();

		uiSelector.score = score.second;

		uiSelector.letters.emplace_back(letter1.GetEntityId());
		uiSelector.letters.emplace_back(letter2.GetEntityId());
		uiSelector.letters.emplace_back(letter3.GetEntityId());

		++idx;
	}


	std::ifstream inFile("../Data/Highscores.txt");
	
	unsigned idx = 0;

	std::string line;
	while (std::getline(inFile, line))
	{
		++idx;

		if (idx == 11)
		{
			break;
		}

		std::istringstream iss(line);

		std::string name;
		unsigned score;

		iss >> name >> score;

		auto hs = pScene->CreateEntity({ 50.f, 300.f + idx * 40.f, 0.f });
		hs.AddComponent<TextComponent>("Lingua.otf", 36, std::to_string(idx) + "    " + name + "    " + std::to_string(score));
		hs.AddComponent<SpriteComponent>();
	}

	auto contText = pScene->CreateEntity({ 50.f, 750.f, 0.f });
	contText.AddComponent<TextComponent>("Lingua.otf", 48, "Press space to continue");
	contText.AddComponent<SpriteComponent>();
}

void Pengo::GameManager::SaveScores()
{
	using namespace Pengin;
	namespace fs = std::filesystem;

	struct PlayerScore
	{
		std::string name;
		unsigned score;

		bool operator<(const PlayerScore& other) const
		{
			if (score == other.score)
			{
				return name < other.name;
			}

			return score > other.score;
		}
	};

	std::vector<PlayerScore> scores;

	std::ifstream inFile("../Data/Highscores.txt");
	if (inFile.is_open())
	{
		std::string line;
		while (std::getline(inFile, line))
		{
			std::istringstream iss(line);
			std::string name;
			unsigned score;

			iss >> name >> score;

			scores.emplace_back(name, score);
		}

		inFile.close();
	}

	auto pScene = SceneManager::GetInstance().GetActiveScene();

	for (const auto& player : pScene->GetSceneData().playerUUIDs)
	{
		auto p = pScene->GetEntity(player);

		const auto& uiSel = p.GetComponent<UISelectorComponent>();

		std::string name;

		for (auto l : uiSel.letters)
		{
			name += Entity{ l, pScene }.GetComponent<UILetterComponent>().letter;
		}

		scores.emplace_back(name, uiSel.score);
	}

	std::sort(scores.begin(), scores.end());

	std::ofstream outFile("../Data/Highscores.txt");
	if (!outFile.is_open())
	{
		std::cerr << "Failed to open file for writing.\n";
		return;
	}

	for (const auto& playerScore : scores)
	{
		outFile << playerScore.name << " " << playerScore.score << "\n";
	}

	outFile.close();
}

void Pengo::GameManager::RegisterKeyboardInputLevel(const Pengin::InputData& inpData)
{
	using namespace Pengin;

	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapKeyboardAction(userIndex, KeyBoardKey::Left, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ -1, 0, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::Right, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 1, 0, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::Up, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, -1, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::Down, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, 1, 0 }));

	input.MapKeyboardAction(userIndex, KeyBoardKey::E, InputState::Pressed, std::make_shared<Pengo::BreakBlock>(userIndex));

	input.MapKeyboardAction(userIndex, KeyBoardKey::F1, InputState::UpThisFrame, std::make_shared<PengoSkipLevel>(userIndex));
	input.MapKeyboardAction(userIndex, KeyBoardKey::F2, InputState::DownThisFrame, std::make_shared<MuteSounds>(userIndex));

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


void Pengo::GameManager::RegisterControllerInputLevel(const Pengin::InputData& inpData)
{
	using namespace Pengin;

	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapControllerAction(userIndex, ControllerButton::DPadLeft, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ -1, 0, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadRight, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 1, 0, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadUp, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, -1, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadDown, InputState::Pressed, std::make_shared<Movement>(userIndex, glm::vec3{ 0, 1, 0 }));

	input.MapControllerAction(userIndex, ControllerButton::A, InputState::Pressed, std::make_shared<Pengo::BreakBlock>(userIndex));

	input.MapControllerAction(userIndex, ControllerButton::LeftShoulder, InputState::Pressed, std::make_shared<PengoSkipLevel>(userIndex));
	input.MapControllerAction(userIndex, ControllerButton::RightShoulder, InputState::Pressed, std::make_shared<MuteSounds>(userIndex));
}

void Pengo::GameManager::RegisterKeyboardInputUI(const Pengin::InputData& inpData)
{
	using namespace Pengin;

	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapKeyboardAction(userIndex, KeyBoardKey::SpaceBar, InputState::DownThisFrame, std::make_shared<PengoPlayGame>(userIndex));

	input.MapKeyboardAction(userIndex, KeyBoardKey::Up, InputState::DownThisFrame, std::make_shared<SelectGameMode>(userIndex, glm::vec2{0.f, -1.f}));
	input.MapKeyboardAction(userIndex, KeyBoardKey::Down, InputState::DownThisFrame, std::make_shared<SelectGameMode>(userIndex, glm::vec2{ 0.f, -1.f }));
}

void Pengo::GameManager::RegisterControllerInputUI(const Pengin::InputData& inpData)
{
	using namespace Pengin;

	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapControllerAction(userIndex, ControllerButton::A, InputState::Pressed, std::make_shared<PengoPlayGame>(userIndex));

	input.MapControllerAction(userIndex, ControllerButton::DPadUp, InputState::DownThisFrame, std::make_shared<SelectGameMode>(userIndex, glm::vec2{ 0.f, -1.f }));
	input.MapControllerAction(userIndex, ControllerButton::DPadDown, InputState::DownThisFrame, std::make_shared<SelectGameMode>(userIndex, glm::vec2{ 0.f, -1.f }));
}

void Pengo::GameManager::RegisterKeyboardInputGameEndUI(const Pengin::InputData& inpData)
{
	using namespace Pengin;

	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapKeyboardAction(userIndex, KeyBoardKey::Left, InputState::DownThisFrame, std::make_shared<SelectLetter>(userIndex, glm::vec3{ -1, 0, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::Right, InputState::DownThisFrame, std::make_shared<SelectLetter>(userIndex, glm::vec3{ 1, 0, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::Up, InputState::DownThisFrame, std::make_shared<SelectLetter>(userIndex, glm::vec3{ 0, -1, 0 }));
	input.MapKeyboardAction(userIndex, KeyBoardKey::Down, InputState::DownThisFrame, std::make_shared<SelectLetter>(userIndex, glm::vec3{ 0, 1, 0 }));

	input.MapKeyboardAction(userIndex, KeyBoardKey::SpaceBar, InputState::DownThisFrame, std::make_shared<Continue>(userIndex));
}

void Pengo::GameManager::RegisterControllerInputGameEndUI(const Pengin::InputData& inpData)
{
	using namespace Pengin;

	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapControllerAction(userIndex, ControllerButton::DPadDown, InputState::DownThisFrame, std::make_shared<SelectLetter>(userIndex, glm::vec3{ -1, 0, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadRight, InputState::DownThisFrame, std::make_shared<SelectLetter>(userIndex, glm::vec3{ 1, 0, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadUp, InputState::DownThisFrame, std::make_shared<SelectLetter>(userIndex, glm::vec3{ 0, -1, 0 }));
	input.MapControllerAction(userIndex, ControllerButton::DPadDown, InputState::DownThisFrame, std::make_shared<SelectLetter>(userIndex, glm::vec3{ 0, 1, 0 }));

	input.MapControllerAction(userIndex, ControllerButton::A, InputState::DownThisFrame, std::make_shared<Continue>(userIndex));
}