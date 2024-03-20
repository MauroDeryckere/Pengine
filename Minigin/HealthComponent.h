#ifndef HEALTHCOMPONENT
#define HEALTHCOMPONENT

#include <algorithm>
#include "EventManager.h"

#include <iostream>

namespace Pengin
{
	class HealthComponent final
	{
	public:
		HealthComponent(unsigned health) :
			m_Health{ health }
		{}

		~HealthComponent() = default; 

		[[nodiscard]] unsigned GetHealth() const { return m_Health; }

		void TakeDamage(unsigned damage)
		{
			int newHp = m_Health - damage;
			m_Health = std::max(0, newHp);


			Event diedEvent{ "PlayerDied" };
			EventManager::GetInstance().EnqueueEvent(diedEvent);

			std::cout << "health: " << m_Health<< "\n";
		}

	private:
		unsigned m_Health;
	};
}

#endif
