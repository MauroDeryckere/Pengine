#ifndef EVENTMANAGER
#define EVENTMANAGER

#include "Singleton.h"

#include "Event.h"
#include "Observer.h"
#include "TypedCompObserver.h"

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <typeindex>
#include <queue>

namespace Pengin
{
	using EntityId = unsigned;

	template<typename ComponentType>
	concept CompObserverConcept = requires(ComponentType component)
	{
		{ component.RegisterObservers() } -> std::same_as<void>;
	};

	class EventManager final : public dae::Singleton<EventManager>
	{
	public:
		void ProcessEventQueue();

		void BroadcoastEvent(const Event& event);
		void BroadcastBlockingEvent(const Event& event);

		template<typename ComponentType>
		requires CompObserverConcept<ComponentType>
		std::shared_ptr<CompObserver> CreateComponentObserver(EntityId entityId) const
		{
			static_assert(CompObserverConcept<ComponentType>, "Must provide a valid function in the component class");

			return std::make_shared<TypedCompObserver<ComponentType>>(entityId);
		}

		std::shared_ptr<Observer> CreateObserver() const
		{
			return std::make_shared<Observer>();
		}

		void SetObserverDirty(EntityId entiyId, std::type_index typeIdx);

		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(const EventManager&&) = delete;

		void RemoveEvent(const std::string& eventName) 
		{ 
			m_CompEventCallbacks.erase(eventName); 
			m_EventCallbacks.erase(eventName);
		}

	private:
		using fEventCallback = std::function<void(const void*)>;

		friend class dae::Singleton<EventManager>;
		EventManager() = default;
		~EventManager() = default;

		friend class CompObserver;
		void RegisterObserver(std::weak_ptr<CompObserver> pObserver, fEventCallback fCallback, const std::string& eventName);

		friend class Observer;
		void RegisterObserver(std::weak_ptr<Observer> pObserver, fEventCallback fCallback, const std::string& eventName);

		void ProcessEvent(const Event& event);

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

		std::unordered_map<ObserverIdentifier, std::weak_ptr<CompObserver>, ObserverIdentifierHash> m_CompObservers;

		using CompObsCallbacks = std::vector<std::pair<std::weak_ptr<CompObserver>, fEventCallback>>;
		using ObsCallbacks = std::vector<std::pair<std::weak_ptr<Observer>, fEventCallback>>;
		std::unordered_map<std::string, CompObsCallbacks> m_CompEventCallbacks;
		std::unordered_map<std::string, ObsCallbacks> m_EventCallbacks;

		std::queue<Event> m_EventQueue;
	};
}
#endif