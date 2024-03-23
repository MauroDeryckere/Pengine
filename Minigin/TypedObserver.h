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
		virtual void RegisterCallbacks() override
		{
			auto& component = ECS::GetInstance().GetComponent<ComponentType>(Observer::GetEntityId());
			component.RegisterObservers();

			Observer::SetIsDirtyFalse();
		}

		virtual ~TypedObserver() override = default;

		TypedObserver(const TypedObserver&) = delete;
		TypedObserver(TypedObserver&&) = delete;
		TypedObserver& operator=(const TypedObserver&) = delete;
		TypedObserver& operator=(const TypedObserver&&) = delete;
		TypedObserver(EntityId entityId) :
			Observer{ entityId, typeid(ComponentType) } //TODO MOVE TO FRIEND / PRIVATE
		{}

	private:
		friend class EventManager;
	};
}
#endif