#pragma once

#include <string>
#include <memory>
#include <typeindex>
#include "ECS.h"

namespace Pengin
{
	class Observer_ abstract : public std::enable_shared_from_this<Observer_>
	{
	public:
		virtual void RegisterCallbacks() = 0;

		void RegisterForEvent(const std::string& eventName, std::function<void()> fCallback);

		[[nodiscard]] EntityId GetEntityId() const { return m_EntityId; }
		[[nodiscard]] const std::type_index GetTypeIdx() const { return m_TypeIdx; }

		[[nodiscard]] bool IsDirty() const { return m_IsDirty; }
		void SetDirty() { m_IsDirty = true; }

	protected:
		Observer_(EntityId entityId, std::type_index typeIdx) :
			m_EntityId{ entityId },
			m_TypeIdx{ typeIdx },
			m_IsDirty{ true }
		{}

		void SetIsDirtyFalse() { m_IsDirty = false; }

		virtual ~Observer_() = default;

		Observer_(const Observer_&) = delete;
		Observer_(Observer_&&) = delete;
		Observer_& operator=(const Observer_&) = delete;
		Observer_& operator=(const Observer_&&) = delete;

	private:
		const EntityId m_EntityId;
		const std::type_index m_TypeIdx;

		bool m_IsDirty{ false };
	};
}
