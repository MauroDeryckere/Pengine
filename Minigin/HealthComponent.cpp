#include "HealthComponent.h"

#include "EventManager.h"

#include <iostream>
#include <algorithm>

namespace Pengin
{
	void HealthComponent::TakeDamage(unsigned damage)
	{
		int newHp = m_Health - damage;
		m_Health = std::max(0, newHp);


		Event diedEvent{ "PlayerDied" };
		EventManager::GetInstance().EnqueueEvent(diedEvent);

		std::cout << "health: " << m_Health << "\n";
	}
}