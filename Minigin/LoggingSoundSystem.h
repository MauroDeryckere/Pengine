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

		virtual ~LoggingSoundSystem() = default;

		virtual void Update() noexcept override
		{
			m_pRealSoundSystem->Update();
		};

		virtual void LoadSound(const SoundData& soundData) noexcept override
		{
			std::cout<< "Loading sound: \n";
			soundData.PrintSoundInfo();

			m_pRealSoundSystem->LoadSound(soundData);
		}

		virtual void UnLoadSound(const std::filesystem::path& soundPath) noexcept override
		{
			std::cout << "UnLoading sound: " << soundPath.string() << "\n";
			m_pRealSoundSystem->UnLoadSound(soundPath);
		}

		virtual void PlaySound(const SoundData& soundData) noexcept override
		{
			std::cout << "Playing sound: \n";
			soundData.PrintSoundInfo();

			m_pRealSoundSystem->PlaySound(soundData);
		}

		virtual void SetChannel3DPosition(const GameUUID& id, const glm::vec3& position) noexcept override
		{
			m_pRealSoundSystem->SetChannel3DPosition(id, position);
		}

		virtual void SetChannelVolume(const GameUUID& id, float volumeDb) noexcept override
		{
			m_pRealSoundSystem->SetChannelVolume(id, volumeDb);
		}

		virtual void MuteAll() noexcept override
		{
			m_pRealSoundSystem->MuteAll();
		}
		virtual void UnmuteAll() noexcept override
		{
			m_pRealSoundSystem->UnmuteAll();
		}
		virtual [[nodiscard]] bool IsMuted() const noexcept override
		{
			return m_pRealSoundSystem->IsMuted();
		}

		virtual void SetVFXVolume(const float vol) noexcept override { m_pRealSoundSystem->SetVFXVolume(vol); }
		virtual void SetMusicVolume(const float vol) noexcept override { m_pRealSoundSystem->SetMusicVolume(vol); }

		LoggingSoundSystem(const LoggingSoundSystem&) = delete;
		LoggingSoundSystem(LoggingSoundSystem&&) = delete;
		LoggingSoundSystem& operator=(const LoggingSoundSystem&) = delete;
		LoggingSoundSystem& operator=(const LoggingSoundSystem&&) = delete;

	private:
		std::unique_ptr<SoundSystem> m_pRealSoundSystem;
	};
}

#endif