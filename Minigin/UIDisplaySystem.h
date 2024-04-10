#ifndef UIDISPLAYSYSTEM
#define UIDISPLAYSYSTEM

#include <memory>

namespace Pengin
{
	class ECS;
	class Scene;
	class Observer;

	class UIDisplaySystem final
	{
	public:
		UIDisplaySystem(ECS& ecs, Scene* pScene);
		~UIDisplaySystem() = default;

		UIDisplaySystem(const UIDisplaySystem&) = delete;
		UIDisplaySystem(UIDisplaySystem&&) = delete;
		UIDisplaySystem& operator=(const UIDisplaySystem&) = delete;
		UIDisplaySystem& operator=(const UIDisplaySystem&&) = delete;

	private:
		ECS& m_ECS;
		Scene* m_pScene;

		std::shared_ptr<Observer> m_pObserver;

		void OnHealthChangeEvent(const void* eventData);
		void OnScoreCollectEvent(const void* eventData);
	};
}

#endif


