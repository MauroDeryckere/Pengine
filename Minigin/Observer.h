#ifndef OBSERVER
#define OBSERVER

#include <string>
#include <memory>
#include <typeindex>
#include "ECS.h"

namespace Pengin
{
	class Observer abstract
	{
	public:
		virtual void RegisterCallbacks() = 0;

		void RegisterForEvent(std::weak_ptr<Observer> pObs, const std::string& eventName, std::function<void()> fCallback);

		[[nodiscard]] EntityId GetEntityId() const { return m_EntityId; }
		[[nodiscard]] const std::type_index GetTypeIdx() const { return m_TypeIdx; }

		[[nodiscard]] bool IsDirty() const { return m_IsDirty; }
		void SetDirty() { m_IsDirty = true; }

		virtual ~Observer() = default;

	protected:
		Observer(EntityId entityId, std::type_index typeIdx) :
			m_EntityId{ entityId },
			m_TypeIdx{ typeIdx },
			m_IsDirty{ false }
		{ }

		void SetIsDirtyFalse() { m_IsDirty = false; }


		Observer(const Observer&) = delete;
		Observer(Observer&&) = delete;
		Observer& operator=(const Observer&) = delete;
		Observer& operator=(const Observer&&) = delete;

	private:
		const EntityId m_EntityId;
		const std::type_index m_TypeIdx;

		bool m_IsDirty{ false };
	};
}

#endif