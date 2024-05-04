#ifndef HEALTHCOMPONENT
#define HEALTHCOMPONENT

#include "CoreIncludes.h"

#include "UUIDComponent.h"

#include "SerializationRegistry.h"

#include <vector>

namespace Pengin
{
	struct HealthComponent final
	{
		HealthComponent(unsigned health = 0, const std::vector<EntityId>& healthbarIds = { }) :
			health{ health },
			healthDisplayIds{ healthbarIds }
		{
			healthDisplayIds.shrink_to_fit();
		}

		~HealthComponent() = default; 

		unsigned health;
		std::vector<EntityId> healthDisplayIds;


		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<HealthComponent>(id);

			fieldSer.SerializeField("Health", comp.health, fieldVector);

			std::vector<std::string> healthDispUuids;
			healthDispUuids.reserve(comp.healthDisplayIds.size());

			for (auto entity : comp.healthDisplayIds)
			{
				healthDispUuids.emplace_back(entity ? ecs.GetComponent<UUIDComponent>(entity).uuid.GetUUID_PrettyStr() : "NULL_UUID");
			}

			fieldSer.SerializeField("HealthDisplayIds", healthDispUuids, fieldVector);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(HealthComponent, HealthComponent::Serialize);
}

#endif
