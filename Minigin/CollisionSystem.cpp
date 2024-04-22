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

				UtilStructs::Rect16 outerRect{ outerEntity.collRect };
				outerRect.x = static_cast<int16_t>(outerTrans.worldPos.x) + outerRect.x * static_cast<int16_t>(outerTrans.scale.x);
				outerRect.y = static_cast<int16_t>(outerTrans.worldPos.y) + outerRect.y * static_cast<int16_t>(outerTrans.scale.y);

				outerRect.width *= static_cast<int16_t>(outerTrans.scale.x);
				outerRect.height *= static_cast<int16_t>(outerTrans.scale.y);

				UtilStructs::Rect16 innerRect{ innerEntity.collRect };
				innerRect.x = static_cast<int16_t>(innerTrans.worldPos.x) + innerRect.x * static_cast<int16_t>(innerTrans.scale.x);
				innerRect.y = static_cast<int16_t>(innerTrans.worldPos.y) + innerRect.y * static_cast<int16_t>(innerTrans.scale.y);

				innerRect.width += static_cast<int16_t>(innerTrans.worldPos.x);
				innerRect.height += static_cast<int16_t>(innerTrans.worldPos.y);

				if (UtilFuncs::IsCollidingABBA(outerRect, innerRect))
				{
					//Can broadcast event, ...
					std::cout << "COLLISION: entity id "  << rectCollComps.GetIdFromIterator(outerIt) << " on entity id " << rectCollComps.GetIdFromIterator(innerIt) << "\n";
				}

				++innerIt;
			}

			++outerIt;
		}
	}
}
