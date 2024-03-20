#ifndef FPSCOUNTERCOMPONENT
#define FPSCOUNTERCOMPONENT

#include "ECS.h"

namespace Pengin
{
	class FPSCounterComponent
	{
	public:
		FPSCounterComponent(EntityId id) :
			m_Id{ id }
		{}

		~FPSCounterComponent() = default;

		void Update();

	private:
		unsigned m_FrameCount{ 0 };
		float m_AccumulatedTime{ 0.f };

		const EntityId m_Id;
	};
}

#endif