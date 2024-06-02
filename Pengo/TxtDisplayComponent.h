#ifndef DISPLAYCOMPONENT
#define DISPLAYCOMPONENT

#include <string>

#include "SerializationRegistry.h"

namespace Pengo
{
	struct TxtDisplayComponent final
	{
		std::string prefix{};
		std::string postfix{};

		TxtDisplayComponent() = default;

		TxtDisplayComponent(const std::string& prefix, const std::string& postfix = {}) :
			prefix{ prefix },
			postfix{ postfix }
		{}

		~TxtDisplayComponent() = default;

		static void Serialize(const Pengin::FieldSerializer& fieldSer, const Pengin::ECS& ecs, const Pengin::EntityId id, std::vector<uint8_t>& fieldVector)
		{ 
			const auto& comp = ecs.GetComponent<TxtDisplayComponent>(id);

			fieldSer.SerializeField("Prefix", comp.prefix, ecs, fieldVector);
			fieldSer.SerializeField("Postfix", comp.postfix, ecs, fieldVector);
		}
		static void Deserialize(const Pengin::FieldSerializer& fieldSer, Pengin::ECS& ecs, const Pengin::EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<Pengin::GameUUID, Pengin::EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<TxtDisplayComponent>(id);

			fieldSer.DeserializeField("Prefix", comp.prefix, serializedFields, entityMap);
			fieldSer.DeserializeField("Postfix", comp.postfix, serializedFields, entityMap);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(TxtDisplayComponent, TxtDisplayComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(TxtDisplayComponent, TxtDisplayComponent::Deserialize);
}

#endif