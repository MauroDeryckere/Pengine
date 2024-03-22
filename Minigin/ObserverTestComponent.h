#ifndef OBSERVERTESTCOMP
#define OBSERVERTESTCOMP

#include <iostream> 
#include <functional> 
#include <memory> 
#include <unordered_map> 
#include <typeindex> 
#include <string> 

namespace Pengin
{
	template<typename CompT>
	concept ObserverConcept = requires(CompT component)
	{
		{ component.InitCallbacks() } -> std::same_as<void>;
	};

	struct Observer //We cant use typeidx, need to template it 
	{
		Observer(unsigned entityId, std::type_index typeidx) :
			m_EntityID{ entityId },
			m_TypeID { typeidx }
		{
			
		}

		//InitCallbacks();
		/*
		ECS::GetInstance().GetComponent(m_TypeIdx, entityID)
		comp.InitCallbacks();
		*/

		unsigned m_EntityID;
		std::type_index m_TypeID;
	};

	class BaseTemplatedObserver
	{
	public:
		virtual void InitCallbacks() = 0;
	private:

	};
	
	template<typename CompT>
	class TemplatedObserver : public BaseTemplatedObserver
	{
	public:
		void InitCallbacks()
		{
			//Ecs::GetComponet<CompT>
			//comp->InitCallbacks();
		}
	};


	struct EventManager
	{
		//ProcessEvents

		//BroadcastEvent

		//SetObserverDirty(entityId, compId)
		/*
		{
			
		}
		*/
		std::vector<unsigned> m_DirtyObsEntities;

		//Event, Observers registered for event
		std::unordered_map<std::string, std::weak_ptr<Observer>> m_Observers;
	};

	template<typename CompT>
	requires ObserverConcept<CompT>
	std::shared_ptr<BaseTemplatedObserver> InitObserver(unsigned entityId)
	{
		static_assert(ObserverConcept<CompT> && "Must provide a valid function in the component class");
		return std::make_shared<Observer>(typeid(CompT));
	}

	struct ObserverTestComp
	{
		ObserverTestComp()
		{
			m_Observer = InitObserver<ObserverTestComp>(1);
			
			//m_Observer->RegisterForEvent(EventName, CALLBACK)
		}
		
		void InitCallbacks()
		{

		}

		std::shared_ptr<BaseTemplatedObserver> m_Observer;
	};


	struct IntheECSImpl
	{
		//EventManager::SetObserverDirty(entityID)
			//If obserevr for given entity -> Setdirty()

		//OR
			//Register somehow in the ES first and only setdirty for the registered.
	};
}

#endif
