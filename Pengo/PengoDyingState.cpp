#include "PengoDyingState.h"

#include "GameTime.h"

#include "EventManager.h"
#include "PengoRespawnEvent.h"
#include "HealthComponent.h"

namespace Pengo
{
	void PengoDyingState::OnEnter()
	{
		using namespace Pengin;

		auto playerEntity{ Pengin::SceneManager::GetInstance().GetActiveScene()->GetPlayer(Pengin::PlayerState::GetUserIndex()) };

		if (playerEntity)
		{
			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(playerEntity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::Dying)));
		}
	}

	std::unique_ptr<Pengin::PlayerState> PengoDyingState::Update(const Pengin::UserIndex& userIndex)
	{
		using namespace Pengin;

		float deltaTime = GameTime::GetInstance().ElapsedSec();
		m_Timer += deltaTime;

		if (m_Timer >= 1.0f)
		{
			auto playerEntity{ Pengin::SceneManager::GetInstance().GetActiveScene()->GetPlayer(Pengin::PlayerState::GetUserIndex()) };
			if (playerEntity.GetComponent<HealthComponent>().health > 0)
			{
				EventManager::GetInstance().BroadcoastEvent(std::make_unique<PengoRespawnEvent>(userIndex));
			}
			else
			{
				//GAME OVER
			}

		}
		return nullptr;;
	}
}
