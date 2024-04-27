#ifndef PENGIN_SOUNDDATA
#define PENGIN_SOUNDDATA

#include <string>
#include <filesystem>
#include <glm/vec3.hpp>

namespace Pengin
{
	struct SoundData final
	{
		SoundData() = default;

		SoundData(const std::filesystem::path& soundPath, const glm::vec3& position = { }, bool is3D = true, bool isLooping = false, bool IsStream = false, float volumedB = 0.f):
			soundPath{ soundPath },
			position{ position },

			volumedB{ volumedB },

			is3D{ is3D },
			isLooping{ isLooping },
			isStream{ IsStream }
		{ }

		~SoundData() = default;

		std::filesystem::path soundPath{ "NO PATH" };
		glm::vec3 position{ 0,0,0 };

		float volumedB{ 0.f };

		bool is3D{ true };
		bool isLooping{ false };
		bool isStream{ false };
	};
}

#endif