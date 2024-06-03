#pragma once

#include "Singleton.h"

#include "EventManager.h"

namespace GS
{
	class LevelManager final : public Pengin::Singleton<LevelManager>
	{
	public:
		LevelManager(const LevelManager&) = delete;
		LevelManager(LevelManager&&) = delete;
		LevelManager& operator=(const LevelManager&) = delete;
		LevelManager& operator=(const LevelManager&&) = delete;
	
	private:
		friend class Pengin::Singleton<LevelManager>;
		LevelManager():
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }
		{
			m_pObserver->RegisterForEvent(m_pObserver, "LoadLevel", [this](const Pengin::BaseEvent&) { LoadLevel(); });
			m_pObserver->RegisterForEvent(m_pObserver, "LoadRestart", [this](const Pengin::BaseEvent&) {LoadRestart(); });
		}

		~LevelManager() = default;

		std::shared_ptr<Pengin::Observer> m_pObserver;

		void LoadLevel();
		void LoadRestart();
	};
}