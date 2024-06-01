#include "BlockSystem.h"

#include "BodyComponent.h"
#include "SceneManager.h"
#include "BlockComponent.h"
#include "GridSystem.h"
#include "OnGridTag.h"
#include "ECS.h"
#include "PengoGrid.h"
#include "Entity.h"

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

		++it;
	}
}

void Pengo::BlockSystem::OnBlockBreakEvent(const Pengin::BaseEvent& event)
{
	using namespace Pengin;

	const auto& breakEv{ static_cast<const PengoBlockBreakEvent&>(event) };

	m_ECS.DestroyEntity(breakEv.GetBlockId());
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
			pActiveScene.get() 
		};

		Entity staticBlock
		{
			collEv.GetCollTypeA() == CollType::Static ? entA : entB,
			pActiveScene.get()
		};

		movingBlock.GetComponent<BodyComponent>().collType = CollType::Static;

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

		movingBlock.SetLocalPosition({ pos.x, pos.y, 0.f });
		movingBlock.SetWorldPosition({ pos.x, pos.y, 0.f });

		dynBlockComp.dir = {};
		dynBlockComp.blockState = BlockComponent::BlockState::Still;

		cellData.type = static_cast<uint8_t>(PengoCellType::Block);
		cellData.entity = movingBlock.GetEntityId();

		movingBlock.RemoveComponent<OnGridTag>();
	}
}