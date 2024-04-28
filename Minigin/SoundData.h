#ifndef PENGIN_SOUNDDATA
#define PENGIN_SOUNDDATA

#include "GameUUID.h"

#include <string>
#include <iostream>
#include <filesystem>
#include <format> 
#include <glm/vec3.hpp>

namespace Pengin
{
	struct SoundData final
	{
		enum class SoundType : uint8_t
		{
			VFX,
			Music
		};


		SoundData() = default;

		SoundData(const std::filesystem::path& soundPath, SoundType soundType = SoundType::VFX, const glm::vec3& position = { }, bool is3D = true, bool isLooping = false, bool IsStream = false, float volume = 1.f) :
			soundUUID{},
			soundType{ soundType },

			soundPath{ soundPath },
			position{ position },

			volume{ volume },

			is3D{ is3D },
			isLooping{ isLooping },
			isStream{ IsStream }
		{ }

		~SoundData() = default;

		void PrintSoundInfo() const noexcept
		{
			std::cout << std::format("SoundId: {}\n", soundUUID.GetUUID_PrettyStr());
			std::cout << std::format("SoundPath: {}\n", soundPath.string());
			std::cout << std::format("Position: [{}, {}, {}]\n", position.x, position.y, position.z);
			std::cout << std::format("Volume: {}\n", volume);
			std::cout << std::format("Is 3D: {}\n", is3D ? "Yes" : "No");
			std::cout << std::format("Is Looping: {}\n", isLooping ? "Yes" : "No");
			std::cout << std::format("Is Stream: {}\n", isStream ? "Yes" : "No");
		}

		GameUUID soundUUID{ };

		std::filesystem::path soundPath{ };
		glm::vec3 position{ 0,0,0 };

		float volume{ 0.f };

		bool is3D{ true };
		bool isLooping{ false };
		bool isStream{ false };
		SoundType soundType{ SoundType::VFX };
	};
}

#endif