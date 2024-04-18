#ifndef SCENEFILEDATA
#define SCENEFILEDATA

#include <functional>
#include <filesystem>
#include "Serializer.h"

namespace Pengin
{
	struct SceneFileData final
	{
		std::filesystem::path inputFilePath{}; //if exists, clear OR reset existing input (depending on the input file)
		
		std::function<void(const InputData&)> f_RegKeyboardInput;
		std::function<void(const InputData&)> f_RegControllerInput;

		std::filesystem::path sceneLoadPath{};
		std::filesystem::path sceneSavePath{};

		float autoSaveTime{ 0.f }; //0.f == no autosave, no negative number allowed
		bool keepPrevInput{ false };
		bool saveSceneOnDestroy{ false };
	};
}

#endif