#ifndef EVENTMANAGER
#define EVENTMANAGER

#include "Singleton.h"

#include "EventQueue.h" //can we move this?
#include "Observer.h"
#include "Event.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Pengin
{
	class EventManager final : public dae::Singleton<EventManager>
	{
	public:
		void ProcessEvents();

		void BroadcastEvent(const Event& event);

		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(const EventManager&&) = delete;

	private:
		friend class dae::Singleton<EventManager>;
		
		friend class Observer;
		void RegisterObserver(const std::string& eventName, std::pair<std::shared_ptr<Observer>, fEventCallback> observer);

		friend class EventQueue;
		void ProcessEvent(const Event& event);

		EventManager();
		~EventManager() = default;

		std::unique_ptr<EventQueue> m_EventQueue;

		using EventListener = std::pair<std::weak_ptr<Observer>, fEventCallback>;
		std::unordered_map<std::string, std::vector<EventListener>> m_Observers; //since using strings here and likely larger map in big games, consider a map inst of uno map (monitor performance 1st) TODO
	};
}

#endif
