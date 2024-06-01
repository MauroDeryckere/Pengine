#include "EnemySystem.h"
#include "ECS.h"
#include "SnobeeComponent.h"
#include "BlockSystem.h"
#include "BlockComponent.h"
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
		DEBUG_OUT("destroy snobee/block");
		m_ECS.DestroyEntity(entB);
		return;
	}
	if (m_ECS.HasComponent<SnobeeComponent>(entA) && m_ECS.HasComponent<BlockComponent>(entB))
	{
		DEBUG_OUT("destroy snobee/block");
		m_ECS.DestroyEntity(entA);
		return;
	}
}