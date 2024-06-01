#ifndef PENGO_BLOCKSYSTEM
#define PENGO_BLOCKSYSTEM

#include "BaseSystem.h"

#include "EventManager.h"
#include "PengoBlockBreakEvent.h"
#include "CollisionEvent.h"


namespace Pengin
{
	class ECS;
}
namespace Pengo
{
	class BlockSystem final : public Pengin::BaseSystem
	{
	public:
		BlockSystem(Pengin::ECS& ecs) :
			Pengin::BaseSystem{ },
			m_ECS{ ecs },
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }

		{ 
			m_pObserver->RegisterForEvent(m_pObserver, PengoBlockBreakEvent::PENGO_BLOCKBR_EVENT_NAME, [this](const Pengin::BaseEvent& ev) { OnBlockBreakEvent(ev); });
			m_pObserver->RegisterForEvent(m_pObserver, Pengin::CollisionEvent::COLLISION_EVENT_NAME, [this](const Pengin::BaseEvent& ev) { OnCollision(ev); });
		}

		~BlockSystem() = default;

		void Update();

		BlockSystem(const BlockSystem&) = delete;
		BlockSystem(BlockSystem&&) = delete;
		BlockSystem& operator=(const BlockSystem&) = delete;
		BlockSystem& operator=(const BlockSystem&&) = delete;

		enum class BlockAnimations : uint8_t
		{
			Still = 0,
			Breaking = 1
		};

	private:
		Pengin::ECS& m_ECS;

		std::shared_ptr<Pengin::Observer> m_pObserver;

		void OnBlockBreakEvent(const Pengin::BaseEvent& event);
		void OnCollision(const Pengin::BaseEvent& event);
	};
}

#endif
