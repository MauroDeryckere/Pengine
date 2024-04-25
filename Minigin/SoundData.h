#ifndef PENGIN_SOUNDDATA
#define PENGIN_SOUNDDATA

#include <string>
#include <glm/vec3.hpp>

namespace Pengin
{
	struct SoundData final
	{
		SoundData() = default;

		SoundData(const std::string& soundPath, const glm::vec3& position = {0,0,0}, bool is3D = true, bool isLooping = false, bool IsStream = false, float volumedB):
			soundPath{ soundPath },
			position{ position },

			volumedB{ volumedB },

			is3D{ is3D },
			isLooping{ isLooping },
			isStream{ IsStream }
		{ }

		~SoundData() = default;

		std::string soundPath{ "NO PATH" };
		glm::vec3 position{ 0,0,0 };

		float volumedB{ };

		bool is3D{ true };
		bool isLooping{ false };
		bool isStream{ false };
	};
}

#endif