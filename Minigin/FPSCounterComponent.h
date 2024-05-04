#ifndef FPSCOUNTERCOMPONENT
#define FPSCOUNTERCOMPONENT

#include "SerializationRegistry.h"

namespace Pengin
{
	struct FPSCounterComponent final
	{
		FPSCounterComponent() = default;
		~FPSCounterComponent() = default;

		unsigned frameCount{ 0 };
		float accumulatedTime{ 0.f };

		static void Serialize(const FieldSerializer&, const ECS&, const EntityId, std::vector<uint8_t>&)
		{ }
	};

	REGISTER_SERIALIZATION_FUNCTION(FPSCounterComponent, FPSCounterComponent::Serialize);
}

#endif