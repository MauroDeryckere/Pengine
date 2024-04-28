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
		SoundData() = default;

		SoundData(const std::filesystem::path& soundPath, const glm::vec3& position = { }, bool is3D = true, bool isLooping = false, bool IsStream = false, float volumedB = 0.f):
			soundUUID{},
			
			soundPath{ soundPath },
			position{ position },

			volumedB{ volumedB },

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
			std::cout << std::format("Volume (dB): {}\n", volumedB);
			std::cout << std::format("Is 3D: {}\n", is3D ? "Yes" : "No");
			std::cout << std::format("Is Looping: {}\n", isLooping ? "Yes" : "No");
			std::cout << std::format("Is Stream: {}\n", isStream ? "Yes" : "No");
		}

		GameUUID soundUUID{ };

		std::filesystem::path soundPath{ };
		glm::vec3 position{ 0,0,0 };

		float volumedB{ 0.f };

		bool is3D{ true };
		bool isLooping{ false };
		bool isStream{ false };
	};
}

#endif