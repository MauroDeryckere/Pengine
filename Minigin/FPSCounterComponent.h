#ifndef FPSCOUNTERCOMPONENT
#define FPSCOUNTERCOMPONENT

#include "SerializationRegistry.h"

namespace Pengin
{
	struct FPSCounterComponent final
	{
		unsigned frameCount{ 0 };
		float accumulatedTime{ 0.f };

		FPSCounterComponent() = default;
		~FPSCounterComponent() = default;

		static void Serialize(const FieldSerializer&, const ECS&, const EntityId, std::vector<uint8_t>&)
		{ }
		static void Deserialize(const FieldSerializer&, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>&, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			ecs.AddComponent<FPSCounterComponent>(id);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(FPSCounterComponent, FPSCounterComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(FPSCounterComponent, FPSCounterComponent::Deserialize);
}

#endif