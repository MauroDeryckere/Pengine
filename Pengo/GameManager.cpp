#include "GameManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "PengoInputCommands.h"
#include "PengoGrid.h"
#include "AnimationComponent.h"
#include "TagComponent.h"
#include "InputCommands.h"
#include "OnGridTag.h"
#include "BlockComponent.h"
#include "SpriteComponent.h"
#include "GridSystem.h"
#include "UIDisplaySystem.h"
#include "WallSystem.h"
#include "BlockSystem.h"
#include "EnemySystem.h"
#include "PlayerSystem.h"
#include "GameManager.h"


void Pengo::GameManager::LoadUI()
{
	using namespace Pengin;

	SceneData sceneData{};
	sceneData.name = "Pengo UI";

	SceneFileData data{};
	data.inputFilePath = "../Data/InputTest.json";

	data.f_RegKeyboardInput = [this](const Pengin::InputData& data) { RegisterKeyboardInputUI(data); };
	data.f_RegControllerInput = [this](const Pengin::InputData& data) { RegisterControllerInputUI(data); };

	sceneData.sceneFileData = data;

	auto pScene = SceneManager::GetInstance().CreateScene(sceneData);

	auto ent = pScene->CreateEntity();
	ent.AddComponent<TextComponent>("Lingua.otf", 56, "Press spacebar to play");
	ent.AddComponent<SpriteComponent>();
}

void Pengo::GameManager::LoadLevel1()
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
	data.f_RegControllerInput = [this](const Pengin::InputData& data) { RegisterControllerInputLevel(data); };
	data.f_RegKeyboardInput = [this](const Pengin::InputData& data) { RegisterKeyboardInputLevel(data); };
	data.keepPrevInput = false; //boolean to override any funcs

	sceneData.sceneFileData = data;

	ServiceLocator::GetSoundSystem().LoadSoundsFromFolder("../Data/Audio");
	auto pScene = SceneManager::GetInstance().CreateScene(sceneData, true);

	pScene->RegisterSystems([&](SystemManager& sysManager, ECS& ecs)
		{
			sysManager.RegisterSystem<Pengo::PlayerSystem>(std::make_shared<Pengo::PlayerSystem>(ecs));
			sysManager.RegisterSystem<Pengo::EnemySystem>(std::make_shared<Pengo::EnemySystem>(ecs));
			sysManager.RegisterSystem<Pengo::BlockSystem>(std::make_shared<Pengo::BlockSystem>(ecs));
			sysManager.RegisterSystem<Pengo::WallSystem>(std::make_shared<Pengo::WallSystem>(ecs));

			sysManager.RegisterSystem<Pengo::UIDisplaySystem>(std::make_shared<Pengo::UIDisplaySystem>(ecs, pScene.get()));
		});


	auto grid = pScene->GetEntity(GameUUID{ std::string{"cf0f18cb-7425-460b-9221-999f8160d448"} });
	auto& gridComp = grid.GetComponent<GridComponent>();

	std::vector<uint8_t> cells
	{
		0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0,
		0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
		0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
		0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
		0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0,
		0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0,
		0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0,
		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0,
		0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0,
		0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0,
		0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0
	};

	constexpr float SCALE{ 3.f };


	uint16_t row = 0;
	uint16_t col = 0;

	for (int idx{ 0 }; const auto & cell : cells)
	{
		if (cell == uint8_t{ 1 })
		{
			auto ent = pScene->CreatePhysicsEntity(UtilStructs::Rectu16{ 1, 1, 14, 14 },
				{ 8.f * SCALE + (col * 16.f * SCALE), 72.f + 8.f * SCALE + (row * 16.f * SCALE), 0.f },
				{ },
				{ SCALE, SCALE, 0.f });

			auto& cellData = gridComp.At(row, col);
			cellData.type = static_cast<uint8_t>(Pengo::PengoCellType::Block);
			cellData.entity = ent.GetEntityId();

			ent.AddComponent<TagComponent>("Block Entity");
			ent.AddComponent<Pengo::BlockComponent>();
			ent.GetComponent<BodyComponent>().collType = CollType::Static;

			ent.AddComponent<SpriteComponent>("PengoFieldAndBlocks.png", UtilStructs::Rectu16{ 708, 0, 16, 16 });

			std::vector<AnimationData> aniData;

			AnimationData ani1;
			ani1.frame0sourceRect = UtilStructs::Rectu16{ 708, 0, 16, 16 };
			ani1.frameDuration = 0.f;
			ani1.frameCt = 1;

			AnimationData ani2;
			ani2.frame0sourceRect = UtilStructs::Rectu16{ 708, 48, 16, 16 };
			ani2.frameDuration = 0.125f;
			ani2.frameCt = 8;

			aniData.emplace_back(ani1);
			aniData.emplace_back(ani2);

			ent.AddComponent<AnimationComponent>(aniData);
		}

		++idx;
		++col;

		if (idx % 13 == 0)
		{
			col = 0;
			++row;
		}
	}
}

void Pengo::GameManager::PlayGame()
{
	if (!playing)
	{
		playing = true;
		Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("LoadLevel"));
	}
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

	input.MapControllerAction(userIndex, ControllerButton::A, InputState::UpThisFrame, std::make_shared<AttackPlayer>(userIndex));
	input.MapControllerAction(userIndex, ControllerButton::B, InputState::Pressed, std::make_shared<CollectScore>(userIndex));
	input.MapControllerAction(userIndex, ControllerButton::X, InputState::DownThisFrame, std::make_shared<MakeSound>(userIndex, SoundData{ "../Data/Audio/Act Start.mp3" }));
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
	inpData;
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