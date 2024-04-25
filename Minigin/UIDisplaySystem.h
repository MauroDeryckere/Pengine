#ifndef UIDISPLAYSYSTEM
#define UIDISPLAYSYSTEM

#include "BaseSystem.h"

#include <memory>

namespace Pengin
{
	class ECS;
	class Scene;
	class Observer;
	class BaseEvent;

	class UIDisplaySystem final : public BaseSystem
	{
	public:
		UIDisplaySystem(ECS& ecs, Scene* pScene);
		virtual ~UIDisplaySystem() override = default;

		UIDisplaySystem(const UIDisplaySystem&) = delete;
		UIDisplaySystem(UIDisplaySystem&&) = delete;
		UIDisplaySystem& operator=(const UIDisplaySystem&) = delete;
		UIDisplaySystem& operator=(const UIDisplaySystem&&) = delete;

	private:
		ECS& m_ECS;
		Scene* m_pScene;

		std::shared_ptr<Observer> m_pObserver;

		void OnHealthChangeEvent(const BaseEvent& event);
		void OnScoreCollectEvent(const BaseEvent& event);
	};
}

#endif


