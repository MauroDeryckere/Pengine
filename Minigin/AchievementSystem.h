#ifndef ACHIEVEMENTSYSTEM
#define ACHIEVEMENTSYSTEM

#include "Defines.h"

#include "Singleton.h"
#include "EventManager.h"

#ifdef USE_STEAMWORKS
	#pragma warning (push)
	#pragma warning (disable: 4996)
		#include "steam_api.h"
	#pragma warning (pop)
#endif

namespace Pengin
{
	class AchievementSystem final
	{
	public:
		AchievementSystem() :
			m_Observer{ EventManager::GetInstance().CreateObserver() }
		{
			RegisterObservers();
		}
 
		~AchievementSystem() = default;

		AchievementSystem(const AchievementSystem&) = delete;
		AchievementSystem(AchievementSystem&&) = delete;
		AchievementSystem& operator=(const AchievementSystem&) = delete;
		AchievementSystem& operator=(const AchievementSystem&&) = delete;

	private:
		void RegisterObservers()
		{
			//TODO
			auto callBack = [this](const BaseEvent& event) { ScoreAchievementCallback(event); };
			m_Observer->RegisterForEvent(m_Observer,"OnScoreIncreaseEvent", callBack);
		}

		void ScoreAchievementCallback(const BaseEvent& event)
		{
			//TODO
			event;
			//const unsigned score{ (*static_cast<const unsigned*>(pScore)) };

			//constexpr unsigned reqScore{ 500 };

			//if (score >= reqScore  && !m_Unlocked)
			//{
			//#ifdef USE_STEAMWORKS
			//	SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
			//	SteamUserStats()->StoreStats();
			//	return;
			//#else 
			//	std::cout << "winner achievement unlocked\n";
			//	m_Unlocked = true;
			//#endif				
			//}
		}

		std::shared_ptr<Observer> m_Observer;
		bool m_Unlocked = false;
	};
}

#endif