#ifndef VELOCITYCOMPONENT
#define VELOCITYCOMPONENT

#include "glm/glm.hpp"

#include "SerializationRegistry.h"

namespace Pengin
{
	struct VelocityComponent final
	{
		VelocityComponent() = default;

		VelocityComponent(const glm::vec3& vel) :
			velocity{ vel }
		{}

		~VelocityComponent() = default;

		glm::vec3 velocity{ 0, 0, 0 };


		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<VelocityComponent>(id);

			fieldSer.SerializeField("Velocity", std::vector<float>{comp.velocity.x, comp.velocity.y, comp.velocity.z}, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<VelocityComponent>(id);

			std::vector<float> velVec{};

			fieldSer.DeserializeField("Velocity", velVec, serializedFields);
			comp.velocity = { velVec[0], velVec[1], velVec[2] };
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(VelocityComponent, VelocityComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(VelocityComponent, VelocityComponent::Deserialize);
}

#endif