#ifndef RECTCOLLIDERCOMPONENT
#define RECTCOLLIDERCOMPONENT

#include "UtilStructs.h"

#include "SerializationRegistry.h"

namespace Pengin
{
	struct RectColliderComponent final
	{
		UtilStructs::Rectu16 collRect;

		RectColliderComponent(const UtilStructs::Rectu16 collRect = UtilStructs::Rectu16{ 0 ,0, 1, 1 }) :
			collRect{ collRect }
		{
			assert(collRect.width > 0 && collRect.height > 0);
		}
		~RectColliderComponent() = default;

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<RectColliderComponent>(id);

			fieldSer.SerializeField("CollRect", std::vector<uint16_t>{comp.collRect.x, comp.collRect.y, comp.collRect.width, comp.collRect.height }, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			std::vector<uint16_t> rectVec{};
			fieldSer.DeserializeField("CollRect", rectVec, serializedFields);

			ecs.AddComponent<RectColliderComponent>(id, UtilStructs::Rectu16{ rectVec[0], rectVec[1], rectVec[2], rectVec[3] });
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(RectColliderComponent, RectColliderComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(RectColliderComponent, RectColliderComponent::Deserialize);
}

#endif