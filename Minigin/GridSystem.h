#ifndef PENGIN_GRIDSYSTEM
#define PENGIN_GRIDSYSTEM

#include "BaseSystem.h"

#include "EntityId.h"

#include <unordered_map>

namespace Pengin
{
	class ECS;

	class GridSystem final : public BaseSystem
	{
	public:
		GridSystem(ECS& ecs) :
			BaseSystem{ },
			m_ECS{ ecs }
		{}
		virtual ~GridSystem() override = default;
		
		[[nodiscard]] std::pair<uint16_t, uint16_t> GetCellCoords(EntityId entityId) const noexcept;
		[[nodiscard]] bool IsEntityInGridArea(EntityId entityId, EntityId gridId) const noexcept;

		GridSystem(const GridSystem&) = delete;
		GridSystem(GridSystem&&) = delete;
		GridSystem& operator=(const GridSystem&) = delete;
		GridSystem& operator=(const GridSystem&&) = delete;

	private:
		ECS& m_ECS;

		//Checkcollision(id)
		//ecs.getComp<OnGridTag>() 
		//ecs.getComp<GridComp>(onGridTag.gridId)

		//grid destroy - check all on grid tag-..;
		//OnGridDestroy Event 

		//MoveEntity(id, vec2{})
		//....
	};
}

#endif
