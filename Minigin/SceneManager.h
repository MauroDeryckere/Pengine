#ifndef SCENEMANAGER
#define SCENEMANAGER

#include "CoreIncludes.h"
#include "SceneFileData.h"
#include "Scene.h"

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

namespace Pengin
{
	class SceneManager final : public Pengin::Singleton<SceneManager>
	{
	public:
		std::shared_ptr<Scene> CreateScene(const SceneData& sceneData, bool swapToNext = true)  noexcept;

		[[nodiscard]] std::shared_ptr<Scene> GetActiveScene() noexcept { return m_Scenes[m_ActiveSceneIdx]; }

		[[nodiscard]] std::shared_ptr<Scene> GetScene(const std::string& sceneName) noexcept;
		void SetSceneActive(const std::string& sceneName, bool destroyActive = true) noexcept;

		void DestroyScene(const std::string& sceneName) noexcept;

		[[nodiscard]] bool SwitchToNextScene() noexcept;

		void Update();
		void FixedUpdate();
		void Render() const;

		void RenderImGUI();

		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&&) = delete;

	private:
		friend class Pengin::Singleton<SceneManager>;
		SceneManager() = default;
		~SceneManager() = default;

		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::unordered_map<std::string, size_t> m_SceneName_IdMap;

		size_t m_ActiveSceneIdx{ 0 };
		uint32_t m_SceneCounter{ 0 };
	};
}

#endif