#include "EventManager.h"
#include <algorithm>
#include <cassert>
#include <ranges>

namespace Pengin
{
	void EventManager::ProcessEventQueue()
	{		
		while (!m_EventQueue.empty())
		{
			ProcessEvent(m_EventQueue.front());
			m_EventQueue.pop();
		}
	}

	void EventManager::BroadcoastEvent(const Event& event)
	{
		m_EventQueue.emplace(event);
	}

	void EventManager::BroadcastBlockingEvent(const Event& event)
	{
		ProcessEvent(event);
	}
	
	void EventManager::ProcessEvent(const Event& event)
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
				auto pObs{ observer.lock() };

				if (pObs->IsDirty())
				{
					pObs->RegisterCallbacks();
				}

				fCallback(event.GetEventData());
			}
		}
	}

	void EventManager::RegisterObserver(std::weak_ptr<Observer> pObserver, fEventCallback fCallback, const std::string& event)
	{
		auto obs = pObserver.lock();

		const ObserverIdentifier identifier{ obs->GetEntityId(), obs->GetTypeIdx() };
		m_Observers[identifier] = pObserver;
		
		auto& ObserverVec{ m_EventCallbacks[event] };

		auto it = std::ranges::find_if(ObserverVec, [obs](const auto& pair)
			{
				return pair.first.lock() == obs;
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
		auto it = m_Observers.find({entiyId, typeIdx});

		if (it == m_Observers.end()) //TODO: this check could get heavy for very large componentSets, maybe could be improved upon
		{
			//std::cerr << "trying to set obs dirty that isnt in observer map \n";
			return;
		}

		auto pObs = it->second.lock();
		
		if (pObs)
		{
			pObs->SetDirty();
			return;
		}

		m_Observers.erase(it);
	}
}

