#ifndef UIDISPLAYSCORECOMPONENT
#define UIDISPLAYSCORECOMPONENT

#include "ECS.h"
#include "EventManager.h"
#include "TextComponent.h"

#include <cassert>
#include <string>

namespace Pengin
{
	class UIDisplayScoreComponent final
	{
	public:
		UIDisplayScoreComponent(EntityId id, const std::string& prefix, const std::string& eventName) :
			m_Id{ id },
			m_Observer{ EventManager::GetInstance().CreateObserver<UIDisplayScoreComponent>(id) },
			m_Prefix{prefix},
			m_EventName{ eventName },
			m_Score{0}
		{
			const std::string text{ m_Prefix + " " + std::to_string(m_Score) };

			auto& textComp = ECS::GetInstance().GetComponent<TextComponent>(m_Id);
			textComp.SetText(text);
			RegisterObservers();
		}

		void RegisterObservers()
		{
			auto callback = [this](const void* eventData) { OnScoreIncrease(eventData); };
			m_Observer->RegisterForEvent(m_Observer, m_EventName, callback);
		}

	private:
		const EntityId m_Id;
		std::shared_ptr<Observer> m_Observer;

		const std::string m_Prefix;
		const std::string m_EventName;
		unsigned m_Score;

		void OnScoreIncrease(const void* eventData)
		{			
			const unsigned scoreIncrease{ (*static_cast<const unsigned*>(eventData)) };

			m_Score += scoreIncrease;

			if (m_Score >= 500)
			{
				const Event achievement{"ScoreAchievement"};
				EventManager::GetInstance().BroadcoastEvent(achievement);
			}

			const std::string text{ m_Prefix + " " + std::to_string(m_Score) };

			auto& textComp = ECS::GetInstance().GetComponent<TextComponent>(m_Id);
			textComp.SetText(text);
		}
	};
}

#endif