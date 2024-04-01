#ifndef UIDISPLAYSYSTEM
#define UIDISPLAYSYSTEM

#include <memory>

namespace Pengin
{
	class ECS;
	class Observer;

	class UIDisplaySystem final
	{
	public:
		UIDisplaySystem(ECS& ecs);
		~UIDisplaySystem() = default;

		UIDisplaySystem(const UIDisplaySystem&) = delete;
		UIDisplaySystem(UIDisplaySystem&&) = delete;
		UIDisplaySystem& operator=(const UIDisplaySystem&) = delete;
		UIDisplaySystem& operator=(const UIDisplaySystem&&) = delete;

	private:
		ECS& m_ECS;

		std::shared_ptr<Observer> m_pObserver;

		void OnHealthChangeEvent(const void* eventData);
		void OnScoreCollectEvent(const void* eventData);
	};
}

#endif


