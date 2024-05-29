#include "PengoBreakingBlockState.h"
#include "GameTime.h"

#include "EventManager.h"
#include "PengoBlockBreakEvent.h"

namespace Pengo
{
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
}
