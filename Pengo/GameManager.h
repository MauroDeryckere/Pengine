#ifndef PENGO_GAMESYSTEM
#define PENGO_GAMESYSTEM

#include "Singleton.h"

#include "SceneManager.h"
#include "EventManager.h"

namespace Pengo
{
	class GameManager final : public Pengin::Singleton<GameManager>
	{
	public:
		void LoadUI();

		void PlayGame();

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
			m_pObserver->RegisterForEvent(m_pObserver, "NextLevel", [this](const Pengin::BaseEvent&) { LoadNextLevel(); });
		}

		~GameManager() = default;

		std::shared_ptr<Pengin::Observer> m_pObserver;
		uint8_t m_CurrLevel{ 0 };

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
