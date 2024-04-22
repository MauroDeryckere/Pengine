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
	using EntityId = unsigned;

	class EventManager final : public dae::Singleton<EventManager>
	{
	public:
		void ProcessEventQueue() noexcept;

		void BroadcoastEvent(const Event& event) noexcept;
		void BroadcastBlockingEvent(const Event& event) noexcept;

		[[nodiscard]] std::shared_ptr<Observer> CreateObserver() const noexcept
		{
			return std::make_shared<Observer>();
		}

		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(const EventManager&&) = delete;

		void RemoveEvent(const std::string& eventName) 
		{ 
			m_EventCallbacks.erase(eventName);
		}

	private:
		using fEventCallback = std::function<void(const void*)>;

		friend class dae::Singleton<EventManager>;
		EventManager() = default;
		~EventManager() = default;

		friend class Observer;
		void RegisterObserver(std::weak_ptr<Observer> pObserver, fEventCallback fCallback, const std::string& eventName) noexcept;
		void ProcessEvent(const Event& event) noexcept;

		using ObserverIdentifier = std::pair<EntityId, std::type_index>;
		struct ObserverIdentifierHash 
		{
			size_t operator()(const ObserverIdentifier& identifier) const 
			{
				const size_t hash1 = std::hash<unsigned>{}(identifier.first);
				const size_t hash2 = identifier.second.hash_code();

				return hash1 ^ hash2;
			}
		};

		using ObsCallbacks = std::vector<std::pair<std::weak_ptr<Observer>, fEventCallback>>;
		std::unordered_map<std::string, ObsCallbacks> m_EventCallbacks;

		std::queue<Event> m_EventQueue;
	};
}
#endif