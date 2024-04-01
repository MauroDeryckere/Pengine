#ifndef FPSSYSTEM
#define FPSSYSTEM

namespace Pengin
{
	class ECS;

	class FPSSystem final
	{
	public:
		FPSSystem(ECS& ecs) :
			m_ECS{ ecs }
		{}

		~FPSSystem() = default;

		void Update();

		FPSSystem(const FPSSystem&) = delete;
		FPSSystem(FPSSystem&&) = delete;
		FPSSystem& operator=(const FPSSystem&) = delete;
		FPSSystem& operator=(const FPSSystem&&) = delete;

	private:
		ECS& m_ECS;
	};
}

#endif
