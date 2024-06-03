#include "BlockSystem.h"

#include "BodyComponent.h"
#include "SceneManager.h"
#include "BlockComponent.h"
#include "GridSystem.h"
#include "OnGridTag.h"
#include "ECS.h"
#include "PengoGrid.h"
#include "Entity.h"
#include "GameTime.h"
#include "WallComponent.h"
#include "GridComponent.h"

void Pengo::BlockSystem::Update()
{
	using namespace Pengin;

	constexpr float BLOCK_MOVEMENTSPEED{ 200.f };

	auto blocks = m_ECS.GetComponents<BlockComponent>();
	for (auto it = std::begin(blocks); auto& block : blocks)
	{
		if (block.blockState == BlockComponent::BlockState::Moving)
		{
			const auto inptvel{ BLOCK_MOVEMENTSPEED * block.dir };
			m_ECS.GetComponent<BodyComponent>(blocks.GetIdFromIterator(it)).inputVelocity += glm::vec3{inptvel.x, inptvel.y, 0.f};
		}

		else if (block.blockState == BlockComponent::BlockState::Breaking)
		{
			block.breakingTimer += GameTime::GetInstance().ElapsedSec();

			if (block.breakingTimer >= 1.f)
			{
				m_ECS.DestroyEntity(blocks.GetIdFromIterator(it));
			}
		}

		++it;
	}
}

void Pengo::BlockSystem::OnBlockBreakEvent(const Pengin::BaseEvent& event)
{
	using namespace Pengin;

	const auto& breakEv{ static_cast<const PengoBlockBreakEvent&>(event) };
	breakEv;
}

void Pengo::BlockSystem::OnCollision(const Pengin::BaseEvent& event)
{
	using namespace Pengin;

	const auto& collEv{ static_cast<const CollisionEvent&>(event) };

	const auto entA = collEv.GetEntityA();
	const auto entB = collEv.GetEntityB();

	if (m_ECS.HasComponent<BlockComponent>(entB) && m_ECS.HasComponent<BlockComponent>(entA))
	{
		auto pActiveScene = SceneManager::GetInstance().GetActiveScene();

		GridSystem* pGridSys = pActiveScene->GetSystem<GridSystem>();
		assert(pGridSys);

		Entity movingBlock
		{ 
			collEv.GetCollTypeA() == CollType::Dynamic ? entA : entB, 
			pActiveScene
		};

		Entity staticBlock
		{
			collEv.GetCollTypeA() == CollType::Static ? entA : entB,
			pActiveScene
		};

		auto& dynBlockComp = movingBlock.GetComponent<BlockComponent>();

		const auto& gridTag = movingBlock.GetComponent<OnGridTag>();
		const auto staticBlockCoords = pGridSys->GetCellCoords(gridTag.gridId, staticBlock.GetEntityId());

		const auto pos = pGridSys->GetCellPos(gridTag.gridId, 
											  static_cast<uint16_t>(staticBlockCoords.first - dynBlockComp.dir.y), 
											  static_cast<uint16_t>(staticBlockCoords.second - dynBlockComp.dir.x));

		auto& cellData = pGridSys->GetCellData(gridTag.gridId, 
											   static_cast<uint16_t>(staticBlockCoords.first - dynBlockComp.dir.y), 
											   static_cast<uint16_t>(staticBlockCoords.second - dynBlockComp.dir.x));
		
		assert(cellData.entity == NULL_ENTITY_ID);
		
		dynBlockComp.dir = {};
		dynBlockComp.blockState = BlockComponent::BlockState::Still;

		movingBlock.SetLocalPosition({ pos.x, pos.y, 0.f });

		cellData.type = static_cast<uint8_t>(PengoCellType::Block);
		cellData.entity = movingBlock.GetEntityId();

		movingBlock.RemoveComponent<OnGridTag>();
		movingBlock.GetComponent<BodyComponent>().collType = CollType::Static;

		return;
	}

	EntityId blockEntity{};

	if (m_ECS.HasComponent<BlockComponent>(entB) && m_ECS.HasComponent<WallComponent>(entA))
	{
		blockEntity = entB;
	}
	else if (m_ECS.HasComponent<BlockComponent>(entA) && m_ECS.HasComponent<WallComponent>(entB))
	{
		blockEntity = entA;
	}
	else
	{
		return;
	}
	auto& blockComp = m_ECS.GetComponent<BlockComponent>(blockEntity);

	if (blockComp.blockState != BlockComponent::BlockState::Moving)
	{
		return;
	}

	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();
	GridSystem* pGridSys = pActiveScene->GetSystem<GridSystem>();
	assert(pGridSys);

	Entity block
	{
		blockEntity,
		pActiveScene
	};

	const auto& gridTag = m_ECS.GetComponent<OnGridTag>(blockEntity);
	auto& gridComp = m_ECS.GetComponent<GridComponent>(gridTag.gridId);
	assert(blockComp.dir != glm::vec2{});
	
	uint16_t row{ };
	uint16_t col{ };

	if (blockComp.dir.x > 0)
	{
		row = pGridSys->GetRow(gridTag.gridId, blockEntity);
		col = gridComp.cols - 1;
	}
	else if (blockComp.dir.x < 0)
	{
		row = pGridSys->GetRow(gridTag.gridId, blockEntity);
		col = 0;
	}
	else if (blockComp.dir.y > 0)
	{
		col = pGridSys->GetCol(gridTag.gridId, blockEntity);
		row = gridComp.rows - 1;
	}
	else if (blockComp.dir.y < 0)
	{
		col = pGridSys->GetCol(gridTag.gridId, blockEntity);
		row = 0;
	}

	const auto pos = pGridSys->GetCellPos(gridTag.gridId, row, col);

	block.SetLocalPosition({ pos.x, pos.y, 0.f });
	block.SetWorldPosition({ pos.x, pos.y, 0.f });

	auto& cellData = gridComp.At(row, col);

	assert(cellData.entity == NULL_ENTITY_ID);

	cellData.entity = blockEntity;
	cellData.type = static_cast<uint8_t>(PengoCellType::Block);

	blockComp.dir = {  };
	blockComp.blockState = BlockComponent::BlockState::Still;

	block.GetComponent<BodyComponent>().collType = CollType::Static;

	block.RemoveComponent<OnGridTag>();
}