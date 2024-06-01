#ifndef PENGIN_ONGRIDTAG
#define PENGIN_ONGRIDTAG

#include "EntityId.h"
#include "UUIDComponent.h"
#include "ECS.h"

#include "SerializationRegistry.h"

namespace Pengin
{
	struct OnGridTag final
	{
		EntityId gridId;

		OnGridTag(EntityId id) :
			gridId{ id }
		{}

		OnGridTag() = default;
		~OnGridTag() = default;

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<OnGridTag>(id);

			fieldSer.SerializeField("GridId", comp.gridId ? ecs.GetComponent<UUIDComponent>(comp.gridId).uuid.GetUUID_PrettyStr() : "NULL_UUID", ecs, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<OnGridTag>(id);
			
			std::string uuidStr{};
			fieldSer.DeserializeField("GridId", uuidStr, serializedFields, entityMap);

			assert(!uuidStr.empty());
			assert(uuidStr != "NULL_UUID");

			GameUUID uuid{ uuidStr };
			comp.gridId = entityMap.at(uuid);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(OnGridTag, OnGridTag::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(OnGridTag, OnGridTag::Deserialize);
}

#endif