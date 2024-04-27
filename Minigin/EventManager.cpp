#include "EventManager.h"
#include "DebugOutput.h"

#include <algorithm>
#include <cassert>
#include <mutex>
#include <ranges>
#include <iostream>

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
	}

	void EventManager::BroadcoastEvent(std::unique_ptr<BaseEvent> event) noexcept
	{
		m_EventQueue.emplace(std::move(event));
	}

	void EventManager::BroadcastBlockingEvent(std::unique_ptr<BaseEvent> event) noexcept
	{
		ProcessEvent(event.get());
	}

	void EventManager::EraseEvent(const std::string& eventName) noexcept
	{
		m_EventCallbacks.erase(eventName);
	}
	
	void EventManager::ProcessEvent(BaseEvent* event) noexcept
	{
		const auto eventName{ event->GetEventName() };

		auto it{ m_EventCallbacks.find(eventName) };

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

			return;
		}

		DEBUG_OUT(eventName << " event without callback(s)");
	}

	void EventManager::RegisterObserver(std::weak_ptr<Observer> pObserver, fEventCallback fCallback, const std::string& event) noexcept
	{
		auto& ObserverVec{ m_EventCallbacks[event] };
		ObserverVec.emplace_back(pObserver, fCallback);
	}
}

