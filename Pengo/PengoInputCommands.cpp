#include "PengoInputCommands.h"

#include "GameManager.h"
#include "EventManager.h"
#include "ServiceLocator.h"
#include "LevelWonEvent.h"

void Pengo::PengoPlayGame::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("PlayGame"));
}

void Pengo::PengoSkipLevel::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<LevelWonEvent>(GetUserIdx()));
}

void Pengo::MuteSounds::Execute()
{
	auto& pSoundSys = Pengin::ServiceLocator::GetSoundSystem();

	pSoundSys.IsMuted() ? pSoundSys.Unmute() : pSoundSys.Mute();
}
