#ifndef TIME
#define TIME

#include "Singleton.h"

#include <chrono>

namespace Pengin
{
	class Time final : public dae::Singleton<Time>
	{
	public:
		[[nodiscard]] float GetElapsedSec() const { return m_ElapsedSec; }
		[[nodiscard]] float GetFixedTimeStep() const { return (m_FixedTimeStep / 1000.f); }

		[[nodiscard]] bool IsLag() const { return m_Lag >= m_FixedTimeStep;  }

		void ProcessLag() { m_Lag -= m_FixedTimeStep;}

		void Update();

		[[nodiscard]] auto GetSleepTime() const
		{
			const auto sleepTime{ m_LastTime + std::chrono::milliseconds(static_cast<long>(m_MsPerFrame)) - std::chrono::high_resolution_clock::now() };
			return sleepTime;
		}

		Time(const Time&) = delete;
		Time(Time&&) = delete;
		Time& operator=(const Time&) = delete;
		Time& operator=(const Time&&) = delete;

	private:
		friend class dae::Singleton<Time>;
		Time() = default;
		~Time() = default;

		std::chrono::steady_clock::time_point m_LastTime{ std::chrono::high_resolution_clock::now() };

		const float m_MsPerFrame{ 16.7f };
		const float m_FixedTimeStep{ 20.f };

		float m_ElapsedSec{ 0.f };
		float m_Lag{ 0.f };
	};
}

#endif