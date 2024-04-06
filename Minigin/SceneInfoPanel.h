#ifndef SCENEINFOPANEL
#define SCENEINFOPANEL

#include "EntityId.h"

namespace Pengin
{
	class ECS;
	struct TransformComponent;
	
	class SceneInfoPanel final
	{
	public:
		SceneInfoPanel() = default;

		~SceneInfoPanel() = default;

		void Render(ECS& ecs);

		SceneInfoPanel(const SceneInfoPanel&) = delete;
		SceneInfoPanel(SceneInfoPanel&&) = delete;
		SceneInfoPanel& operator=(const SceneInfoPanel&) = delete;
		SceneInfoPanel& operator=(const SceneInfoPanel&&) = delete;

	private:
		void RenderTransformInfo(const TransformComponent& transform);
		void RenderRectCollInfo(ECS& ecs, const EntityId id, const TransformComponent& transform);
		void RenderSpriteInfo(ECS& ecs, const EntityId id);
		void RenderAnimationInfo(ECS& ecs, const EntityId id);
	};
}

#endif
