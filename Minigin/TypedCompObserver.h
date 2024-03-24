#ifndef TYPEDCOMPOBSERVER
#define TYPEDCOMPOBSERVER

#include "ECS.h"
#include "CompObserver.h"

namespace Pengin
{
	template <typename ComponentType>
	class TypedCompObserver final : public CompObserver
	{
	public:
		TypedCompObserver(EntityId entityId) :
			CompObserver{ entityId, typeid(ComponentType) }
		{}

		virtual ~TypedCompObserver() override = default;

		virtual void RegisterCallbacks() override
		{
			auto& component = ECS::GetInstance().GetComponent<ComponentType>(CompObserver::GetEntityId());
			component.RegisterObservers();

			CompObserver::SetIsDirtyFalse();
		}

		TypedCompObserver(const TypedCompObserver&) = delete;
		TypedCompObserver(TypedCompObserver&&) = delete;
		TypedCompObserver& operator=(const TypedCompObserver&) = delete;
		TypedCompObserver& operator=(const TypedCompObserver&&) = delete;
	};
}
#endif