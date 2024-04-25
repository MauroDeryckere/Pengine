#ifndef OBSERVER
#define OBSERVER

#include <memory>
#include <string>
#include <functional>

namespace Pengin
{
	class BaseEvent;

	class Observer final
	{
	public:
		Observer() = default;
		~Observer() = default;

		Observer(const Observer&) = delete;
		Observer(Observer&&) = delete;
		Observer& operator=(const Observer&) = delete;
		Observer& operator=(const Observer&&) = delete;

		void RegisterForEvent(std::weak_ptr<Observer> pObs, const std::string& eventName, std::function<void(const BaseEvent& event)> fCallback);
	};
}

#endif