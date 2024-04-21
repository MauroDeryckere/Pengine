#ifndef FPSSYSTEM
#define FPSSYSTEM

#include "BaseSystem.h"

namespace Pengin
{
	class ECS;

	class FPSSystem final : public BaseSystem
	{
	public:
		FPSSystem(ECS& ecs) :
			BaseSystem{ },
			m_ECS{ ecs }
		{}

		virtual ~FPSSystem() override = default;

		virtual void Update() override;

		FPSSystem(const FPSSystem&) = delete;
		FPSSystem(FPSSystem&&) = delete;
		FPSSystem& operator=(const FPSSystem&) = delete;
		FPSSystem& operator=(const FPSSystem&&) = delete;

	private:
		ECS& m_ECS;
	};
}

#endif
