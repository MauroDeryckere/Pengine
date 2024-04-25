#ifndef PENGIN_SOUNDSYSTEM
#define PENGIN_SOUNDSYSTEM

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

		virtual void Update() = 0;
		virtual void LoadSound(const std::string& soundName, bool is3D = true, bool isLooping = false, bool isStream = false) = 0;
		virtual void UnLoadSound(const std::string& soundName) = 0;

		//Returns the channelId in case specific changes to volume,... have to be made after loading	
		virtual const int PlaySounds(const std::string& soundName, const glm::vec3& position = { 0, 0, 0 }, float volumedB = 0.f) = 0;

		virtual void SetChannel3DPosition(int channelId, const glm::vec3& position) = 0;
		virtual void SetChannelVolume(int channelId, float volumeDb) = 0;

		//Conversion functions
		[[nodiscard]] static float DbToVolume(const float dB) noexcept
		{
			return powf(10.0f, 0.05f * dB);
		}
		[[nodiscard]] static float VolumeTodB(const float volume) noexcept
		{
			return 20.0f * log10f(volume);
		}

		SoundSystem(const SoundSystem&) = delete;
		SoundSystem(SoundSystem&&) = delete;
		SoundSystem& operator=(const SoundSystem&) = delete;
		SoundSystem& operator=(const SoundSystem&&) = delete;

	private:


	};
}

#endif