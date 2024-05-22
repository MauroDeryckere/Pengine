#ifndef HEALTHCOMPONENT
#define HEALTHCOMPONENT

#include "CoreIncludes.h"

#include "UUIDComponent.h"

#include "SerializationRegistry.h"

#include <vector>

namespace Pengo
{
	struct HealthComponent final
	{
		HealthComponent(unsigned health = 0, const std::vector<Pengin::EntityId>& healthbarIds = { }) :
			health{ health },
			healthDisplayIds{ healthbarIds }
		{
			healthDisplayIds.shrink_to_fit();
		}

		~HealthComponent() = default; 

		unsigned health;
		std::vector<Pengin::EntityId> healthDisplayIds;


		static void Serialize(const Pengin::FieldSerializer& fieldSer, const Pengin::ECS& ecs, const Pengin::EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<HealthComponent>(id);

			fieldSer.SerializeField("Health", comp.health, fieldVector);

			std::vector<std::string> healthDispUuids;
			healthDispUuids.reserve(comp.healthDisplayIds.size());

			for (auto entity : comp.healthDisplayIds)
			{
				healthDispUuids.emplace_back(entity != Pengin::NULL_ENTITY_ID ? ecs.GetComponent<Pengin::UUIDComponent>(entity).uuid.GetUUID_PrettyStr() : "NULL_UUID");
			}

			fieldSer.SerializeField("HealthDisplayIds", healthDispUuids, fieldVector);
		}
		static void Deserialize(const Pengin::FieldSerializer& fieldSer, Pengin::ECS& ecs, const Pengin::EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<Pengin::GameUUID, Pengin::EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<HealthComponent>(id);

			fieldSer.DeserializeField("Health", comp.health, serializedFields);

			std::vector<std::string> idVec{};
			fieldSer.DeserializeField("HealthDisplayIds", idVec, serializedFields);

			comp.healthDisplayIds.reserve(idVec.size());
			for (const auto& uuid : idVec)
			{
				comp.healthDisplayIds.emplace_back(entityMap.at({ uuid }));
			}
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(HealthComponent, HealthComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(HealthComponent, HealthComponent::Deserialize);
}

#endif
