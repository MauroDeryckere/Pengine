#ifndef DISPLAYCOMPONENT
#define DISPLAYCOMPONENT

#include "ECS.h"

#include "EventManager.h"
#include "TextComponent.h"
#include "HealthComponent.h"

#include <string>

namespace Pengin
{
	class DisplayComponent final
	{
	public:
		DisplayComponent(EntityId thisId, EntityId playerId, const std::string& playerName) :
			m_ThisId{ thisId },
			m_PlayerId{ playerId },
			m_Observer{ std::make_shared<Observer>("DisplayComponent") }
		{
			auto& textComp{ ECS::GetInstance().GetComponent<TextComponent>(m_ThisId) };
			auto& healthComp{ ECS::GetInstance().GetComponent<HealthComponent>(m_PlayerId) };

			const std::string displayText = "Player: " + playerName + " Health: " + std::to_string(healthComp.GetHealth());

			textComp.SetText(displayText);

			auto callback = [this](const void* eventData) { OnPlayerDeath(eventData); };
			m_Observer->RegisterForEvent("PlayerDied", callback);
		}

	private:
		const EntityId m_ThisId;
		const EntityId m_PlayerId;
		std::shared_ptr<Observer> m_Observer;

		void OnPlayerDeath(const void*) const
		{
			auto& textComp{ ECS::GetInstance().GetComponent<TextComponent>(m_ThisId) };
			const auto& text = textComp.GetText();

			std::string newText = text;

			auto& healthComp{ ECS::GetInstance().GetComponent<HealthComponent>(m_PlayerId) };
			size_t digitPos = newText.find_first_of("0123456789");
			if (digitPos != std::string::npos)
			{
				newText.replace(digitPos, std::string::npos, std::to_string(healthComp.GetHealth()));
			}

			textComp.SetText(newText);
		}
	};
}

#endif