#include "Observer.h"
#include "EventManager.h"

namespace Pengin
{
	void Observer::RegisterForEvent(const std::string& eventName, fEventCallback fCallBack)
	{
		EventManager::GetInstance().RegisterObserver(eventName, { weak_from_this(), fCallBack });
	}
}
