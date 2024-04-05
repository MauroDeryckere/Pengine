#include "CollisionSystem.h"

#include "ECS.h"

#include "TransformComponent.h"
#include "RectColliderComponent.h"

#include <iostream>

namespace Pengin
{
	void CollisionSystem::Update() //Only supporting rect collider right now
	{
		auto rectCollComps = m_ECS.GetComponents<RectColliderComponent>();

		for (auto outerIt{ rectCollComps.begin()}; const auto& outerEntity : rectCollComps)
		{
			for (auto innerIt{ rectCollComps.begin() }; const auto& innerEntity : rectCollComps)
			{
				if (innerIt == outerIt) //Can not collide with self
				{
					++innerIt;
					continue;
				}

				const auto& outerTrans{ m_ECS.GetComponent<TransformComponent>(rectCollComps.GetIdFromIterator(outerIt)) };
				const auto& innerTrans{ m_ECS.GetComponent<TransformComponent>(rectCollComps.GetIdFromIterator(innerIt)) };

				//TODO scale 

				UtilStructs::Rectu16 outerRect{ outerEntity.m_CollRect };
				outerRect.x += static_cast<uint16_t>(outerTrans.worldPos.x);
				outerRect.y += static_cast<uint16_t>(outerTrans.worldPos.y);

				UtilStructs::Rectu16 innerRect{ innerEntity.m_CollRect };
				innerRect.x += static_cast<uint16_t>(innerTrans.worldPos.x);
				innerRect.y += static_cast<uint16_t>(innerTrans.worldPos.y);

				if (IsCollidingABBA(outerRect, innerRect))
				{
					//Can broadcast event, ...
					std::cout << "COLLISION: entity id "  << rectCollComps.GetIdFromIterator(outerIt) << " on entity id " << rectCollComps.GetIdFromIterator(innerIt) << "\n";
				}

				++innerIt;
			}

			++outerIt;
		}
	}
	inline constexpr bool CollisionSystem::IsCollidingABBA(UtilStructs::Rectu16 rect1, UtilStructs::Rectu16 rect2) const noexcept
	{
		if ((rect1.x + rect1.width) < rect2.x || (rect2.x + rect2.width) < rect1.x)
		{
			return false;
		}

		if (rect1.y > (rect2.y + rect2.height) || rect2.y > (rect1.y + rect1.height))
		{
			return false;
		}

		return true;
	}
}
