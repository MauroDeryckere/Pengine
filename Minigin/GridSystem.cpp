#include "GridSystem.h"

#include "TransformComponent.h"
#include "GridComponent.h"
#include "OnGridTag.h"

#include "UtilStructs.h"
#include <cassert>

#include "ECS.h"

namespace Pengin
{
	glm::vec2 GridSystem::GetCellPos(EntityId gridId, uint16_t row, uint16_t col) const noexcept
	{
		assert(m_ECS.Exists(gridId));
		assert(m_ECS.HasComponent<GridComponent>(gridId));

		const auto& gridTransform = m_ECS.GetComponent<TransformComponent>(gridId);
		const auto cellPos = m_ECS.GetComponent<GridComponent>(gridId).GetCellPosition(row, col);

		return { cellPos.first * gridTransform.scale.x + gridTransform.worldPos.x, 
				 cellPos.second * gridTransform.scale.y + gridTransform.worldPos.y };
	}

	std::pair<uint16_t, uint16_t> GridSystem::GetCellCoords(EntityId entityId) const noexcept
	{
		assert(m_ECS.Exists(entityId));

		assert(m_ECS.HasComponent<OnGridTag>(entityId));
		const auto& gridTag = m_ECS.GetComponent<OnGridTag>(entityId);

		assert(m_ECS.HasComponent<GridComponent>(gridTag.gridId));
		assert(m_ECS.Exists(gridTag.gridId));

		assert(IsEntityInGridArea(entityId, gridTag.gridId));

		const auto& gridComp = m_ECS.GetComponent<GridComponent>(gridTag.gridId);
		
		const auto& entityTransform = m_ECS.GetComponent<TransformComponent>(entityId);
		const auto& gridTransform = m_ECS.GetComponent<TransformComponent>(gridTag.gridId);

		const float posX = (entityTransform.worldPos.x - gridTransform.worldPos.x) / gridTransform.scale.x;
		const float posY = (entityTransform.worldPos.y - gridTransform.worldPos.y) / gridTransform.scale.y;

		return gridComp.GetCellCoords(static_cast<uint32_t>(posX), static_cast<uint32_t>(posY));
	}

	std::pair<uint16_t, uint16_t> GridSystem::GetCellCoords(EntityId gridId, EntityId entityId) const noexcept
	{
		assert(m_ECS.Exists(entityId));
		assert(m_ECS.Exists(gridId));

		assert(m_ECS.HasComponent<GridComponent>(gridId));
		assert(IsEntityInGridArea(entityId, gridId));

		const auto& gridComp = m_ECS.GetComponent<GridComponent>(gridId);

		const auto& entityTransform = m_ECS.GetComponent<TransformComponent>(entityId);
		const auto& gridTransform = m_ECS.GetComponent<TransformComponent>(gridId);

		const float posX = (entityTransform.worldPos.x - gridTransform.worldPos.x) / gridTransform.scale.x;
		const float posY = (entityTransform.worldPos.y - gridTransform.worldPos.y) / gridTransform.scale.y;

		return gridComp.GetCellCoords(static_cast<uint32_t>(posX), static_cast<uint32_t>(posY));
	}

	std::pair<uint16_t, uint16_t> GridSystem::GetCellCoords(EntityId gridId, const glm::vec3& position) const noexcept
	{
		assert(m_ECS.Exists(gridId));

		assert(m_ECS.HasComponent<GridComponent>(gridId));

		const auto& gridComp = m_ECS.GetComponent<GridComponent>(gridId);

		const auto& gridTransform = m_ECS.GetComponent<TransformComponent>(gridId);

		const float posX = (position.x - gridTransform.worldPos.x) / gridTransform.scale.x;
		const float posY = (position.y - gridTransform.worldPos.y) / gridTransform.scale.y;

		return gridComp.GetCellCoords(static_cast<uint32_t>(posX), static_cast<uint32_t>(posY));
	}

