#ifndef DEBUGDRAWSYSTEM
#define DEBUGDRAWSYSTEM

#include "BaseSystem.h"
#include "UtilStructs.h"

namespace Pengin
{
	class ECS;

	class DebugDrawSystem final : public BaseSystem
	{
	public:
		DebugDrawSystem(ECS& ecs) :
			BaseSystem{ },
			m_ECS{ ecs }
		{}

		virtual ~DebugDrawSystem() override = default;

		virtual void Render() const override;

		DebugDrawSystem(const DebugDrawSystem&) = delete;
		DebugDrawSystem(DebugDrawSystem&&) = delete;
		DebugDrawSystem& operator=(const DebugDrawSystem&) = delete;
		DebugDrawSystem& operator=(const DebugDrawSystem&&) = delete;

	private:
		ECS& m_ECS;
	};
}

#endif
