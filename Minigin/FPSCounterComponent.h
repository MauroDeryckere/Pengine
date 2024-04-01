#ifndef FPSCOUNTERCOMPONENT
#define FPSCOUNTERCOMPONENT

namespace Pengin
{
	struct FPSCounterComponent final
	{
		FPSCounterComponent() = default;
		~FPSCounterComponent() = default;

		unsigned m_FrameCount{ 0 };
		float m_AccumulatedTime{ 0.f };
	};
}

#endif