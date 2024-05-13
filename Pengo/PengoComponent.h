#ifndef PENGOCOMPONENT
#define PENGOCOMPONENT

#include <memory>
#include <string>

#include "PlayerState.h"
#include "PengoIdleState.h"

#include "SerializationRegistry.h"

namespace Pengo
{
	struct PengoComponent final
	{
		std::unique_ptr<Pengin::PlayerState> pPlayerState{ std::make_unique<PengoIdleState>( Pengin::GameUUID{true} )};

		void SetPlayerState(std::unique_ptr<Pengin::PlayerState> pState)
		{
			assert(pState);

			pPlayerState->OnExit();

			pPlayerState = std::move(pState);
			pPlayerState->OnEnter();
		}

		void Update(const Pengin::UserIndex& userIndex)
		{
			assert(pPlayerState);

			auto pNewState = pPlayerState->Update(userIndex);

			if (pNewState)
			{
				pPlayerState->OnExit();

				pPlayerState = std::move(pNewState);
				pPlayerState->OnEnter();
			}
		}

		void HandleInput(const Pengin::UserIndex& userIndex, const std::string& actionName)
		{
			assert(pPlayerState);

			auto pNewState = pPlayerState->HandleInput(userIndex, actionName);

			if (pNewState)
			{
				pPlayerState->OnExit();

				pPlayerState = std::move(pNewState);
				pPlayerState->OnEnter();
			}

		}


		static void Serialize(const Pengin::FieldSerializer& fieldSer, const Pengin::ECS& ecs, const Pengin::EntityId id, std::vector<uint8_t>& fieldVector)
		{
			fieldSer;
			ecs;
			id;
			fieldVector;

			//fieldSer.SerializeField("", , fieldVector);
		}
		static void Deserialize(const Pengin::FieldSerializer& fieldSer, Pengin::ECS& ecs, const Pengin::EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<Pengin::GameUUID, Pengin::EntityId>& entityMap [[maybe_unused]] )
		{
			ecs.AddComponent<PengoComponent>(id);

			fieldSer;
			serializedFields;

			//fieldSer.DeserializeField("", , serializedFields);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(PengoComponent, PengoComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(PengoComponent, PengoComponent::Deserialize);
}

#endif