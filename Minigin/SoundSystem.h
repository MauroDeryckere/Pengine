#ifndef PENGIN_SOUNDSYSTEM
#define PENGIN_SOUNDSYSTEM

#include "SoundData.h"

#include "glm/vec3.hpp"
#include <string>

#include <math.h>

namespace Pengin
{
	using ChannelId = GameUUID;

	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default;

		virtual void Update() noexcept = 0;
		virtual void LoadSound(SoundData& soundData) noexcept = 0;
		virtual void UnLoadSound(const std::filesystem::path& soundPath) noexcept = 0;

		//It is important to load the sound first if you want a valid channel idx
		virtual const ChannelId PlaySound(SoundData& soundData) noexcept  = 0;

		virtual void SetChannel3DPosition(const ChannelId& id, const glm::vec3& position) noexcept = 0;
		virtual void SetChannelVolume(const ChannelId& id, float volume) noexcept = 0;

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
	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		NullSoundSystem() = default;
		~NullSoundSystem() = default;

		void Update() noexcept {}
		void LoadSound(SoundData&) noexcept {}
		void UnLoadSound(const std::filesystem::path&) noexcept {}

		const ChannelId PlaySound(SoundData&) noexcept { return GameUUID::INVALID_UUID; }

		void SetChannel3DPosition(const ChannelId&, const glm::vec3&) noexcept {}
		void SetChannelVolume(const ChannelId&, float) noexcept {}

		void SetVFXVolume(const float) noexcept {}
		void SetMusicVolume(const float) noexcept {}

		void MuteAll() noexcept {}
		void UnmuteAll() noexcept {}
		bool IsMuted() const noexcept { return false; }
	};
}

#endif