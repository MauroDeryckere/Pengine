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
			m_Observer{ EventManager::GetInstance().CreateComponentObserver<UIDisplayHealthComponent>(id) },
			m_PlayerName{ playerName },
			m_EventName{eventName}

		{
			auto& textComp{ ECS::GetInstance().GetComponent<TextComponent>(m_Id) };
			const std::string displayText { "Player: Health: " + std::to_string(initHealth) };

			textComp.SetText(displayText);

			RegisterObservers();
		}

		void RegisterObservers()
		{
			auto callback = [this](const void* eventData)
				{
					const unsigned health{ (*static_cast<const unsigned*>(eventData)) };

					auto& textComp{ ECS::GetInstance().GetComponent<TextComponent>(m_Id) };
					const std::string displayText{ "Player: Health: " + std::to_string(health) };
					
					textComp.SetText(displayText);
				};

			m_Observer->RegisterForEvent(m_Observer, m_EventName, callback);
		}

	private:
		const EntityId m_Id;
		std::shared_ptr<CompObserver> m_Observer;

		const std::string m_PlayerName;
		const std::string m_EventName;
	};
}

#endif