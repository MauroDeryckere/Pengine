#ifndef SCORECOMPONENT
#define SCORECOMPONENT

#include "CoreIncludes.h"

#include "UUIDComponent.h"
#include "SerializationRegistry.h"

#include <vector>

namespace Pengin
{
	struct ScoreComponent final
	{
	public:
		ScoreComponent(const std::vector<EntityId>& scoreDisplayIds = { }) :
			score{ 0 },
			scoreDisplays{ scoreDisplayIds }
		{
			scoreDisplays.shrink_to_fit();
		}

		~ScoreComponent() = default;

		unsigned score{};
		std::vector<EntityId> scoreDisplays{};


		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<ScoreComponent>(id);

			fieldSer.SerializeField("Score", comp.score, fieldVector);

			std::vector<std::string> scoreDispUuids;
			scoreDispUuids.reserve(comp.scoreDisplays.size());

			for (auto entity : comp.scoreDisplays)
			{
				scoreDispUuids.emplace_back(entity ? ecs.GetComponent<UUIDComponent>(entity).uuid.GetUUID_PrettyStr() : "NULL_UUID");
			}

			fieldSer.SerializeField("ScoreDisplayIds", scoreDispUuids, fieldVector);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(ScoreComponent, ScoreComponent::Serialize);
}

#endif
