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
			std::cout
				<< "Loading sound: " << soundData.soundPath.string()
				<< " Vol: " << soundData.volumedB << "\n";
			m_pRealSoundSystem->LoadSound(soundData);
		}

		virtual void UnLoadSound(const std::filesystem::path& soundPath) noexcept override
		{
			std::cout << "UnLoading sound: " << soundPath.string() << "\n";
			m_pRealSoundSystem->UnLoadSound(soundPath);
		}

		//Returns the channelId in case specific changes to volume,... have to be made after loading	
		virtual const int32_t PlaySound(const SoundData& soundData) noexcept override
		{
			std::cout << "Playing sound: " << soundData.soundPath.string() << "\n";
			return m_pRealSoundSystem->PlaySound(soundData);
		}

		virtual void SetChannel3DPosition(int32_t channelId, const glm::vec3& position) noexcept override
		{
			m_pRealSoundSystem->SetChannel3DPosition(channelId, position);
		}

		virtual void SetChannelVolume(int32_t channelId, float volumeDb) noexcept override
		{
			m_pRealSoundSystem->SetChannelVolume(channelId, volumeDb);
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

		LoggingSoundSystem(const LoggingSoundSystem&) = delete;
		LoggingSoundSystem(LoggingSoundSystem&&) = delete;
		LoggingSoundSystem& operator=(const LoggingSoundSystem&) = delete;
		LoggingSoundSystem& operator=(const LoggingSoundSystem&&) = delete;

	private:
		std::unique_ptr<SoundSystem> m_pRealSoundSystem;
	};
}

#endif