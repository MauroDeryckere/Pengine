#ifndef PLAYERCOMPONENT
#define PLAYERCOMPONENT

#include "CoreIncludes.h"

#include "SerializationRegistry.h"

namespace Pengin
{
	using UserIndex = GameUUID;

	struct PlayerComponent final
	{
		PlayerComponent(const UserIndex& userIdx, float movementSpeed = 0.f) :
			userIdx{ userIdx },
			movementSpeed{movementSpeed}
		{}

		PlayerComponent() = default;
		~PlayerComponent() = default;

		UserIndex userIdx{ true };
		float movementSpeed{ 0.f }; //Should possibly be moved in future


		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<PlayerComponent>(id);

			fieldSer.SerializeField("UserIdx", comp.userIdx.GetUUID_PrettyStr(), fieldVector);
			fieldSer.SerializeField("MovementSpeed", comp.movementSpeed, fieldVector);
		}


	};
	REGISTER_SERIALIZATION_FUNCTION(PlayerComponent, PlayerComponent::Serialize);
}

#endif