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
	};

	REGISTER_SERIALIZATION_FUNCTION(VelocityComponent, VelocityComponent::Serialize);
}

#endif