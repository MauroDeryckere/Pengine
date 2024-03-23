#ifndef UIDISPLAYHEALTHCOMPONENT
#define UIDISPLAYHEALTHCOMPONENT

#include "ECS.h"

#include "EventManager.h"

#include "TextComponent.h"
#include "HealthComponent.h"

#include <string>

namespace Pengin
{
	class UIDisplayHealthComponent final
	{
	public:
		UIDisplayHealthComponent(EntityId id, const std::string& eventName , const std::string& playerName, unsigned initHealth = 0) :
			m_Id{ id },
			m_Observer{ EventManager::GetInstance().CreateObserver<UIDisplayHealthComponent>(id) },
			m_PlayerName{ playerName },
			m_EventName{eventName}

		{
			std::cout << m_Id << "\n";
			auto& textComp{ ECS::GetInstance().GetComponent<TextComponent>(m_Id) };
			const std::string displayText { "Player: " + playerName + " Health: " + std::to_string(initHealth) };

			textComp.SetText(displayText);

			RegisterObservers();
		}

		void RegisterObservers()
		{
			auto callback = [&id = m_Id, &test = test]()
				{
					//const unsigned health{ (*static_cast<const unsigned*>(eventData)) };

					auto& textComp{ ECS::GetInstance().GetComponent<TextComponent>(id) };
					const std::string displayText{ "Player: Health: " + std::to_string(test) };
					
					++test;

					textComp.SetText(displayText);
				};

			m_Observer->RegisterForEvent(m_Observer, m_EventName, callback);
		}
		~UIDisplayHealthComponent()
		{
			m_Observer = nullptr;
		 }
	private:

		//A move construct here breaks the program, holds an "extra" strong reference to the m_Obs
		EntityId m_Id;
		std::shared_ptr<Observer> m_Observer; //Problem that this captures the weak ptr?

		int test = 0;

		std::string m_PlayerName;
		std::string m_EventName;
	};
}

#endif