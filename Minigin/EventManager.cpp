#include "EventManager.h"
#include "EventQueue.h"

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

	void EventManager::BroadcastEvent(const Event& event)
	{
		m_EventQueue->AddEvent(event);
	}

	void EventManager::RegisterObserver(const std::string& eventName, std::pair<std::weak_ptr<Observer>, fEventCallback> observer)
	{
		m_Observers[eventName].emplace_back(observer);
	}

	void EventManager::ProcessEvent(const Event& event)
	{
		auto it { m_Observers.find(event.GetName()) };

		if (it != end(m_Observers))
		{
			auto [evName, observers] {*it};
			
			for (auto& [observer ,fCallback] : observers)
			{
				if (auto pObserver{ observer.lock() }; pObserver)
				{
					fCallback(event.GetData());
				}
				else
				{
					std::cout << "weakptr  cant lock for " << event.GetName() << "\n";
				}
			}
		}
	}
}


