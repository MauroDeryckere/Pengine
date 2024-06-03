#ifndef EVENTMANAGER
#define EVENTMANAGER

#include "Singleton.h"

#include "Event.h"
#include "Observer.h"

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <typeindex>
#include <queue>

namespace Pengin
{
	class EventManager final : public Singleton<EventManager>
	{
	public:
		void ProcessEventQueue() noexcept;

		void BroadcoastEvent(std::unique_ptr<BaseEvent> event) noexcept;
		void BroadcastBlockingEvent(std::unique_ptr<BaseEvent> event) noexcept;

		[[nodiscard]] std::shared_ptr<Observer> CreateObserver() const noexcept
		{
			return std::make_shared<Observer>();
		}

		void EraseEvent(const std::string& eventName) noexcept;

		size_t Size() const noexcept { return m_EventQueue.size(); }

		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(const EventManager&&) = delete;

	private:
		using fEventCallback = std::function<void(const BaseEvent&)>;

		friend class Pengin::Singleton<EventManager>;
		EventManager() = default;
		~EventManager() = default;

		friend class Observer;
		void RegisterObserver(std::weak_ptr<Observer> pObserver, fEventCallback fCallback, const std::string& eventName) noexcept;
		void ProcessEvent(BaseEvent* event) noexcept;

		using ObsCallbacks = std::vector<std::pair<std::weak_ptr<Observer>, fEventCallback>>;
		std::unordered_map<std::string, ObsCallbacks> m_EventCallbacks;

		std::queue<std::unique_ptr<BaseEvent>> m_EventQueue;
	};
}
#endif