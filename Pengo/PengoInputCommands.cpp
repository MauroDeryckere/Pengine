#include "PengoInputCommands.h"

#include "GameManager.h"

void Pengo::PengoPlayGame::Execute()
{
	GameManager::GetInstance().PlayGame();
}
