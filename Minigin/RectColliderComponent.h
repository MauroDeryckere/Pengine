#ifndef RECTCOLLIDERCOMPONENT
#define RECTCOLLIDERCOMPONENT

#include "UtilStructs.h"

#include "SerializationRegistry.h"

namespace Pengin
{
	struct RectColliderComponent final
	{
		RectColliderComponent(const UtilStructs::Rectu16 collRect = UtilStructs::Rectu16{0 ,0, 1, 1}) :
			collRect{collRect}
		{
			assert(collRect.width > 0 && collRect.height > 0);
		}
		~RectColliderComponent() = default;

		UtilStructs::Rectu16 collRect;

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<RectColliderComponent>(id);

			fieldSer.SerializeField("CollRect", std::vector<uint16_t>{comp.collRect.x, comp.collRect.y, comp.collRect.width, comp.collRect.height }, fieldVector);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(RectColliderComponent, RectColliderComponent::Serialize);
}

#endif