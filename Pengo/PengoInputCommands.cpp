#include "PengoInputCommands.h"

#include "GameManager.h"
#include "EventManager.h"
#include "LevelWonEvent.h"

void Pengo::PengoPlayGame::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("PlayGame"));
}

void Pengo::PengoSkipLevel::Execute()
{
	std::cout << "skip \n",
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<LevelWonEvent>(GetUserIdx()));
}
