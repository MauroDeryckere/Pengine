#include "PengoInputCommands.h"

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
