#include "SnobeeSpawningState.h"

#include "GameTime.h"
#include "SnobeeWalkState.h"


std::unique_ptr<Pengin::State> Pengo::SnobeeSpawningState::Update()
{
	m_Timer += Pengin::GameTime::GetInstance().ElapsedSec();

	if (m_Timer >= 3.f)
	{
		return std::move(std::make_unique<SnobeeWalkState>(GetEntityId()));
	}

	return nullptr;
}
