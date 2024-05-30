#ifndef PENGIN_STATE
#define PENGIN_STATE

#include <memory>
#include <cassert>
#include <string>

#include "EntityId.h"

namespace Pengin
{
	class State abstract
	{
	public:
		State(EntityId id) :
			m_EntityId{ id }
		{ 
			assert(id != NULL_ENTITY_ID);
		}

		virtual ~State() = default;

		virtual void OnEnter() {}
		virtual std::unique_ptr<State> Update() { return nullptr; }
		virtual void OnExit() {}

	protected:
		[[nodiscard]] const EntityId GetEntityId() const noexcept { return m_EntityId; }

	private:
		const EntityId m_EntityId;
	};
}

#endif
