#include "EventManager.h"

#include <iostream>
namespace Pengin
{
	EventManager::EventManager() :
		m_EventQueue{ std::make_unique<EventQueue>() },
		m_Observers{}
	{

	}

	void EventManager::ProcessEvents()
	{
		m_EventQueue->ProcessEvents();
	}

	void EventManager::BroadcastEvent(const std::string& eventName, const void* eventData)
	{
		m_EventQueue->AddEvent(eventName, eventData);
	}

	void EventManager::RegisterObserver(const std::string& eventName, std::pair<std::weak_ptr<Observer>, std::function<void(const void*)>> observer)
	{
		m_Observers[eventName].emplace_back(observer);
	}

	void EventManager::ProcessEvent(const std::string& eventName, const void* eventData)
	{
		auto it { m_Observers.find(eventName) };

		if (it != end(m_Observers))
		{
			auto [evName, observers] {*it};
			
			for (auto& [observer ,fCallback] : observers)
			{
				if (auto pObserver{ observer.lock() }; pObserver)
				{
					fCallback(eventData);
				}
				else
				{
					std::cout << "weakptr  cant lock for " << eventName << "\n";
				}
			}
		}
	}
}


