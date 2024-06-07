#ifndef PENGO_GAMESYSTEM
#define PENGO_GAMESYSTEM

#include "Singleton.h"

#include "SceneManager.h"
#include "EventManager.h"
#include "ServiceLocator.h"

#include <chrono>

namespace Pengo
{
	class GameManager final : public Pengin::Singleton<GameManager>
	{
	public:
		void LoadUI();

		GameManager(const GameManager&) = delete;
		GameManager(GameManager&&) = delete;
		GameManager& operator=(const GameManager&) = delete;
		GameManager& operator=(const GameManager&&) = delete;

	private:
		friend class Pengin::Singleton<GameManager>;

		GameManager():
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }

		{ 
			m_pObserver->RegisterForEvent(m_pObserver, "PlayGame", [this](const Pengin::BaseEvent&) { LoadNextLevel(); });
			m_pObserver->RegisterForEvent(m_pObserver, "LevelWon", [this](const Pengin::BaseEvent&) { Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("LoadNextLevel")); });
			m_pObserver->RegisterForEvent(m_pObserver, "GameOver", [this](const Pengin::BaseEvent&) 
				{
					assert(m_BackGroundMusicId != Pengin::GameUUID::INVALID_UUID);
					Pengin::ServiceLocator::GetSoundSystem().StopPlaying(m_BackGroundMusicId);
					m_CurrLevel = 0;
					LoadUI(); 
				});

			m_pObserver->RegisterForEvent(m_pObserver, "GameWon", [this](const Pengin::BaseEvent&)
				{
					assert(m_BackGroundMusicId != Pengin::GameUUID::INVALID_UUID);
					Pengin::ServiceLocator::GetSoundSystem().StopPlaying(m_BackGroundMusicId);
					m_CurrLevel = 0;
					LoadUI();
				});


			m_pObserver->RegisterForEvent(m_pObserver, "LoadNextLevel", [this](const Pengin::BaseEvent&) { LoadNextLevel(); });
		}

		~GameManager() = default;

		std::shared_ptr<Pengin::Observer> m_pObserver;
		uint8_t m_CurrLevel{ 0 };

		std::chrono::steady_clock::time_point m_LevelStartTime;

		Pengin::ChannelId m_BackGroundMusicId;

		void LoadNextLevel();

		void LoadLevel1();
		void LoadLevel2();
		void LoadLevel3();

		void RegisterKeyboardInputLevel(const Pengin::InputData& inpData);
		void RegisterControllerInputLevel(const Pengin::InputData& inpData);

		void RegisterKeyboardInputUI(const Pengin::InputData& inpData);
		void RegisterControllerInputUI(const Pengin::InputData& inpData);
	};
}

#endif
