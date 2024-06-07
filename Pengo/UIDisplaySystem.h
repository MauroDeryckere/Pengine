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
}

namespace Pengo
{
	class UIDisplaySystem final : public Pengin::BaseSystem
	{
	public:
		UIDisplaySystem(Pengin::ECS& ecs, Pengin::Scene* pScene);
		virtual ~UIDisplaySystem() override = default;

		UIDisplaySystem(const UIDisplaySystem&) = delete;
		UIDisplaySystem(UIDisplaySystem&&) = delete;
		UIDisplaySystem& operator=(const UIDisplaySystem&) = delete;
		UIDisplaySystem& operator=(const UIDisplaySystem&&) = delete;

	private:
		Pengin::ECS& m_ECS;
		Pengin::Scene* m_pScene;

		std::shared_ptr<Pengin::Observer> m_pObserver;

		void OnHealthChangeEvent(const Pengin::BaseEvent& event);
		void OnScoreCollectEvent(const Pengin::BaseEvent& event);
	};
}

#endif


