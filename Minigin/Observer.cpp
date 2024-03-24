#include "Observer.h"

#include "EventManager.h"

namespace Pengin
{
	void Observer::RegisterForEvent(std::weak_ptr<Observer> pObs, const std::string& eventName, std::function<void(const void* eventData)> fCallback)
	{
		EventManager::GetInstance().RegisterObserver(pObs, fCallback, eventName);
	}
}


