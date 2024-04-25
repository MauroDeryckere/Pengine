#include "EventManager.h"
#include <algorithm>
#include <cassert>
#include <ranges>
#include <iostream>

#include "ThreadManager.h"
#include "ServiceLocator.h"

namespace Pengin
{
	void EventManager::ProcessEventQueue() noexcept
	{		
		while (!m_EventQueue.empty())
		{
			ProcessEvent(m_EventQueue.front().get());
			m_EventQueue.pop();
		}

		//If soundevent
		//add soundev to queue - TEMP HARDCODED
		static int i{ 0 };

		if (i == 2)
		{
			ThreadManager::GetInstance().EnqueueSoundTask([]()
				{
					std::cout << "TEST \n";
					ServiceLocator::GetSoundSystem().LoadSound("../Data/TestSound.wav", true, true);
					ServiceLocator::GetSoundSystem().PlaySounds("../Data/TestSound.wav");
				}
			);
		}

		i++;

		//-------------------------------------
	}

	void EventManager::BroadcoastEvent(std::unique_ptr<BaseEvent> event) noexcept
	{
		m_EventQueue.emplace(std::move(event));
	}

	void EventManager::BroadcastBlockingEvent(std::unique_ptr<BaseEvent> event) noexcept
	{
		ProcessEvent(event.get());
	}
	
	void EventManager::ProcessEvent(BaseEvent* event) noexcept
	{
		auto it{ m_EventCallbacks.find(event->GetEventName()) };

		if (it != end(m_EventCallbacks))
		{
			auto& observers{ it->second };

			std::erase_if(observers, [](const auto& observerPair)
				{
					return observerPair.first.expired();
				});

			for (auto& [observer, fCallback] : observers)
			{
				fCallback(*event);	
			}
		}
	}

	void EventManager::RegisterObserver(std::weak_ptr<Observer> pObserver, fEventCallback fCallback, const std::string& event) noexcept
	{
		auto& ObserverVec{ m_EventCallbacks[event] };
		ObserverVec.emplace_back(pObserver, fCallback);
	}
}

