#ifndef PENGO_GAMESYSTEM
#define PENGO_GAMESYSTEM

#include "Singleton.h"

#include "SceneManager.h"
#include "EventManager.h"
#include "ServiceLocator.h"
#include "Scene.h"
#include "ScoreCollectEvent.h"
#include "LevelWonEvent.h"
#include "Entity.h"

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
			m_pObserver->RegisterForEvent(m_pObserver, "LevelWon", [this](const Pengin::BaseEvent& event) 
				{ 
					const auto& levelWonEv{ static_cast<const LevelWonEvent&>(event) };

					const auto winTime{ std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_LevelStartTime).count() };
					
					unsigned bonusScore{ 0 };

					if (winTime < 20)
					{
						bonusScore = 5000;
					}					
					else if (winTime < 30)
					{
						bonusScore = 2000;
					}
					else if (winTime < 40)
					{
						bonusScore = 1000;
					}
					else if (winTime < 50)
					{
						bonusScore = 500;
					}
					else if (winTime < 60)
					{
						bonusScore = 10;
					}

					auto pActiveScene = Pengin::SceneManager::GetInstance().GetActiveScene();
					Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<ScoreCollectEvent>(bonusScore, pActiveScene->GetPlayer(levelWonEv.GetUserIdx()).GetEntityId()));
					Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("LoadNextLevel")); 
				});

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
		void LoadLevel(uint8_t level);

		void RegisterKeyboardInputUI(const Pengin::InputData& inpData);
		void RegisterControllerInputUI(const Pengin::InputData& inpData);

		void RegisterKeyboardInputLevel(const Pengin::InputData& inpData);
		void RegisterControllerInputLevel(const Pengin::InputData& inpData);
	};
}

#endif
