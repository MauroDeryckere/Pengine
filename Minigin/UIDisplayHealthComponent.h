#ifndef UIDISPLAYHEALTHCOMPONENT
#define UIDISPLAYHEALTHCOMPONENT

#include "ECS.h"

#include "EventManager_.h"

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
			m_Observer{ EventManager_::GetInstance().CreateObserver<UIDisplayHealthComponent>(id) },
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
			auto callback = [this]()
				{
					//const unsigned health{ (*static_cast<const unsigned*>(eventData)) };

					auto& textComp{ ECS::GetInstance().GetComponent<TextComponent>(m_Id) };
					const std::string displayText{ "Player: Health: " + std::to_string(test) };

					++test;

					textComp.SetText(displayText);
				};

			m_Observer->RegisterForEvent(m_EventName, callback);
		}

	private:
		const EntityId m_Id;
		std::shared_ptr<Observer_> m_Observer;

		int test = 0;

		const std::string m_PlayerName;
		const std::string m_EventName;
	};
}

#endif