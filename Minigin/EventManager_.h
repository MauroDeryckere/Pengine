#pragma once

#include "ECS.h"
#include "Singleton.h"

#include "Observer_.h"
#include "TypedObserver.h"

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <typeindex>
#include <queue>

namespace Pengin
{
	template<typename ComponentType>
	concept ObserverConcept = requires(ComponentType component)
	{
		{ component.RegisterObservers() } -> std::same_as<void>;
	};

	class EventManager_ final : public dae::Singleton<EventManager_>
	{
	public:
		void ProcessEventQueue();

		void BroadcoastEvent(const std::string& event);
		void BroadcastBlockingEvent(const std::string& event);

		template<typename ComponentType>
		requires ObserverConcept<ComponentType>
		std::shared_ptr<Observer_> CreateObserver(EntityId entityId) const
		{
			static_assert(ObserverConcept<ComponentType>, "Must provide a valid function in the component class");

			return std::make_shared<TypedObserver<ComponentType>>(entityId);
		}

		EventManager_(const EventManager_&) = delete;
		EventManager_(EventManager_&&) = delete;
		EventManager_& operator=(const EventManager_&) = delete;
		EventManager_& operator=(const EventManager_&&) = delete;

	private:
		using fEventCallback = std::function<void()>;

		friend class dae::Singleton<EventManager_>;
		EventManager_() = default;
		~EventManager_() = default;

		friend class Observer_;
		void RegisterObserver(std::shared_ptr<Observer_> pObserver, fEventCallback fCallback, const std::string& event);

		void SetObserverDirty();
		
		void ProcessEvent(const std::string& event);

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

		std::unordered_map<ObserverIdentifier, std::weak_ptr<Observer_>, ObserverIdentifierHash> m_Observers;
		std::unordered_map<std::string, std::vector<std::pair<std::weak_ptr<Observer_>, fEventCallback>>> m_EventCallbacks;

		std::queue<std::string> m_EventQueue;
	};
}