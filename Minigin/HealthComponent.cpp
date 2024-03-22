#include "HealthComponent.h"

#include "EventManager.h"
#include "EventManager_.h"

#include <iostream>
#include <algorithm>

namespace Pengin
{
	void HealthComponent::TakeDamage(unsigned damage, const std::string& event)
	{
		int newHp = m_Health - damage;
		m_Health = std::max(0, newHp);

		Event diedEvent{ event, &m_Health };
		EventManager_::GetInstance().BroadcoastEvent(diedEvent.GetName());
	}
}