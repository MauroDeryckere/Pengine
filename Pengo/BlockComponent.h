#ifndef PENGO_BLOCK
#define PENGO_BLOCK

#include "SerializationRegistry.h"
#include <glm/vec2.hpp>

namespace Pengo
{
	struct BlockComponent final
	{
		enum class BlockState : uint8_t
		{
			Still, 
			Breaking, 
			Moving
		};

		BlockState blockState{ BlockState::Still };

		glm::vec2 dir{};

		float breakingTimer{ 0.f };

		BlockComponent() = default;
		~BlockComponent() = default;

		static void Serialize(const Pengin::FieldSerializer&, const Pengin::ECS&, const Pengin::EntityId, std::vector<uint8_t>&)
		{ }
		static void Deserialize(const Pengin::FieldSerializer&, Pengin::ECS& ecs, const Pengin::EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>&, const std::unordered_map<Pengin::GameUUID, Pengin::EntityId>& entityMap [[maybe_unused]] )
		{
			//auto& comp =
			ecs.AddComponent<BlockComponent>(id);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(BlockComponent, BlockComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(BlockComponent, BlockComponent::Deserialize);
}

#endif