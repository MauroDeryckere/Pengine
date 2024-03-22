#ifndef HEALTHCOMPONENT
#define HEALTHCOMPONENT

#include <string>

namespace Pengin
{
	class HealthComponent final
	{
	public:
		HealthComponent(unsigned health) :
			m_Health{ health }
		{}

		~HealthComponent() = default; 

		void TakeDamage(unsigned damage, const std::string& event);

	private:
		unsigned m_Health;
	};
}

#endif
