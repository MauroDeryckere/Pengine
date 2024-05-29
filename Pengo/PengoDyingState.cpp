#include "PengoDyingState.h"

#include "GameTime.h"

#include "EventManager.h"
#include "PengoRespawnEvent.h"

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

		DEBUG_OUT("Enter Dying");
	}

	std::unique_ptr<Pengin::PlayerState> PengoDyingState::Update(const Pengin::UserIndex& userIndex)
	{
		//if health >= 0 else broadcast gameover Event
		using namespace Pengin;

		static float timer = 0.0f;

		float deltaTime = GameTime::GetInstance().ElapsedSec();
		timer += deltaTime;

		if (timer >= 1.0f)
		{
			timer = 0.0f;

			EventManager::GetInstance().BroadcoastEvent(std::make_unique<PengoRespawnEvent>(userIndex));
		}

		return nullptr;;
	}
}
