#ifndef PENGIN_SOUNDSYSTEM
#define PENGIN_SOUNDSYSTEM

#include "SoundData.h"

#include "glm/vec3.hpp"
#include <string>

#include <math.h>

namespace Pengin
{
	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default;

		virtual void Update() noexcept = 0;
		virtual void LoadSound(const SoundData& soundData) noexcept = 0;
		virtual void UnLoadSound(const std::filesystem::path& soundPath) noexcept = 0;

		virtual void PlaySound(const SoundData& soundData) noexcept  = 0;

		virtual void SetChannel3DPosition(const GameUUID& id, const glm::vec3& position) noexcept = 0;
		virtual void SetChannelVolume(const GameUUID& id, float volume) noexcept = 0;

		virtual void SetVFXVolume(const float vol) noexcept = 0;
		virtual void SetMusicVolume(const float vol) noexcept = 0;

		virtual void MuteAll() noexcept = 0;
		virtual void UnmuteAll() noexcept = 0;
		virtual [[nodiscard]] bool IsMuted() const noexcept = 0;

		//Conversion functions
		[[nodiscard]] inline static float DbToVolume(const float dB) noexcept
		{
			return powf(10.0f, 0.05f * dB);
		}
		[[nodiscard]] inline static float VolumeTodB(const float volume) noexcept
		{
			return 20.0f * log10f(volume);
		}

		SoundSystem(const SoundSystem&) = delete;
		SoundSystem(SoundSystem&&) = delete;
		SoundSystem& operator=(const SoundSystem&) = delete;
		SoundSystem& operator=(const SoundSystem&&) = delete;
	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		NullSoundSystem() = default;
		virtual ~NullSoundSystem() override = default;

		virtual void Update() noexcept {}
		virtual void LoadSound(const SoundData&) noexcept {}
		virtual void UnLoadSound(const std::filesystem::path&) noexcept {}

		virtual void PlaySound(const SoundData&) noexcept { }

		virtual void SetChannel3DPosition(const GameUUID&, const glm::vec3&) noexcept {}
		virtual void SetChannelVolume(const GameUUID&, float) noexcept {}

		virtual void SetVFXVolume(const float) noexcept {}
		virtual void SetMusicVolume(const float) noexcept {}

		virtual void MuteAll() noexcept {}
		virtual void UnmuteAll() noexcept {}
		virtual bool IsMuted() const noexcept { return false; }

		NullSoundSystem(const NullSoundSystem&) = delete;
		NullSoundSystem(NullSoundSystem&&) = delete;
		NullSoundSystem& operator=(const NullSoundSystem&) = delete;
		NullSoundSystem& operator=(const NullSoundSystem&&) = delete;
	};
}

#endif