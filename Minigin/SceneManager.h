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
		Scene* CreateScene(const SceneData& sceneData, bool setAsActive = true)  noexcept;

		[[nodiscard]] Scene* GetActiveScene() noexcept 
		{
			if (m_ActiveSceneIdx >= 0)
			{
				return m_Scenes[m_ActiveSceneIdx].get(); 
			}

			return nullptr;
		}

		[[nodiscard]] Scene* GetScene(const std::string& sceneName) noexcept;

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

		std::vector<std::unique_ptr<Scene>> m_Scenes{ };
		std::unordered_map<std::string, size_t> m_SceneName_IdMap{ };

		int32_t m_ActiveSceneIdx{ -1 };
	};
}

#endif