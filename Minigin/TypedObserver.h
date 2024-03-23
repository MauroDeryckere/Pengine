#pragma once

#include "ECS.h"
#include "Observer_.h"

namespace Pengin
{
	template <typename ComponentType>
	class TypedObserver final : public Observer_
	{
	public:
		virtual void RegisterCallbacks() override
		{
			auto& component = ECS::GetInstance().GetComponent<ComponentType>(Observer_::GetEntityId());
			component.RegisterObservers();

			Observer_::SetIsDirtyFalse();
		}

		virtual ~TypedObserver() override = default;

		TypedObserver(const TypedObserver&) = delete;
		TypedObserver(TypedObserver&&) = delete;
		TypedObserver& operator=(const TypedObserver&) = delete;
		TypedObserver& operator=(const TypedObserver&&) = delete;
		TypedObserver(EntityId entityId) :
			Observer_{ entityId, typeid(ComponentType) } //TODO MOVE TO FRIEND / PRIVATE
		{}

	private:
		friend class EventManager_;
	};
}