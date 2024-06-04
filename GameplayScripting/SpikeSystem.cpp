#include "SpikeSystem.h"

#include "MinerComponent.h"
#include "SpikeComponent.h"

void GS::SpikeSystem::OnCollision(const Pengin::BaseEvent& event)
{
	using namespace Pengin;

	const auto& collEv = static_cast<const CollisionEvent&>(event);

	const EntityId entA = collEv.GetEntityA();
	const EntityId entB = collEv.GetEntityB();

	if (m_ECS.HasComponent<SpikeComponent>(entB) && m_ECS.HasComponent<MinerComponent>(entA))
	{
		EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("LoadRestart"));
		return;
	}

	if (m_ECS.HasComponent<SpikeComponent>(entA) && m_ECS.HasComponent<MinerComponent>(entB))
	{
		EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("LoadRestart"));
		return;
	}
}