	uint16_t GridSystem::GetRow(EntityId gridId, EntityId entityId) const noexcept
	{
		assert(m_ECS.Exists(entityId));
		assert(m_ECS.Exists(gridId));

		assert(m_ECS.HasComponent<GridComponent>(gridId));

		const auto& gridComp = m_ECS.GetComponent<GridComponent>(gridId);

		const auto& entityTransform = m_ECS.GetComponent<TransformComponent>(entityId);
		const auto& gridTransform = m_ECS.GetComponent<TransformComponent>(gridId);

		const float posY = (entityTransform.worldPos.y - gridTransform.worldPos.y) / gridTransform.scale.y;

		assert(posY >= 0.f);
		assert(static_cast<uint32_t>(posY) < gridComp.GetTotalHeight());

		return static_cast<uint16_t>(posY / gridComp.cellHeight);
	}

	uint16_t GridSystem::GetCol(EntityId gridId, EntityId entityId) const noexcept
	{
		assert(m_ECS.Exists(entityId));
		assert(m_ECS.Exists(gridId));

		assert(m_ECS.HasComponent<GridComponent>(gridId));

		const auto& gridComp = m_ECS.GetComponent<GridComponent>(gridId);

		const auto& entityTransform = m_ECS.GetComponent<TransformComponent>(entityId);
		const auto& gridTransform = m_ECS.GetComponent<TransformComponent>(gridId);

		const float posX = (entityTransform.worldPos.x - gridTransform.worldPos.x) / gridTransform.scale.x;

		assert(posX >= 0.f);
		assert(static_cast<uint32_t>(posX) < gridComp.GetTotalWidth());

		return static_cast<uint16_t>(posX / gridComp.cellWidth);
	}

	bool GridSystem::IsPosInGridArea(const glm::vec3& position, EntityId gridId) const noexcept
	{
		assert(m_ECS.Exists(gridId));
		assert(m_ECS.HasComponent<GridComponent>(gridId));

		const auto& gridComp = m_ECS.GetComponent<GridComponent>(gridId);
		const auto& gridTransform = m_ECS.GetComponent<TransformComponent>(gridId);

		return UtilFuncs::IsPointInRect(UtilStructs::Rectf{ (gridTransform.worldPos.x - 1.f),
															(gridTransform.worldPos.y - 1.f),
															(gridComp.cellWidth * gridComp.cols * gridTransform.scale.x + 2.f),
															(gridComp.cellHeight * gridComp.rows * gridTransform.scale.y + 2.f) },
									   (position.x), (position.y));
	}

	bool GridSystem::IsEntityInGridArea(EntityId entityId, EntityId gridId) const noexcept
	{
		assert(m_ECS.Exists(entityId));
		assert(m_ECS.Exists(gridId));

		const auto& entityTransform = m_ECS.GetComponent<TransformComponent>(entityId);

		assert(m_ECS.HasComponent<GridComponent>(gridId));
		const auto& gridComp = m_ECS.GetComponent<GridComponent>(gridId);
		const auto& gridTransform = m_ECS.GetComponent<TransformComponent>(gridId);

		return UtilFuncs::IsPointInRect(UtilStructs::Rectf{ (gridTransform.worldPos.x - 1.f), 
															(gridTransform.worldPos.y - 1.f), 
															(gridComp.cellWidth * gridComp.cols * gridTransform.scale.x + 1.f), 
															(gridComp.cellHeight * gridComp.rows * gridTransform.scale.y + 1.f) },
			(entityTransform.worldPos.x), (entityTransform.worldPos.y));
	}

	bool GridSystem::IsWithinBounds(EntityId gridId, uint16_t row, uint16_t col) const noexcept
	{
		assert(m_ECS.Exists(gridId));
		assert(m_ECS.HasComponent<GridComponent>(gridId));

		return m_ECS.GetComponent<GridComponent>(gridId).IsWithinBounds(row, col);
	}

	const GridCellData& GridSystem::GetCellData(EntityId gridId, uint16_t row, uint16_t col) const noexcept
	{	
		assert(m_ECS.HasComponent<GridComponent>(gridId));
		const auto& gridComp = m_ECS.GetComponent<GridComponent>(gridId);

		return gridComp.At(row, col);
	}

	GridCellData& GridSystem::GetCellData(EntityId gridId, uint16_t row, uint16_t col) noexcept
	{
		assert(m_ECS.HasComponent<GridComponent>(gridId));
		auto& gridComp = m_ECS.GetComponent<GridComponent>(gridId);

		return gridComp.At(row, col);
	}
}


