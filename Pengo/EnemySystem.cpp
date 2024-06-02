#include "EnemySystem.h"
#include "ECS.h"
#include "SnobeeComponent.h"
#include "BlockSystem.h"
#include "BlockComponent.h"
#include "SnobeeDeathEvent.h"
#include "CollisionEvent.h"

void Pengo::EnemySystem::Update()
{
	for (auto& comp : m_ECS.GetComponents<SnobeeComponent>())
	{
		comp.UpdateState();
	}
}

void Pengo::EnemySystem::OnCollision(const Pengin::BaseEvent& event)
{
	using namespace Pengin;

	const auto& collEv{ static_cast<const CollisionEvent&>(event) };

	const auto entA = collEv.GetEntityA();
	const auto entB = collEv.GetEntityB();
	
	if (m_ECS.HasComponent<SnobeeComponent>(entB) && m_ECS.HasComponent<BlockComponent>(entA))
	{	
		auto& block = m_ECS.GetComponent<BlockComponent>(entA);
		if (block.blockState == BlockComponent::BlockState::Moving)
		{
			EventManager::GetInstance().BroadcoastEvent(std::make_unique<SnobeeDeathEvent>(block.pusherId));

			block.pusherId = NULL_ENTITY_ID;

			ServiceLocator::GetSoundSystem().PlaySound({"../Data/Audio/Snow-Bee Squashed.mp3"});

			m_ECS.DestroyEntity(entB);
		}
		return;
	}
	if (m_ECS.HasComponent<SnobeeComponent>(entA) && m_ECS.HasComponent<BlockComponent>(entB))
	{
		auto& block = m_ECS.GetComponent<BlockComponent>(entB);
		if (block.blockState == BlockComponent::BlockState::Moving)
		{
			EventManager::GetInstance().BroadcoastEvent(std::make_unique<SnobeeDeathEvent>(block.pusherId));

			block.pusherId = NULL_ENTITY_ID;

			ServiceLocator::GetSoundSystem().PlaySound({ "../Data/Audio/Snow-Bee Squashed.mp3" });

			m_ECS.DestroyEntity(entA);
		}
		return;
	}
}