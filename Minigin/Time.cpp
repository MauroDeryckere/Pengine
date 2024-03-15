#include "Time.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration;

void Pengin::Time::Update()
{
	const auto currentTime{ high_resolution_clock::now() };
	m_ElapsedSec = duration<float>(currentTime - m_LastTime).count();

	m_LastTime = currentTime;
	m_Lag += m_ElapsedSec;
}