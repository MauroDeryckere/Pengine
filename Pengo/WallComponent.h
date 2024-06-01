#ifndef PENGO_WALL
#define PENGO_WALL

#include "SerializationRegistry.h"

namespace Pengo
{
	struct WallComponent final
	{
		WallComponent() = default;
		~WallComponent() = default;

		static void Serialize(const Pengin::FieldSerializer&, const Pengin::ECS&, const Pengin::EntityId, std::vector<uint8_t>&)
		{ }
		static void Deserialize(const Pengin::FieldSerializer&, Pengin::ECS& ecs, const Pengin::EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>&, const std::unordered_map<Pengin::GameUUID, Pengin::EntityId>& entityMap [[maybe_unused]] )
		{
			//auto& comp =
			ecs.AddComponent<WallComponent>(id);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(WallComponent, WallComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(WallComponent, WallComponent::Deserialize);
}

#endif