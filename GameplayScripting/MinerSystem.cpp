#include "MinerSystem.h"

#include "SceneManager.h"
#include "LevelManager.h"

#include "MinerComponent.h"
#include "BodyComponent.h"

#include "TransformComponent.h"

void GS::MinerSystem::Update()
{
	using namespace Pengin;

	auto miner = m_ECS.GetComponents<MinerComponent>();
	for (auto it = miner.begin(); auto& m : miner )
	{
		const EntityId id = miner.GetIdFromIterator(it);

		m_ECS.GetComponent<BodyComponent>(id).inputVelocity += glm::vec3{ 0.f, 1.f, 0.f } * static_cast<float>(m.totalWeight);

		const auto& transform = m_ECS.GetComponent<TransformComponent>(id);
		if (transform.worldPos.x < 0.f || transform.worldPos.x > 1280.f || transform.worldPos.y < 0.f || transform.worldPos.y > 720.f)
		{
			EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("PlayerDeath"));
		}

		++it;
	}
}

void GS::MinerSystem::OnPlayerDeath()
{
	using namespace Pengin;

	EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("LoadRestart"));
}
