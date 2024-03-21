#ifndef UIDISPLAYCOMPONENT
#define UIDISPLAYCOMPONENT

#include "ECS.h"

#include "EventManager.h"
#include "TextComponent.h"
#include "HealthComponent.h"

#include <string>

namespace Pengin
{
	class UIDisplayComponent final
	{
	public:
		UIDisplayComponent(EntityId id, const std::string& playerName, unsigned initHealth = 0) :
			m_Id{ id },
			m_Observer{ std::make_shared<Observer>("DisplayComponent") },
			m_PlayerName{ playerName }
		{
			auto& textComp{ ECS::GetInstance().GetComponent<TextComponent>(m_Id) };
			const std::string displayText { "Player: " + playerName + " Health: " + std::to_string(initHealth) };

			textComp.SetText(displayText);

			auto callback = [this](const void* eventData) { OnPlayerDeath(eventData); };
			m_Observer->RegisterForEvent("OnHealthChange", callback);
		}

	private:
		const EntityId m_Id;
		std::shared_ptr<Observer> m_Observer;

		const std::string m_PlayerName;

		void OnPlayerDeath(const void* eventData) const
		{
			const unsigned health{ (*static_cast<const unsigned*>(eventData)) };

			auto& textComp{ ECS::GetInstance().GetComponent<TextComponent>(m_Id) };
			const std::string displayText{ "Player: " + m_PlayerName + " Health: " + std::to_string(health) };

			textComp.SetText(displayText);
		}
	};
}

#endif