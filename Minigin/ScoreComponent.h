#ifndef SCORECOMPONENT
#define SCORECOMPONENT

#include "CoreIncludes.h"

#include "EntityId.h"

#include "UUIDComponent.h"
#include "SerializationRegistry.h"

#include <vector>

namespace Pengin
{
	struct ScoreComponent final
	{
		unsigned score{};
		std::vector<EntityId> scoreDisplays{};

		ScoreComponent(const std::vector<EntityId>& scoreDisplayIds = { }) :
			score{ 0 },
			scoreDisplays{ scoreDisplayIds }
		{
			scoreDisplays.shrink_to_fit();
		}

		~ScoreComponent() = default;

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<ScoreComponent>(id);

			fieldSer.SerializeField("Score", comp.score, fieldVector);

			std::vector<std::string> scoreDispUuids;
			scoreDispUuids.reserve(comp.scoreDisplays.size());

			for (auto entity : comp.scoreDisplays)
			{
				scoreDispUuids.emplace_back(entity != NULL_ENTITY_ID ? ecs.GetComponent<UUIDComponent>(id).uuid.GetUUID_PrettyStr() : "NULL_UUID");
			}

			fieldSer.SerializeField("ScoreDisplayIds", scoreDispUuids, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<ScoreComponent>(id);

			fieldSer.DeserializeField("Score", comp.score, serializedFields);

			std::vector<std::string> idVec{};
			fieldSer.DeserializeField("ScoreDisplayIds", idVec, serializedFields);

			comp.scoreDisplays.reserve(idVec.size());
			for (const auto& uuid : idVec)
			{
				comp.scoreDisplays.emplace_back(entityMap.at({ uuid }));
			}
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(ScoreComponent, ScoreComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(ScoreComponent, ScoreComponent::Deserialize);
}

#endif
