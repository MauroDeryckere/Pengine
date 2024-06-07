#ifndef PENGIN_SOUNDSYSTEM
#define PENGIN_SOUNDSYSTEM

#include "SoundData.h"
#include "DebugOutput.h"

#include "glm/vec3.hpp"
#include <string>

#include <math.h>
#include <algorithm>

namespace Pengin
{
	using ChannelId = GameUUID;

	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default;

		virtual void Update() noexcept = 0;

		virtual void LoadSound(const SoundData& soundData) noexcept = 0;
		virtual void UnLoadSound(const std::filesystem::path& soundPath) noexcept = 0;

		virtual void Clear() noexcept = 0;

		//It is important to load the sound first if you want a valid channel idx
		virtual const ChannelId PlaySound(const SoundData& soundData) noexcept  = 0;
		virtual bool StopPlaying(const ChannelId& channel) noexcept = 0;

		virtual void SetChannel3DPosition(const ChannelId& id, const glm::vec3& position) noexcept = 0;
		virtual void SetChannelVolume(const ChannelId& id, float volume) noexcept = 0;

		virtual void SetVFXVolume(const float vol) noexcept = 0;
		virtual void SetMusicVolume(const float vol) noexcept = 0;

		virtual void Mute() noexcept = 0;
		virtual void Unmute() noexcept = 0;
		virtual [[nodiscard]] bool IsMuted() const noexcept = 0;

		virtual bool Mute(const ChannelId& channel) noexcept = 0;
		virtual bool Unmute(const ChannelId& channel) noexcept = 0;
		virtual bool IsMuted(const ChannelId& channel) const noexcept = 0;

		//Util funtion to allow loading all files from a folder easily (will load all with default soundData parameters)
		std::vector<SoundData> LoadSoundsFromFolder(const std::string& folderPath) noexcept
		{
			std::vector<SoundData> loadedSounds{ };

			if (!std::filesystem::is_directory(folderPath) || !std::filesystem::exists(folderPath))
			{
				std::cerr << "Error: Folder does not exist - " << folderPath << std::endl;
				return (loadedSounds);
			}

			try 
			{
				for (const auto& entry : std::filesystem::directory_iterator(folderPath)) 
				{
					if (entry.is_regular_file()) 
					{
						const auto& filePath = entry.path();
						auto pathStr = filePath.string();
						std::replace(pathStr.begin(), pathStr.end(), '\\', '/'); //naming convention for engine is /

						DEBUG_OUT("loaded sound from folder: " << pathStr);
						std::cout<<"loaded sound from folder: " << pathStr << "\n";

						SoundData data{ pathStr };
						LoadSound(data); //LoadSound will assert for a valid file extension

						loadedSounds.emplace_back(std::move(data));
					}
				}
			}
			catch (const std::filesystem::filesystem_error& e) 
			{
				std::cerr << "Filesystem error: " << e.what() << std::endl;
			}
			catch (const std::exception& e) 
			{
				std::cerr << "Error: " << e.what() << std::endl;
			}

			return (loadedSounds);
		}

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
		void LoadSound(const SoundData&) noexcept {}
		void UnLoadSound(const std::filesystem::path&) noexcept {}

		void Clear() noexcept {}

		const ChannelId PlaySound(const SoundData&) noexcept { return GameUUID::INVALID_UUID; }
		bool StopPlaying(const ChannelId&) noexcept { return false; }

		void SetChannel3DPosition(const ChannelId&, const glm::vec3&) noexcept {}
		void SetChannelVolume(const ChannelId&, float) noexcept {}

		void SetVFXVolume(const float) noexcept {}
		void SetMusicVolume(const float) noexcept {}

		void Mute() noexcept {}
		void Unmute() noexcept {}
		bool IsMuted() const noexcept { return false; }

		bool Mute(const ChannelId&) noexcept { return false; }
		bool Unmute(const ChannelId&) noexcept { return false; }
		bool IsMuted(const ChannelId&) const noexcept { return false; }
	};
}

#endif