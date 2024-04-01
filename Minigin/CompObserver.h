#ifndef COMPOBSERVER
#define COMPOBSERVER

#include <string>
#include <memory>
#include <typeindex>
#include "EntityId.h"

namespace Pengin
{
	class CompObserver
	{
	public:
		virtual void RegisterCallbacks() = 0;

		void RegisterForEvent(std::weak_ptr<CompObserver> pObs, const std::string& eventName, std::function<void(const void* eventData)> fCallback);

		[[nodiscard]] EntityId GetEntityId() const { return m_EntityId; }
		[[nodiscard]] const std::type_index GetTypeIdx() const { return m_TypeIdx; }

		[[nodiscard]] bool IsDirty() const { return m_IsDirty; }
		void SetDirty() { m_IsDirty = true; }

		virtual ~CompObserver() = default;

	protected:
		CompObserver(EntityId entityId, std::type_index typeIdx) :
			m_EntityId{ entityId },
			m_TypeIdx{ typeIdx },
			m_IsDirty{ false }
		{ }

		void SetIsDirtyFalse() { m_IsDirty = false; }

		CompObserver(const CompObserver&) = delete;
		CompObserver(CompObserver&&) = delete;
		CompObserver& operator=(const CompObserver&) = delete;
		CompObserver& operator=(const CompObserver&&) = delete;

	private:
		const EntityId m_EntityId;
		const std::type_index m_TypeIdx;

		bool m_IsDirty{ false };
	};
}

#endif