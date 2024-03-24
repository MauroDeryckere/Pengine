#ifndef ACHIEVEMENTSYSTEM
#define ACHIEVEMENTSYSTEM

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
		AchievementSystem(EntityId id) :
			m_Observer{ EventManager::GetInstance().CreateComponentObserver<AchievementSystem>(id) }
		{
			RegisterObservers();
		}

		~AchievementSystem() = default;

		void RegisterObservers()
		{
			auto callBack = [this](const void* eventData){ ScoreAchievementCallback(eventData); };
			m_Observer->RegisterForEvent(m_Observer, "ScoreAchievement", callBack);
		}

		//AchievementSystem(const AchievementSystem&) = delete;
		//AchievementSystem(AchievementSystem&&) = delete;
		//AchievementSystem& operator=(const AchievementSystem&) = delete;
		//AchievementSystem& operator=(const AchievementSystem&&) = delete;

	private:
		void ScoreAchievementCallback(const void* pScore)
		{
			const unsigned score{ (*static_cast<const unsigned*>(pScore)) };

			constexpr unsigned reqScore{ 500 };

			if (score >= reqScore)
			{
			#ifdef USE_STEAMWORKS
				SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
				SteamUserStats()->StoreStats();
			#else	
				if (!m_Unlocked)
				{
					std::cout << "winner achievement unlocked\n";
					m_Unlocked = true;
				}
			#endif
			}
		}

		std::shared_ptr<CompObserver> m_Observer;
		bool m_Unlocked = false;
	};
}

#endif