#ifndef PENGOSNOBEECOMPONENT
#define PENGOSNOBEECOMPONENT

#include <memory>
#include <string>
#include "State.h"
#include "SerializationRegistry.h"
#include "SnobeeSpawningState.h"

namespace Pengo
{
	struct SnobeeComponent final
	{
		std::unique_ptr<Pengin::State> pSnobeeState;
		float movementSpeed{ 0.f };

		void SetState(std::unique_ptr<Pengin::State> pState)
		{
			assert(pState);

			pSnobeeState->OnExit();

			pSnobeeState = std::move(pState);
			pSnobeeState->OnEnter();
		}

		void UpdateState()
		{
			assert(pSnobeeState);

			auto pNewState = pSnobeeState->Update();

			if (pNewState)
			{
				pSnobeeState->OnExit();

				pSnobeeState = std::move(pNewState);
				pSnobeeState->OnEnter();
			}
		}


		static void Serialize(const Pengin::FieldSerializer& fieldSer, const Pengin::ECS& ecs, const Pengin::EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<SnobeeComponent>(id);

			fieldSer.SerializeField("MovementSpeed", comp.movementSpeed, ecs, fieldVector);
		}
		static void Deserialize(const Pengin::FieldSerializer& fieldSer, Pengin::ECS& ecs, const Pengin::EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<Pengin::GameUUID, Pengin::EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<SnobeeComponent>(id);

			comp.pSnobeeState = std::make_unique<SnobeeSpawningState>(id);
			fieldSer.DeserializeField("MovementSpeed", comp.movementSpeed, serializedFields, entityMap);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(SnobeeComponent, SnobeeComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(SnobeeComponent, SnobeeComponent::Deserialize);
}

#endif