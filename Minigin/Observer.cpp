#include "Observer.h"

#include "EventManager.h"

namespace Pengin
{
	void Observer::RegisterForEvent(const std::string& eventName, std::function<void()> fCallback)
	{
		EventManager::GetInstance().RegisterObserver(shared_from_this(), fCallback, eventName);
	}
}


