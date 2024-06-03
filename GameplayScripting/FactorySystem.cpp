#include "FactorySystem.h"

#include "FactoryComponent.h"
#include "MinerComponent.h"
#include "PlayerComponent.h"

#include "InputManager.h"

void GS::FactorySystem::OnCollision(const Pengin::BaseEvent& event)
{
	using namespace Pengin;

	const auto& collEv = static_cast<const CollisionEvent&>(event);

	const EntityId entA = collEv.GetEntityA();
	const EntityId entB = collEv.GetEntityB();

	if (m_ECS.HasComponent<FactorySystem>(entB) && m_ECS.HasComponent<MinerComponent>(entA))
	{
		const auto& playerComp = m_ECS.GetComponent<PlayerComponent>(entA);

		if (InputManager::GetInstance().IsActionExecuted(playerComp.userIdx, "Mine"))
		{
			EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("LoadRestart"));
		}

		return;
	}

	if (m_ECS.HasComponent<FactorySystem>(entA) && m_ECS.HasComponent<MinerComponent>(entB))
	{
		const auto& playerComp = m_ECS.GetComponent<PlayerComponent>(entA);

		if (InputManager::GetInstance().IsActionExecuted(playerComp.userIdx, "Mine"))
		{
			EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("LoadRestart"));
		}

		return;
	}
}
