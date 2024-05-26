#ifndef PLAYERCOMPONENT
#define PLAYERCOMPONENT

#include "CoreIncludes.h"

#include "SerializationRegistry.h"

namespace Pengin
{
	using UserIndex = GameUUID;

	struct PlayerComponent final
	{
		UserIndex userIdx{ true };
		float movementSpeed{ 0.f }; //Should possibly be moved in future

		PlayerComponent(const UserIndex& userIdx, float movementSpeed = 0.f) :
			userIdx{ userIdx },
			movementSpeed{ movementSpeed }
		{}

		PlayerComponent() = default;
		~PlayerComponent() = default;

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<PlayerComponent>(id);

			fieldSer.SerializeField("UserIdx", comp.userIdx.GetUUID_PrettyStr(),ecs,  fieldVector);
			fieldSer.SerializeField("MovementSpeed", comp.movementSpeed, ecs, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			std::string userIdxStr{};
			float movementSpeed{};

			fieldSer.DeserializeField("UserIdx", userIdxStr, serializedFields, entityMap);
			fieldSer.DeserializeField("MovementSpeed", movementSpeed, serializedFields, entityMap);

			ecs.AddComponent<PlayerComponent>(id, GameUUID{userIdxStr}, movementSpeed);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(PlayerComponent, PlayerComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(PlayerComponent, PlayerComponent::Deserialize);
}

#endif