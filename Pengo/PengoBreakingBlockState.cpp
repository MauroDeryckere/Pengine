#include "PengoBreakingBlockState.h"
#include "GameTime.h"

#include "EventManager.h"
#include "PengoBlockBreakEvent.h"

namespace Pengo
{
	std::unique_ptr<PlayerState> PengoBreakingBlockState::Update(const UserIndex&)
	{
        static float timer = 0.0f;

        float deltaTime = GameTime::GetInstance().GetElapsedSec();
        timer += deltaTime;

        if (timer >= 1.0f)
        {
            timer = 0.0f;
            
            EventManager::GetInstance().BroadcoastEvent(std::make_unique<PengoBlockBreakEvent>(PlayerState::GetUserIndex()));
        }
		
		return nullptr;
	}
}
