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
		UIDisplayScoreComponent(EntityId id, const std::string& prefix) :
			m_Id{ id },
			m_Observer{ std::make_shared<Observer>() },
			m_Prefix{prefix},
			m_Score{0}

		{
			assert( ECS::GetInstance().HasComponent<TextComponent>(m_Id) );

			auto callback = [this](const void* eventData) { OnScoreIncrease(eventData); };
			m_Observer->RegisterForEvent("OnScoreCollect", callback);

			const std::string text{ m_Prefix + " " + std::to_string(m_Score) };

			auto& textComp = ECS::GetInstance().GetComponent<TextComponent>(m_Id);
			textComp.SetText(text);
		}

	private:
		const EntityId m_Id;
		unsigned m_Score;

		const std::string m_Prefix;

		std::shared_ptr<Observer> m_Observer;

		void OnScoreIncrease(const void* eventData)
		{
			const int scoreChange{ (*static_cast<const int*>(eventData)) };
				
			m_Score += scoreChange;

			const std::string text{ m_Prefix + " " + std::to_string(m_Score) };

			auto& textComp = ECS::GetInstance().GetComponent<TextComponent>(m_Id);
			textComp.SetText(text);
		}
	};
}

#endif