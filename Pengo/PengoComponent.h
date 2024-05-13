#ifndef PENGOCOMPONENT
#define PENGOCOMPONENT

#include <memory>
#include <string>

#include "PlayerState.h"
#include "PengoIdleState.h"

#include "SerializationRegistry.h"

namespace Pengo
{
	using namespace Pengin;

	struct PengoComponent final
	{
		std::unique_ptr<PlayerState> pPlayerState{ std::make_unique<PengoIdleState>( GameUUID{true} )};

		void SetPlayerState(std::unique_ptr<PlayerState> pState)
		{
			assert(pState);

			pPlayerState->OnExit();

			pPlayerState = std::move(pState);
			pPlayerState->OnEnter();
		}

		void Update(const UserIndex& userIndex)
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

		void HandleInput(const UserIndex& userIndex, const std::string& actionName)
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


		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			fieldSer;
			ecs;
			id;
			fieldVector;

			//fieldSer.SerializeField("", , fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
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