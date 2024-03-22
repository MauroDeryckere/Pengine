#include "EventManager_.h"
#include <algorithm>
#include <cassert>

namespace Pengin
{
	void EventManager_::ProcessEventQueue()
	{
		//For now set all dirty
		SetObserverDirty(); 

		while (!m_EventQueue.empty())
		{
			ProcessEvent(m_EventQueue.front()); //br here
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
		auto it{ m_Callbacks.find(event) };

		if (it != end(m_Callbacks))
		{
			auto& observers{ it->second };

			std::erase_if(observers, [](const auto& observerPair)
				{
					return observerPair.first.expired();
				});
			//TODO cleanup


			for (size_t i = 0; i < observers.size(); i++)
			{
				auto pObs{ observers[i].first.lock()};
				pObs->RegisterCallbacks(); //some breaking error in here - inf loop, obv need to erase first... dumbass
;			}

			for (auto& [observer, fCallback] : observers)
			{
				fCallback();
			}
		}
	
	}
	void EventManager_::RegisterObserver(std::shared_ptr<Observer_> pObserver, std::function<void()> fCallback, const std::string& event)
	{
		const ObserverIdentifier identifier{ pObserver->GetEntityId(), pObserver->GetTypeIdx() };

		m_Observers[identifier] = pObserver;
		auto& vec = m_Callbacks[event];

		bool found = false;

		for (int i = 0; i < vec.size(); ++i)
		{
			if (vec[i].first.lock() == pObserver)
			{
				vec[i] = { pObserver, fCallback };
				found = true;
			}
		}

		if (!found)
		{
			m_Callbacks[event].emplace_back(pObserver, fCallback);
		}
	}

	void EventManager_::SetObserverDirty()
	{
		for (auto& obsPair : m_Observers)
		{
			obsPair.second.lock()->SetDirty();
		}
	}
}

