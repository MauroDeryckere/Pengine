#include "FactorySystem.h"

#include "FactoryComponent.h"
#include "MinerComponent.h"

#include "InputManager.h"

void GS::FactorySystem::OnCollision(const Pengin::BaseEvent& event)
{
	using namespace Pengin;

	const auto& collEv = static_cast<const CollisionEvent&>(event);

	const EntityId entA = collEv.GetEntityA();
	const EntityId entB = collEv.GetEntityB();

	if (m_ECS.HasComponent<FactoryComponent>(entB) && m_ECS.HasComponent<MinerComponent>(entA))
	{
		EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("LoadRestart"));

		return;
	}

	if (m_ECS.HasComponent<FactoryComponent>(entA) && m_ECS.HasComponent<MinerComponent>(entB))
	{
		EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("LoadRestart"));

		return;
	}
}