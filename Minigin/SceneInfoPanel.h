#ifndef SCENEINFOPANEL
#define SCENEINFOPANEL

#include "CoreIncludes.h"

namespace Pengin
{
	class ECS;
	class Scene;
	struct TransformComponent;
	
	class SceneInfoPanel final
	{
	public:
		SceneInfoPanel(Scene* pScene) :
			m_pScene{ pScene }
		{}

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

		void RenderEditorWindow(ECS& ecs);

		Scene* m_pScene;
	};
}

#endif
