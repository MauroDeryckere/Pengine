#ifndef PENGO_GAMESYSTEM
#define PENGO_GAMESYSTEM

#include "BaseSystem.h"

#include "EventManager.h"

namespace Pengin
{
	class ECS;
}
namespace Pengo
{
	class GameSystem final : public Pengin::BaseSystem
	{
	public:
		GameSystem(Pengin::ECS& ecs) :
			Pengin::BaseSystem{ },
			m_ECS{ ecs },
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }

		{

		}

		~GameSystem() = default;

		GameSystem(const GameSystem&) = delete;
		GameSystem(GameSystem&&) = delete;
		GameSystem& operator=(const GameSystem&) = delete;
		GameSystem& operator=(const GameSystem&&) = delete;

	private:
		Pengin::ECS& m_ECS;

		std::shared_ptr<Pengin::Observer> m_pObserver;
	};
}

#endif
