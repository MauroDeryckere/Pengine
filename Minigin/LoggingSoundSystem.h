#ifndef PENGIN_LOGGINGSOUNDSYSTEM
#define PENGIN_LOGGINGSOUNDSYSTEM

#include "SoundSystem.h"
#include <iostream>

namespace Pengin
{
	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem) :
			m_pRealSoundSystem{ std::move(pSoundSystem) }
		{}

		~LoggingSoundSystem() = default;

		void Update() noexcept
		{
			m_pRealSoundSystem->Update();
		};

		void LoadSound(const SoundData& soundData) noexcept
		{
			std::cout<< "Loading sound: \n";
			soundData.PrintSoundInfo();

			m_pRealSoundSystem->LoadSound(soundData);
		}

		void UnLoadSound(const std::filesystem::path& soundPath) noexcept
		{
			std::cout << "UnLoading sound: " << soundPath.string() << "\n";
			m_pRealSoundSystem->UnLoadSound(soundPath);
		}

		void Clear() noexcept
		{
			std::cout << "Clearing sound sys\n";
			m_pRealSoundSystem->Clear();
		}

		const ChannelId PlaySound(const SoundData& soundData) noexcept
		{
			std::cout << "Playing sound: \n";
			soundData.PrintSoundInfo();

			return m_pRealSoundSystem->PlaySound(soundData);
		}
		bool StopPlaying(const ChannelId& channel) noexcept
		{
			const bool stopped = m_pRealSoundSystem->StopPlaying(channel);

			if (stopped)
			{
				std::cout << "Stop playing id: "<< channel << "\n";
			}

			return stopped;
		}

		void SetChannel3DPosition(const ChannelId& id, const glm::vec3& position) noexcept
		{
			m_pRealSoundSystem->SetChannel3DPosition(id, position);
		}
		void SetChannelVolume(const ChannelId& id, float volume) noexcept
		{
			m_pRealSoundSystem->SetChannelVolume(id, volume);
		}

		void Mute() noexcept
		{
			m_pRealSoundSystem->Mute();
		}
		void Unmute() noexcept
		{
			m_pRealSoundSystem->Unmute();
		}
		[[nodiscard]] bool IsMuted() const noexcept
		{
			return m_pRealSoundSystem->IsMuted();
		}

		bool Mute(const ChannelId& channel) noexcept
		{
			return m_pRealSoundSystem->Mute(channel);
		}
		bool Unmute(const ChannelId& channel) noexcept
		{
			return m_pRealSoundSystem->Unmute(channel);
		}
		bool IsMuted(const ChannelId& channel) const noexcept
		{
			return m_pRealSoundSystem->IsMuted(channel);
		}

		void SetVFXVolume(const float vol) noexcept { m_pRealSoundSystem->SetVFXVolume(vol); }
		void SetMusicVolume(const float vol) noexcept { m_pRealSoundSystem->SetMusicVolume(vol); }

	private:
		std::unique_ptr<SoundSystem> m_pRealSoundSystem;
	};
}

#endif