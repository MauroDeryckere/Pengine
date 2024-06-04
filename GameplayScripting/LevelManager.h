#pragma once

#include <chrono>

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
			m_pObserver->RegisterForEvent(m_pObserver, "LoadPlayGame", [this](const Pengin::BaseEvent&) { LoadPlayGame(); });

			m_pObserver->RegisterForEvent(m_pObserver, "GameOver", [this](const Pengin::BaseEvent& event) { LoadGameOver(event); });
			m_pObserver->RegisterForEvent(m_pObserver, "Victory", [this](const Pengin::BaseEvent& event) { LoadVictory(event); });
		}

		~LevelManager() = default;

		std::shared_ptr<Pengin::Observer> m_pObserver;

		std::chrono::steady_clock::time_point m_StartTime;

		void LoadLevel();
		void LoadPlayGame();

		void LoadGameOver(const Pengin::BaseEvent& event);
		void LoadVictory(const Pengin::BaseEvent& event);
	};
}