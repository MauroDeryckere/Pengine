#include "Observer_.h"

#include "EventManager_.h"

namespace Pengin
{
	void Observer_::RegisterForEvent(const std::string& eventName, std::function<void()> fCallback)
	{
		EventManager_::GetInstance().RegisterObserver(shared_from_this(), fCallback, eventName);
	}
}


