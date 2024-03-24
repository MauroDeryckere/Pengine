#ifndef EVENTMANAGER
#define EVENTMANAGER

#include "Singleton.h"

#include "Event.h"
#include "TypedObserver.h"

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
	concept ObserverConcept = requires(ComponentType component)
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
		requires ObserverConcept<ComponentType>
		std::shared_ptr<Observer> CreateObserver(EntityId entityId) const
		{
			static_assert(ObserverConcept<ComponentType>, "Must provide a valid function in the component class");

			return std::make_shared<TypedObserver<ComponentType>>(entityId);
		}

		void SetObserverDirty(EntityId entiyId, std::type_index typeIdx);

		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(const EventManager&&) = delete;

	private:
		using fEventCallback = std::function<void(const void*)>;

		friend class dae::Singleton<EventManager>;
		EventManager() = default;
		~EventManager() = default;

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

		std::unordered_map<ObserverIdentifier, std::weak_ptr<Observer>, ObserverIdentifierHash> m_Observers;
		std::unordered_map<std::string, std::vector<std::pair<std::weak_ptr<Observer>, fEventCallback>>> m_EventCallbacks;

		std::queue<Event> m_EventQueue;
	};
}
#endif