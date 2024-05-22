#include "PengoInputCommands.h"

#include "HealthChangeEvent.h"
#include "ScoreChangeEvent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

void Pengo::Movement::Execute()
{
	using namespace Pengin;

	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();

	Entity playerEntity{ pActiveScene->GetPlayer(InputCommand::GetUserIdx()) };

	if (playerEntity.HasComponent<PengoComponent>())
	{
		const auto movementSpeed = playerEntity.GetComponent<PlayerComponent>().movementSpeed;
		playerEntity.GetComponent<BodyComponent>().inputVelocity += (m_Direction * movementSpeed);
	}
}

void Pengo::BreakBlock::Execute()
{
	using namespace Pengin;

	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();

	Entity playerEntity{ pActiveScene->GetPlayer(InputCommand::GetUserIdx()) };

	//if (playerEntity.HasComponent<PengoComponent>())
	//{
	//}
}

void Pengo::AttackPlayer::Execute()
{
	using namespace Pengin;

	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();
	Entity playerEntity{ pActiveScene->GetPlayer(InputCommand::GetUserIdx()) };

	if (!playerEntity)
	{
		DEBUG_OUT("attack for invalid player");
		return;
	}

	playerEntity.GetComponent<HealthComponent>().health--;

	EventManager::GetInstance().BroadcoastEvent(std::make_unique<HealthChangeEvent>("OnHealthChangeEvent", playerEntity.GetEntityId()));
}

void Pengo::CollectScore::Execute()
{
	using namespace Pengin;

	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();

	Entity playerEntity{ pActiveScene->GetPlayer(InputCommand::GetUserIdx()) };

	if (!playerEntity)
	{
		DEBUG_OUT("score for invalid player");
		return;
	}

	playerEntity.GetComponent<ScoreComponent>().score += m_ScoreVal;

	EventManager::GetInstance().BroadcoastEvent(std::make_unique<ScoreChangeEvent>("OnScoreCollectEvent", playerEntity.GetEntityId()));
}
