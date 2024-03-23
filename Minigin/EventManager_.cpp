#include "EventManager_.h"
#include <algorithm>
#include <cassert>
#include <ranges>

namespace Pengin
{
	void EventManager_::ProcessEventQueue()
	{
		//For now set all dirty - this logic has to be inside ECS later - TODO
		SetObserverDirty(); 

		while (!m_EventQueue.empty())
		{
			ProcessEvent(m_EventQueue.front());
			m_EventQueue.pop();
		}
	}

	void EventManager_::BroadcoastEvent(const std::string& event)
	{
		m_EventQueue.emplace(event);
	}

	void EventManager_::BroadcastBlockingEvent(const std::string& event)
	{
		ProcessEvent(event);
	}
	
	void EventManager_::ProcessEvent(const std::string& event)
	{
		auto it{ m_EventCallbacks.find(event) };

		if (it != end(m_EventCallbacks))
		{
			auto& observers{ it->second };

			std::erase_if(observers, [](const auto& observerPair)
				{
					return observerPair.first.expired();
				});

			for (auto& [observer, fCallback] : observers)
			{
				auto pObs{ observer.lock() };

				if (pObs->IsDirty())
				{
					pObs->RegisterCallbacks();
				}
				fCallback();
			}
		}
	}

	void EventManager_::RegisterObserver(std::shared_ptr<Observer_> pObserver, fEventCallback fCallback, const std::string& event)
	{
		const ObserverIdentifier identifier{ pObserver->GetEntityId(), pObserver->GetTypeIdx() };
		m_Observers[identifier] = pObserver;
		
		auto& ObserverVec{ m_EventCallbacks[event] };

		auto it = std::ranges::find_if(ObserverVec, [pObserver](const auto& pair) 
			{
				return pair.first.lock() == pObserver;
			});

		if (it != ObserverVec.end())
		{
			*it = { pObserver, fCallback };
			return;
		}

		ObserverVec.emplace_back(pObserver, fCallback);
	}

	void EventManager_::SetObserverDirty()
	{
		for (auto& obsPair : m_Observers) //Set specific obs dirty
		{
			obsPair.second.lock()->SetDirty();
		}
	}
}

