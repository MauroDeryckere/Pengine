#include "PengoPushingBlockState.h"

#include "PengoIdleState.h"
#include "GameTime.h"

void Pengo::PengoPushingBlockState::OnEnter()
{
	using namespace Pengin;
	
	ServiceLocator::GetSoundSystem().PlaySound({ "../Data/Audio/Push Ice Block.mp3" });

	auto playerEntity{ Pengin::SceneManager::GetInstance().GetActiveScene()->GetPlayer(Pengin::PlayerState::GetUserIndex()) };

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

std::unique_ptr<Pengin::PlayerState> Pengo::PengoPushingBlockState::Update(const Pengin::UserIndex& userIndex)
{
	using namespace Pengin;

	m_Timer += GameTime::GetInstance().ElapsedSec();

	if (m_Timer >= 0.5f)
	{
		return std::make_unique<PengoIdleState>(userIndex, m_Direction);
	}

	return nullptr;
}
