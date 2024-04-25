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
			ProcessEvent(m_EventQueue.front());
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

	void EventManager::BroadcoastEvent(const Event& event) noexcept
	{
		m_EventQueue.emplace(event);
	}

	void EventManager::BroadcastBlockingEvent(const Event& event) noexcept
	{
		ProcessEvent(event);
	}
	
	void EventManager::ProcessEvent(const Event& event) noexcept
	{
		auto it{ m_EventCallbacks.find(event.GetEventName()) };

		if (it != end(m_EventCallbacks))
		{
			auto& observers{ it->second };

			std::erase_if(observers, [](const auto& observerPair)
				{
					return observerPair.first.expired();
				});

			for (auto& [observer, fCallback] : observers)
			{
				fCallback(event.GetEventData());
			}
		}
	}

	void EventManager::RegisterObserver(std::weak_ptr<Observer> pObserver, fEventCallback fCallback, const std::string& event) noexcept
	{
		auto& ObserverVec{ m_EventCallbacks[event] };
		ObserverVec.emplace_back(pObserver, fCallback);
	}
}

