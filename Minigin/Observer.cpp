#include "Observer.h"
#include "EventManager.h"

#include "Event.h"

namespace Pengin
{
	void Observer::RegisterForEvent(std::weak_ptr<Observer> pObs, const std::string& eventName, std::function<void(const BaseEvent& event)> fCallback)
	{
		EventManager::GetInstance().RegisterObserver(pObs, fCallback, eventName);
	}
}


