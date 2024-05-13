#include "PengoDyingState.h"

#include "GameTime.h"

#include "EventManager.h"
#include "PengoRespawnEvent.h"

namespace Pengo
{
	void PengoDyingState::OnEnter()
	{
		using namespace Pengin;

		auto& sceneData = SceneManager::GetInstance().GetActiveScene()->GetSceneData();

		auto it = sceneData.user_UUIDVecIdxMap.find(GetUserIndex());

		if (it != sceneData.user_UUIDVecIdxMap.end())
		{
			auto entity = SceneManager::GetInstance().GetActiveScene()->GetEntity(sceneData.playerUUIDs[it->second]);
			EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(entity.GetEntityId(), static_cast<uint8_t>(PlayerSystem::PengoAnimations::Dying)));
		}

		DEBUG_OUT("Enter Dying");
	}

	std::unique_ptr<Pengin::PlayerState> PengoDyingState::Update(const Pengin::UserIndex& userIndex)
	{
		using namespace Pengin;

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
