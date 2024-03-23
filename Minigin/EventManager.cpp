#include "EventManager.h"
#include <algorithm>
#include <cassert>
#include <ranges>

namespace Pengin
{
	void EventManager::ProcessEventQueue()
	{
		//For now set all dirty - this logic has to be inside ECS later - TODO
		SetObserverDirty(1, typeid(EventManager));

		while (!m_EventQueue.empty())
		{
			ProcessEvent(m_EventQueue.front());
			m_EventQueue.pop();
		}
	}

	void EventManager::BroadcoastEvent(const std::string& event)
	{
		m_EventQueue.emplace(event);
	}

	void EventManager::BroadcastBlockingEvent(const std::string& event)
	{
		ProcessEvent(event);
	}
	
	void EventManager::ProcessEvent(const std::string& event)
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

	void EventManager::RegisterObserver(std::shared_ptr<Observer> pObserver, fEventCallback fCallback, const std::string& event)
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

	void EventManager::SetObserverDirty(EntityId entiyId, std::type_index typeIdx)
	{
		entiyId;
		typeIdx;

		for (auto& obsPair : m_Observers) //Set specific obs dirty
		{
			obsPair.second.lock()->SetDirty();
		}
	}
}

