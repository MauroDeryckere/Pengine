#include "OreSystem.h"

#include "OreComponent.h"
#include "MinerComponent.h"
#include "PlayerComponent.h"

#include "InputManager.h"

void GS::OreSytem::OnCollision(const Pengin::BaseEvent& event)
{
	using namespace Pengin;

	const auto& collEv = static_cast<const CollisionEvent&>(event);
	
	const EntityId entA = collEv.GetEntityA();
	const EntityId entB = collEv.GetEntityB();

	if (m_ECS.HasComponent<OreComponent>(entB) && m_ECS.HasComponent<MinerComponent>(entA))
	{
		const auto& playerComp = m_ECS.GetComponent<PlayerComponent>(entA);

		if (InputManager::GetInstance().IsActionExecuted(playerComp.userIdx, "Mine"))
		{
			auto& minerComp = m_ECS.GetComponent<MinerComponent>(entA);
			auto& OreComp = m_ECS.GetComponent<OreComponent>(entB);

			minerComp.ores.emplace_back(OreComp.weight);
			minerComp.totalWeight += OreComp.weight;

			EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("MinerOreChange"));

			m_ECS.DestroyEntity(entB);
		}

		return;
	}

	if (m_ECS.HasComponent<OreComponent>(entA) && m_ECS.HasComponent<MinerComponent>(entB))
	{
		const auto& playerComp = m_ECS.GetComponent<PlayerComponent>(entB);

		if (InputManager::GetInstance().IsActionExecuted(playerComp.userIdx, "Mine"))
		{
			auto& minerComp = m_ECS.GetComponent<MinerComponent>(entB);
			auto& OreComp = m_ECS.GetComponent<OreComponent>(entA);

			minerComp.ores.emplace_back(OreComp.weight);
			minerComp.totalWeight += OreComp.weight;

			EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("MinerOreChange"));

			m_ECS.DestroyEntity(entA);
		}

		return;
	}
}
