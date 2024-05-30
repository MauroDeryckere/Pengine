#include "SnobeeSpawningState.h"

#include "GameTime.h"
#include "SnobeeWalkState.h"


std::unique_ptr<Pengin::State> Pengo::SnobeeSpawningState::Update()
{
	static float timer{ 0.f };

	timer += Pengin::GameTime::GetInstance().ElapsedSec();

	if (timer >= 3.f)
	{
		timer = 0.f;

		return std::move(std::make_unique<SnobeeWalkState>(GetEntityId()));
	}

	return nullptr;
}
