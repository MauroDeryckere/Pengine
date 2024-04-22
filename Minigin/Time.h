#ifndef TIME
#define TIME

#include "Singleton.h"

#include <chrono>

namespace Pengin
{
	class GameTime final : public dae::Singleton<GameTime>
	{
	public:
		[[nodiscard]] inline float GetElapsedSec() const noexcept { return m_ElapsedSec; }
		[[nodiscard]] inline float GetFixedTimeStep() const { return (m_MsFixedTimeStep / 1000.f); }

		[[nodiscard]] inline bool IsLag() const noexcept { return m_MsLag >= m_MsFixedTimeStep;  }
		inline void ProcessLag() noexcept { m_MsLag -= m_MsFixedTimeStep;}

		inline void Update() noexcept
		{
			using std::chrono::high_resolution_clock;
			using std::chrono::duration;

			const auto currentTime{ high_resolution_clock::now() };
			m_ElapsedSec = duration<float>(currentTime - m_LastTime).count();

			m_LastTime = currentTime;
			m_MsLag += m_ElapsedSec * 1000.f;
		}

		[[nodiscard]] inline auto GetSleepTime() const noexcept
		{
			const auto sleepTime{ m_LastTime + std::chrono::milliseconds(static_cast<long>(m_MsPerFrame)) - std::chrono::high_resolution_clock::now() };
			return sleepTime;
		}

		GameTime(const GameTime&) = delete;
		GameTime(GameTime&&) = delete;
		GameTime& operator=(const GameTime&) = delete;
		GameTime& operator=(const GameTime&&) = delete;

	private:
		friend class dae::Singleton<GameTime>;
		GameTime() = default;
		~GameTime() = default;

		std::chrono::steady_clock::time_point m_LastTime{ std::chrono::high_resolution_clock::now() };

		const float m_MsPerFrame{ 16.7f };
		const float m_MsFixedTimeStep{ 20.f };

		float m_ElapsedSec{ 0.f };
		float m_MsLag{ 0.f };
	};
}

#endif