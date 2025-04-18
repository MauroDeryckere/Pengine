#ifndef TIME
#define TIME

#include "Singleton.h"
#include <chrono>

namespace Pengin
{
	class GameTime final : public Singleton<GameTime>
	{
	public:
		[[nodiscard]] inline float ElapsedSec() const noexcept { return m_ElapsedSec; }
		[[nodiscard]] inline float FixedTimeStep() const { return (m_MsFixedTimeStep / 1000.f); }

		[[nodiscard]] inline bool IsLag() const noexcept { return m_MsLag >= m_MsFixedTimeStep;  }
		inline void ProcessLag() noexcept { m_MsLag -= m_MsFixedTimeStep;}

		inline void Update() noexcept
		{
			using namespace std::chrono;

			const auto currentTime = high_resolution_clock::now();
			m_ElapsedSec = duration<float>(currentTime - m_LastTime).count();

			m_MsLag += m_ElapsedSec * 1000.f;
			m_LastTime = std::chrono::high_resolution_clock::now();
		}

		[[nodiscard]] inline auto SleepTime() const noexcept
		{
			return m_LastTime + std::chrono::milliseconds(static_cast<long>(m_MsPerFrame)) - std::chrono::high_resolution_clock::now();
		}

		GameTime(const GameTime&) = delete;
		GameTime(GameTime&&) = delete;
		GameTime& operator=(const GameTime&) = delete;
		GameTime& operator=(const GameTime&&) = delete;

	private:
		friend class Pengin::Singleton<GameTime>;
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