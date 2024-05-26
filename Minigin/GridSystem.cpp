#include "GridSystem.h"

#include "TransformComponent.h"
#include "GridComponent.h"
#include "OnGridTag.h"

#include "UtilStructs.h"
#include <cassert>

#include "ECS.h"

namespace Pengin
{
	std::pair<uint16_t, uint16_t> GridSystem::GetCellCoords(EntityId entityId) const noexcept
	{
		assert(m_ECS.HasComponent<OnGridTag>(entityId));
		const auto& gridTag = m_ECS.GetComponent<OnGridTag>(entityId);

		assert(m_ECS.HasComponent<GridComponent>(gridTag.gridId));
		assert(IsEntityInGridArea(entityId, gridTag.gridId));

		const auto& gridComp = m_ECS.GetComponent<GridComponent>(gridTag.gridId);
		
		const auto& entityTransform = m_ECS.GetComponent<TransformComponent>(entityId);
		const auto& gridTransform = m_ECS.GetComponent<TransformComponent>(gridTag.gridId);


		const float posX = (entityTransform.worldPos.x - gridTransform.worldPos.x) / gridTransform.scale.x;
		const float posY = (entityTransform.worldPos.y - gridTransform.worldPos.y) / gridTransform.scale.y;

		assert(posX >= 0.f - UtilFuncs::EPSILON && posY >= 0.f - UtilFuncs::EPSILON);

		return gridComp.GetCellCoords(static_cast<uint32_t>(posX), static_cast<uint32_t>(posY) );
	}
	bool GridSystem::IsEntityInGridArea(EntityId entityId, EntityId gridId) const noexcept
	{
		const auto& entityTransform = m_ECS.GetComponent<TransformComponent>(entityId);

		const auto& gridComp = m_ECS.GetComponent<GridComponent>(gridId);
		const auto& gridTransform = m_ECS.GetComponent<TransformComponent>(gridId);

		return UtilFuncs::IsPointInRect(UtilStructs::Recti{ static_cast<int>(gridTransform.worldPos.x), 
															static_cast<int>(gridTransform.worldPos.y), 
															static_cast<int>(gridComp.cellWidth * gridComp.cols), 
															static_cast<int>(gridComp.cellHeight * gridComp.rows) },
			static_cast<int>(entityTransform.worldPos.x), static_cast<int>(entityTransform.worldPos.y));
	}
}


