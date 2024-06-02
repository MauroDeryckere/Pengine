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
		void LoadLevel1();

		void PlayGame();

		GameManager(const GameManager&) = delete;
		GameManager(GameManager&&) = delete;
		GameManager& operator=(const GameManager&) = delete;
		GameManager& operator=(const GameManager&&) = delete;

	private:
		friend class Pengin::Singleton<GameManager>;

		GameManager():
			m_pObserver{ Pengin::EventManager::GetInstance().CreateObserver() }

		{ }

		~GameManager() = default;

		std::shared_ptr<Pengin::Observer> m_pObserver;
		bool playing = false;

		void RegisterKeyboardInputLevel(const Pengin::InputData& inpData);
		void RegisterControllerInputLevel(const Pengin::InputData& inpData);

		void RegisterKeyboardInputUI(const Pengin::InputData& inpData);
		void RegisterControllerInputUI(const Pengin::InputData& inpData);
	};
}

#endif
