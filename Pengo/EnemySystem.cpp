#include "EnemySystem.h"
#include "ECS.h"
#include "SnobeeComponent.h"

void Pengo::EnemySystem::Update()
{
	for (auto& comp : m_ECS.GetComponents<SnobeeComponent>())
	{
		comp.UpdateState();
	}
}
