#include "SnobeeWalkState.h"

#include "SwitchAnimationEvent.h"
#include "EventManager.h"
#include "EnemySystem.h"

void Pengo::SnobeeWalkState::OnEnter()
{

	Pengin::EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<Pengin::SwitchAnimationEvent>(GetEntityId(), 
																				static_cast<uint8_t>(Pengo::EnemySystem::EnemyAnimations::WalkDown)));
}

glm::vec2 Pengo::SnobeeWalkState::CalcDirection()
{
	return glm::vec2();
}
