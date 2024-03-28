#include "EventManager.h"
#include <algorithm>
#include <cassert>
#include <ranges>

namespace Pengin
{
	void EventManager::ProcessEventQueue() noexcept
	{		
		while (!m_EventQueue.empty())
		{
			ProcessEvent(m_EventQueue.front());
			m_EventQueue.pop();
		}
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
		auto compIt{ m_CompEventCallbacks.find(event.GetEventName()) };

		if (compIt != end(m_CompEventCallbacks))
		{
			auto& observers{ compIt->second };

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

		auto normalIt{ m_EventCallbacks.find(event.GetEventName()) };

		if (normalIt != end(m_EventCallbacks))
		{
			auto& observers{ normalIt->second };

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

	void EventManager::RegisterObserver(std::weak_ptr<CompObserver> pObserver, fEventCallback fCallback, const std::string& event) noexcept
	{
		auto obs = pObserver.lock();

		const ObserverIdentifier identifier{ obs->GetEntityId(), obs->GetTypeIdx() };
		m_CompObservers[identifier] = pObserver;
		
		auto& ObserverVec{ m_CompEventCallbacks[event] };

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

	void EventManager::RegisterObserver(std::weak_ptr<Observer> pObserver, fEventCallback fCallback, const std::string& event) noexcept
	{
		auto& ObserverVec{ m_EventCallbacks[event] };
		ObserverVec.emplace_back(pObserver, fCallback);
	}

	void EventManager::SetObserverDirty(EntityId entiyId, std::type_index typeIdx) noexcept
	{
		auto it = m_CompObservers.find({entiyId, typeIdx});

		if (it == m_CompObservers.end()) [[unlikely]]
		{
			return;
		}

		auto pObs = it->second.lock();
		
		if (pObs)
		{
			pObs->SetDirty();
			return;
		}

		m_CompObservers.erase(it);
	}
}

