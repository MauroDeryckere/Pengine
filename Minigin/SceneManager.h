#ifndef SCENEMANAGER
#define SCENEMANAGER

#include "CoreIncludes.h"

#include <vector>
#include <string>
#include <filesystem>
#include <memory>

namespace Pengin
{
	class Scene;
	using std::filesystem::path;

	class SceneManager final : public dae::Singleton<SceneManager>
	{
	public:
		std::shared_ptr<Scene> CreateScene(const std::string& name, const path& sceneLoadPath = {}, const path& sceneSavePath = {}, bool saveOnDestroy = false, bool swapToNext = true);
		[[nodiscard]] std::shared_ptr<Scene> GetActiveScene() noexcept { return m_Scenes[m_ActiveSceneIdx]; }

		[[nodiscard]] std::shared_ptr<Scene> GetScene(const std::string& sceneName);
		void SetSceneActive(const std::string& sceneName, bool destroyActive = true);

		void DestroyScene(const std::string& sceneName);

		[[nodiscard]] bool SwitchToNextScene() noexcept;

		void Update();
		void FixedUpdate();
		void Render() const;

		void RenderImGUI();

	private:
		friend class dae::Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::unordered_map<std::string, size_t> m_SceneName_IdMap;

		size_t m_ActiveSceneIdx{ 0 };
		unsigned m_SceneCounter{ 0 };
	};
}

#endif