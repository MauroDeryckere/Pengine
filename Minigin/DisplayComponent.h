#ifndef DISPLAYCOMPONENT
#define DISPLAYCOMPONENT

#include <string>

namespace Pengin
{
	struct TxtDisplayComponent final
	{
		TxtDisplayComponent() = default;

		TxtDisplayComponent(const std::string& prefix, const std::string& postfix = {}) :
			prefix{ prefix },
			postfix{ postfix }
		{}

		~TxtDisplayComponent() = default;

		std::string prefix{};
		std::string postfix{};


		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{ 
			const auto& comp = ecs.GetComponent<TxtDisplayComponent>(id);

			fieldSer.SerializeField("Prefix", comp.prefix, fieldVector);
			fieldSer.SerializeField("Postfix", comp.postfix, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<TxtDisplayComponent>(id);

			fieldSer.DeserializeField("Prefix", comp.prefix, serializedFields);
			fieldSer.DeserializeField("Postfix", comp.postfix, serializedFields);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(TxtDisplayComponent, TxtDisplayComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(TxtDisplayComponent, TxtDisplayComponent::Deserialize);
}

#endif