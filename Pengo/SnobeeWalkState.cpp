#include "SnobeeWalkState.h"

#include "SwitchAnimationEvent.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "UtilStructs.h"
#include "EnemySystem.h"
#include "Entity.h"
#include "BodyComponent.h"
#include "SnobeeComponent.h"
#include "EnemySystem.h"
#include "OnGridTag.h"
#include "BlockSystem.h"
#include "BlockComponent.h"
#include "GridSystem.h"
#include "PengoGrid.h"

#include <array>

void Pengo::SnobeeWalkState::OnEnter()
{
	using namespace Pengin;

	if (m_Direction.x > 0.f)
	{
		EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
			GetEntityId(),
			static_cast<uint8_t>(EnemySystem::EnemyAnimations::WalkRight)));
	}
	else if (m_Direction.x < 0.f)
	{
		EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
			GetEntityId(),
			static_cast<uint8_t>(EnemySystem::EnemyAnimations::WalkLeft)));
	}
	else if (m_Direction.y > 0.f)
	{
		EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
			GetEntityId(),
			static_cast<uint8_t>(EnemySystem::EnemyAnimations::WalkDown)));
	}
	else if (m_Direction.y < 0.f)
	{
		EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
			GetEntityId(),
			static_cast<uint8_t>(EnemySystem::EnemyAnimations::WalkUp)));
	}
}

std::unique_ptr<Pengin::State> Pengo::SnobeeWalkState::Update()
{
	using namespace Pengin;

	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();
	auto pGridSystem = pActiveScene->GetSystem<GridSystem>();

	Entity snobee{ GetEntityId(), pActiveScene.get() };
	const EntityId gridId = snobee.GetComponent<OnGridTag>().gridId;

	const auto vel = m_Direction * snobee.GetComponent<SnobeeComponent>().movementSpeed;
	
	auto& body = snobee.GetComponent<BodyComponent>();

	auto predPos = body.currentPosition + glm::vec3{ vel.x, vel.y, 0.f };

	if (m_Direction.y < 0.f)
	{
		predPos.y += 48.f;
	}
	else if (m_Direction.x < 0.f)
	{
		predPos.x += 48.f;
	}

	if (!pGridSystem->IsPosInGridArea(predPos, gridId))
	{
		const auto coords = pGridSystem->GetCellCoords(gridId, GetEntityId());
		const auto cellPos = pGridSystem->GetCellPos(gridId, coords.first, coords.second);

		snobee.SetLocalPosition({ cellPos.x, cellPos.y, 0 });

		return std::make_unique<SnobeeWalkState>(GetEntityId());
	}
	else
	{
		const auto predCoords = pGridSystem->GetCellCoords(gridId, predPos);

		auto& predCellData = pGridSystem->GetCellData(gridId, predCoords.first, predCoords.second);

		if (predCellData.type == static_cast<uint8_t>(PengoCellType::Block) )
		{
			Entity block{ predCellData.entity, pActiveScene.get() };

			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
				predCellData.entity,
				static_cast<uint8_t>(BlockSystem::BlockAnimations::Breaking)));

			auto& blockComp = block.GetComponent<BlockComponent>();
			blockComp.blockState = BlockComponent::BlockState::Breaking;

			predCellData.Reset();
		}
	}

	body.inputVelocity += glm::vec3{vel.x, vel.y, 0.f };

	return nullptr;
}

glm::vec2 Pengo::SnobeeWalkState::CalcDirection()
{
	using namespace Pengin;

	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();

	Entity snobee{ GetEntityId(), pActiveScene.get()};
	const EntityId gridId = snobee.GetComponent<OnGridTag>().gridId;

	auto pGridSystem = pActiveScene->GetSystem<GridSystem>();
	const auto coords = pGridSystem->GetCellCoords(gridId, GetEntityId());

	static constexpr std::array<std::pair<int, int>, 4> dirs
	{
		std::pair<int, int>{1, 0},
		std::pair<int, int>{0, 1},
		std::pair<int, int>{-1, 0},
		std::pair<int, int>{0, -1}
	};

	std::vector<std::pair<int, int>> avDirs{};

	for (auto& dir : dirs)
	{
		const int row = coords.first + dir.first;
		const int col = coords.second + dir.second;
		
		if (row < 0 ||  col < 0)
		{
			continue;
		}

		if (!pGridSystem->IsWithinBounds(gridId, static_cast<uint16_t>(row), static_cast<uint16_t>(col)))
		{
			continue;
		}

		avDirs.emplace_back(dir);
	}

	const auto dir = UtilFuncs::RandomNumber<size_t>(0, avDirs.size()); dir;
	return glm::vec2{ avDirs[dir].second, avDirs[dir].first };
}
