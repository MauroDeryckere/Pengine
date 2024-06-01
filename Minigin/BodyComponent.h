#ifndef VELOCITYCOMPONENT
#define VELOCITYCOMPONENT

#include "glm/glm.hpp"

#include "SerializationRegistry.h"

#include "CollisionTypes.h"

namespace Pengin
{
	struct BodyComponent final
	{
		glm::vec3 lastPosition{ 0, 0, 0 };
		glm::vec3 currentPosition{ 0, 0, 0 }; //center of mass pos for physics simulation (currently just the same as transform)

		glm::vec3 inputVelocity{ 0, 0 ,0 };
		glm::vec3 lastFrInputVelocity{ 0, 0, 0 };

		glm::vec3 velocity{ 0, 0, 0 };

		CollType collType{ CollType::Dynamic };

		BodyComponent() = default;

		BodyComponent(const glm::vec3& vel) :
			velocity{ vel }
		{ }

		~BodyComponent() = default;

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<BodyComponent>(id);

			fieldSer.SerializeField("Velocity", std::vector<float>{comp.velocity.x, comp.velocity.y, comp.velocity.z}, ecs, fieldVector);

			fieldSer.SerializeField("CurrPosition", std::vector<float>{comp.currentPosition.x, comp.currentPosition.y, comp.currentPosition.z}, ecs, fieldVector);
			fieldSer.SerializeField("LastPosition", std::vector<float>{comp.lastPosition.x, comp.lastPosition.y, comp.lastPosition.z}, ecs, fieldVector);

			fieldSer.SerializeField("CollType", comp.collType, ecs, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<BodyComponent>(id);

			std::vector<float> vec3f{};

			fieldSer.DeserializeField("Velocity", vec3f, serializedFields, entityMap);
			comp.velocity = { vec3f[0], vec3f[1], vec3f[2] };
			vec3f.clear();

			fieldSer.DeserializeField("CurrPosition", vec3f, serializedFields, entityMap);
			comp.currentPosition = { vec3f[0], vec3f[1], vec3f[2] };
			vec3f.clear();

			fieldSer.DeserializeField("LastPosition", vec3f, serializedFields, entityMap);
			comp.lastPosition = { vec3f[0], vec3f[1], vec3f[2] };

			fieldSer.DeserializeField("CollType", comp.collType, serializedFields, entityMap);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(BodyComponent, BodyComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(BodyComponent, BodyComponent::Deserialize);
}

#endif