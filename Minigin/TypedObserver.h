#ifndef TYPEDOBSERVER
#define TYPEDOBSERVER

#include "ECS.h"
#include "Observer.h"

namespace Pengin
{
	template <typename ComponentType>
	class TypedObserver final : public Observer
	{
	public:
		TypedObserver(EntityId entityId) :
			Observer{ entityId, typeid(ComponentType) }
		{}

		virtual ~TypedObserver() override = default;

		virtual void RegisterCallbacks() override
		{
			auto& component = ECS::GetInstance().GetComponent<ComponentType>(Observer::GetEntityId());
			component.RegisterObservers();

			Observer::SetIsDirtyFalse();
		}

		TypedObserver(const TypedObserver&) = delete;
		TypedObserver(TypedObserver&&) = delete;
		TypedObserver& operator=(const TypedObserver&) = delete;
		TypedObserver& operator=(const TypedObserver&&) = delete;
	};
}
#endif