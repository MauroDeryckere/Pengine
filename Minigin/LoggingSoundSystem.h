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

		const ChannelId PlaySound(const SoundData& soundData) noexcept
		{
			std::cout << "Playing sound: \n";
			soundData.PrintSoundInfo();

			return m_pRealSoundSystem->PlaySound(soundData);
		}

		void SetChannel3DPosition(const ChannelId& id, const glm::vec3& position) noexcept
		{
			m_pRealSoundSystem->SetChannel3DPosition(id, position);
		}
		void SetChannelVolume(const ChannelId& id, float volume) noexcept
		{
			m_pRealSoundSystem->SetChannelVolume(id, volume);
		}

		void MuteAll() noexcept
		{
			m_pRealSoundSystem->MuteAll();
		}
		void UnmuteAll() noexcept
		{
			m_pRealSoundSystem->UnmuteAll();
		}
		[[nodiscard]] bool IsMuted() const noexcept
		{
			return m_pRealSoundSystem->IsMuted();
		}

		void SetVFXVolume(const float vol) noexcept { m_pRealSoundSystem->SetVFXVolume(vol); }
		void SetMusicVolume(const float vol) noexcept { m_pRealSoundSystem->SetMusicVolume(vol); }

	private:
		std::unique_ptr<SoundSystem> m_pRealSoundSystem;
	};
}

#endif