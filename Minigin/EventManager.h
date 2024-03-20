#ifndef EVENTMANAGER
#define EVENTMANAGER

#include "Singleton.h"
#include "EventQueue.h"

#include "Observer.h"
#include "Event.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Pengin
{
	class Observer;

	class EventManager final : public dae::Singleton<EventManager>
	{
	public:
		//ProcessEvents;
		void ProcessEvents();
		
		void BroadcastEvent(const std::string& eventName, const void* eventData);

		void RegisterObserver(const std::string& eventName, std::pair<std::weak_ptr<Observer>, std::function<void(const void*)>> observer);
		
		void ProcessEvent(const std::string& eventName, const void* eventData); //Event data could be inside Event object in future, simplifies this area slightly, just Event.naame orGetName and data 
		
		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(const EventManager&&) = delete;

	private:
		friend class dae::Singleton<EventManager>;

		EventManager();
		~EventManager() = default;


		std::unique_ptr<EventQueue> m_EventQueue;

		std::unordered_map<std::string, std::vector<std::pair<std::weak_ptr<Observer>, std::function<void(const void*)>>>> m_Observers; //since using strings here and likely larger map in big games, consider a map inst of uno map (monitor performance 1st) TODO
	};
}

#endif
