#include "PengoInputCommands.h"

#include "GameManager.h"
#include "EventManager.h"

void Pengo::PengoPlayGame::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("PlayGame"));
}

void Pengo::PengoSkipLevel::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("LevelWon"));
}
