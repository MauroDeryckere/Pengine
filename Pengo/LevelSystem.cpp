#include "LevelSystem.h"
#include "GameTime.h"

void Pengo::LevelSystem::Update()
{
	using namespace Pengin;

	m_LevelTimer += GameTime::GetInstance().ElapsedSec();
}
