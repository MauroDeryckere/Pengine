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
			m_Observer{ EventManager::GetInstance().CreateObserver<UIDisplayScoreComponent>(id) },
			m_Prefix{prefix},
			m_Score{0}

		{
			const std::string text{ m_Prefix + " " + std::to_string(m_Score) };

			auto& textComp = ECS::GetInstance().GetComponent<TextComponent>(m_Id);
			textComp.SetText(text);
			RegisterObservers();
		}

		void RegisterObservers()
		{
			auto callback = [this]() { OnScoreIncrease(); };
			m_Observer->RegisterForEvent("OnScoreCollect", callback);
		}

	private:
		const EntityId m_Id;
		unsigned m_Score;

		const std::string m_Prefix;

		std::shared_ptr<Observer> m_Observer;

		void OnScoreIncrease()
		{				
			m_Score += 10;

			const std::string text{ m_Prefix + " " + std::to_string(m_Score) };

			auto& textComp = ECS::GetInstance().GetComponent<TextComponent>(m_Id);
			textComp.SetText(text);
		}
	};
}

#endif