#ifndef PENGO_SNOOBEEWALKSTATE
#define PENGO_SNOOBEEWALKSTATE

#include "State.h"

namespace Pengo
{
	class SnobeeWalkState final : public Pengin::State
	{
	public:
		SnobeeWalkState(Pengin::EntityId id) :
			State{ id }
		{ }

		~SnobeeWalkState() = default;

		void OnEnter();
		std::unique_ptr<Pengin::State> Update()
		{
			///TODO pengo movement pattern
			return nullptr;
		}
	};
}

#endif