#include "LevelSystem.h"
#include "GameTime.h"
#include "EventManager.h"

void Pengo::LevelSystem::Update()
{
	using namespace Pengin;

	m_LevelTimer += GameTime::GetInstance().ElapsedSec();
	
	if (m_LevelTimer >= 20.f)
	{
		//EventManager::GetInstance().BroadcoastEvent(std::make_unique<BaseEvent>("NextLevel"));
	}
}
