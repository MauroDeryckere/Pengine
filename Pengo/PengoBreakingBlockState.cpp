#include "PengoBreakingBlockState.h"
#include "GameTime.h"

#include "EventManager.h"
#include "PengoComponent.h"
#include "PengoBlockBreakEvent.h"
#include "PengoIdleState.h"

namespace Pengo
{
	void PengoBreakingBlockState::OnEnter()
	{		
		using namespace Pengin;

		auto playerEntity{ Pengin::SceneManager::GetInstance().GetActiveScene()->GetPlayer(Pengin::PlayerState::GetUserIndex())};

		if (playerEntity)
		{
			if (m_Direction.x > 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushRight)));
			}
			else if (m_Direction.x < 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushLeft)));
			}
			else if (m_Direction.y > 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushDown)));
			}
			else if (m_Direction.y < 0.f)
			{
				EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<SwitchAnimationEvent>(
					playerEntity.GetEntityId(),
					static_cast<uint8_t>(PlayerSystem::PengoAnimations::PushUp)));
			}
		}
	}

	std::unique_ptr<Pengin::PlayerState> PengoBreakingBlockState::Update(const Pengin::UserIndex&)
	{
        using namespace Pengin;

        static float timer = 0.0f;

        float deltaTime = GameTime::GetInstance().ElapsedSec();
        timer += deltaTime;

        if (timer >= 1.0f)
        {
            timer = 0.0f;
            
            EventManager::GetInstance().BroadcoastEvent(std::make_unique<PengoBlockBreakEvent>(PlayerState::GetUserIndex()));
        }
		
		return nullptr;
	}


	void PengoBreakingBlockState::OnBlockBreakEvent(const Pengin::BaseEvent& event) 
	{
		using namespace Pengin;

		const auto& breakEv{ static_cast<const PengoBlockBreakEvent&>(event) };
		const auto& userIdx = breakEv.GetUserIndex();

		auto player = SceneManager::GetInstance().GetActiveScene()->GetPlayer(userIdx);

		if (player)
		{
			assert(player.HasComponent<PengoComponent>());
			player.GetComponent<PengoComponent>().SetPlayerState(std::make_unique<PengoIdleState>(userIdx, m_Direction));
		}
	}
}
