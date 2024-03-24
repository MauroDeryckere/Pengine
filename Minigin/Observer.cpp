#include "CompObserver.h"

#include "EventManager.h"

namespace Pengin
{
	void CompObserver::RegisterForEvent(std::weak_ptr<CompObserver> pObs, const std::string& eventName, std::function<void(const void* eventData)> fCallback)
	{
		EventManager::GetInstance().RegisterObserver(pObs, fCallback, eventName);
	}
}


