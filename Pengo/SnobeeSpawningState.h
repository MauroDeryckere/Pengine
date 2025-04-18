#ifndef PENGO_SNOOBEESPAWNSTATE
#define PENGO_SNOOBEESPAWNSTATE

#include "State.h"
#include "EventManager.h"
#include "SwitchAnimationEvent.h"

namespace Pengo
{
	class SnobeeSpawningState final : public Pengin::State
	{
	public:
		SnobeeSpawningState(Pengin::EntityId id) :
			State{ id }
		{ }

		~SnobeeSpawningState() = default;

		std::unique_ptr<Pengin::State> Update();

	private:
		float m_Timer{ 0.f };
	};
}

#endif