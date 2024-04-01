#ifndef TRANSFORMCOMPONENT
#define TRANSFORMCOMPONENT

#include "glm/vec3.hpp"
#include "EntityID.h"

namespace Pengin
{
	struct TransformComponent final
	{
		TransformComponent() = default;

		TransformComponent(const glm::vec3& pos = { 0, 0, 0 }, const glm::vec3& rotation = { 0, 0, 0 }, const glm::vec3& scale = { 1, 1, 1 }) :
			worldPos{ pos },
			localPos{ pos },
			rotation{ rotation },
			scale{ scale },
			isPosDirty{ false },
			relation { }
		{ }

		~TransformComponent() = default;

		glm::vec3 worldPos{ 0, 0, 0 };
		glm::vec3 localPos{ 0, 0, 0 };
		glm::vec3 rotation{ 0, 0, 0 };
		glm::vec3 scale{ 1, 1, 1 };

		bool isPosDirty{ false };

		struct Relationship
		{
			size_t children{ 0 };
			EntityId firstChild{ NULL_ENTITY_ID };
			EntityId prevSibling{ NULL_ENTITY_ID };
			EntityId nextSibling{ NULL_ENTITY_ID };
			EntityId parent{ NULL_ENTITY_ID };
		};

		Relationship relation { };
	};
}

#endif

