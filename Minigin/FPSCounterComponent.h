#ifndef FPSCOUNTERCOMPONENT
#define FPSCOUNTERCOMPONENT

namespace Pengin
{
	struct FPSCounterComponent final
	{
		FPSCounterComponent() = default;
		~FPSCounterComponent() = default;

		unsigned frameCount{ 0 };
		float accumulatedTime{ 0.f };
	};
}

#endif