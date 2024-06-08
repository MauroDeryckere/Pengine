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

#include <iostream>

void Pengo::GameManager::LoadUI()
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
}

void Pengo::GameManager::RegisterKeyboardInputUI(const Pengin::InputData& inpData)
{
	using namespace Pengin;

	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapKeyboardAction(userIndex, KeyBoardKey::SpaceBar, InputState::Pressed, std::make_shared<PengoPlayGame>(userIndex));
}

void Pengo::GameManager::RegisterControllerInputUI(const Pengin::InputData& inpData)
{
	using namespace Pengin;

	auto& input = InputManager::GetInstance();

	const auto& userIndex = std::get<0>(inpData);
	assert(userIndex);

	input.MapControllerAction(userIndex, ControllerButton::A, InputState::Pressed, std::make_shared<PengoPlayGame>(userIndex));
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
