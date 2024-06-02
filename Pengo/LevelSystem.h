#ifndef PENGO_LEVELSYSTEM
#define PENGO_LEVELSYSTEM

#include "BaseSystem.h"

#include "EventManager.h"

namespace Pengin
{
	class ECS;
}
namespace Pengo
{
	class LevelSystem final : public Pengin::BaseSystem
	{
	public:
		LevelSystem(Pengin::ECS& ecs) :
			Pengin::BaseSystem{ },
			m_ECS{ ecs },
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }

		{ }

		~LevelSystem() = default;

		void Update();

		LevelSystem(const LevelSystem&) = delete;
		LevelSystem(LevelSystem&&) = delete;
		LevelSystem& operator=(const LevelSystem&) = delete;
		LevelSystem& operator=(const LevelSystem&&) = delete;

	private:
		Pengin::ECS& m_ECS;

		std::shared_ptr<Pengin::Observer> m_pObserver;
		float m_LevelTimer{ 0.f };
	};
}

#endif
