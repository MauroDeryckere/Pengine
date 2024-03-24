#ifndef TYPEDOBSERVER
#define TYPEDOBSERVER

#include "ECS.h"
#include "Observer.h"

namespace Pengin
{
	template <typename ComponentType>
	class ComponentObserver final : public Observer
	{
	public:
		ComponentObserver(EntityId entityId) :
			Observer{ entityId, typeid(ComponentType) }
		{}

		virtual ~ComponentObserver() override = default;

		virtual void RegisterCallbacks() override
		{
			auto& component = ECS::GetInstance().GetComponent<ComponentType>(Observer::GetEntityId());
			component.RegisterObservers();

			Observer::SetIsDirtyFalse();
		}

		ComponentObserver(const ComponentObserver&) = delete;
		ComponentObserver(ComponentObserver&&) = delete;
		ComponentObserver& operator=(const ComponentObserver&) = delete;
		ComponentObserver& operator=(const ComponentObserver&&) = delete;
	};
}
#endif