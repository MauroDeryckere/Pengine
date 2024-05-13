#include "PengoDyingState.h"

#include "GameTime.h"

#include "EventManager.h"
#include "PengoRespawnEvent.h"

namespace Pengo
{
	std::unique_ptr<PlayerState> PengoDyingState::Update(const UserIndex& userIndex)
	{
		static float timer = 0.0f;

		float deltaTime = GameTime::GetInstance().GetElapsedSec();
		timer += deltaTime;

		if (timer >= 1.0f)
		{
			timer = 0.0f;

			EventManager::GetInstance().BroadcoastEvent(std::make_unique<PengoRespawnEvent>(userIndex));
		}

		return nullptr;;
	}
}
