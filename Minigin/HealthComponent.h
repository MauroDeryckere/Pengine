#ifndef HEALTHCOMPONENT
#define HEALTHCOMPONENT

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

		void TakeDamage(unsigned damage);

	private:
		unsigned m_Health;
	};
}

#endif
