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
		{
			Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::SwitchAnimationEvent>(id, uint8_t{0} ));
		}

		~SnobeeSpawningState() = default;

		std::unique_ptr<Pengin::State> Update();

	private:
		float m_Timer{ 0.f };
	};
}

#endif