#include "EventManager.h"
#include <iostream>

namespace Pengin
{
	void EventManager::ProcessEvents()
	{
		while (!m_EventQueue.empty())
		{
			ProcessEvent(m_EventQueue.front());
			m_EventQueue.pop();
		}
	}

	void EventManager::BroadcastBlockingEvent(const Event& event)
	{
		ProcessEvent(event);
	}

	void EventManager::EnqueueEvent(const Event& event)
	{
		m_EventQueue.push(event);
	}

	void EventManager::RegisterObserver(const std::string& eventName, std::pair<std::shared_ptr<Observer>, fEventCallback> observer)
	{
		m_Observers[eventName].emplace_back(observer);
	}

	void EventManager::ProcessEvent(const Event& event)
	{
		auto it { m_Observers.find(event.GetName()) };

		if (it != end(m_Observers))
		{
			auto& observers{ it->second };

			std::erase_if(observers, [](const auto& observerPair) 
			{
				return observerPair.first.expired();
			});

			for (auto& [observer ,fCallback] : observers)
			{
				if (fCallback)
				{
					fCallback(event.GetData());
				}
				else
				{
					std::cout << "Im fucked \n";
				}
			}
		}
		//No listeners
	}
}


